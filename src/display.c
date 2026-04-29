#include "display.h"
#include "customers.h"
 
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
 
#define SDA 21
#define SCL 22
#define I2C_NUM I2C_NUM_0
#define FREQ 100000
 
#define LCD_ADDR 0x27
 
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE 0x04
#define LCD_RS 0x01
 
static void lcd_send(uint8_t data)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
 
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (LCD_ADDR << 1), true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
 
    i2c_master_cmd_begin(I2C_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}
 
static void lcd_send_nibble(uint8_t data, uint8_t mode)
{
    uint8_t d = data | LCD_BACKLIGHT | mode;
 
    lcd_send(d | LCD_ENABLE);
    vTaskDelay(pdMS_TO_TICKS(1));
    lcd_send(d & ~LCD_ENABLE);
}
 
static void lcd_write(uint8_t val, uint8_t mode)
{
    lcd_send_nibble(val & 0xF0, mode);
    lcd_send_nibble((val << 4) & 0xF0, mode);
}
 
static void lcd_cmd(uint8_t cmd)
{
    lcd_write(cmd, 0);
    vTaskDelay(pdMS_TO_TICKS(2));
}
 
static void lcd_char(char c)
{
    lcd_write(c, LCD_RS);
}
 
static void lcd_clear()
{
    lcd_cmd(0x01);
    vTaskDelay(pdMS_TO_TICKS(2));
}
 
static void lcd_set(int col, int row)
{
    int row_offset[] = {0x00, 0x40};
    lcd_cmd(0x80 | (col + row_offset[row]));
}
 
static void lcd_print(const char *s)
{
    while (*s)
    {
        lcd_char(*s++);
    }
}
 
void display_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA,
        .scl_io_num = SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = FREQ
    };
 
    i2c_param_config(I2C_NUM, &conf);
    i2c_driver_install(I2C_NUM, conf.mode, 0, 0, 0);
 
    vTaskDelay(pdMS_TO_TICKS(50));
 
    lcd_send_nibble(0x30, 0);
    vTaskDelay(pdMS_TO_TICKS(5));
 
    lcd_send_nibble(0x30, 0);
    vTaskDelay(pdMS_TO_TICKS(5));
 
    lcd_send_nibble(0x30, 0);
    vTaskDelay(pdMS_TO_TICKS(5));
 
    lcd_send_nibble(0x20, 0);
    vTaskDelay(pdMS_TO_TICKS(5));
 
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_clear();
}
 
// Print up to 16 chars of a string starting at offset, on a given row
static void lcd_print_row(const char *s, int offset, int row)
{
    lcd_set(0, row);
    int len = (int)strlen(s);
    for (int col = 0; col < 16; col++)
    {
        int idx = offset + col;
        if (idx < len)
            lcd_char(s[idx]);
        else
            lcd_char(' '); // pad with spaces to clear leftover chars
    }
}
 
// Scroll a single row of text from right to left across the screen
static void lcd_scroll_row(const char *text, int row, int total_steps, int step_delay_ms)
{
    int len = (int)strlen(text);
    for (int i = 0; i < total_steps; i++)
    {
        lcd_set(0, row);
        for (int col = 0; col < 16; col++)
        {
            int idx = i - 16 + col;
            if (idx >= 0 && idx < len)
                lcd_char(text[idx]);
            else
                lcd_char(' ');
        }
        vTaskDelay(pdMS_TO_TICKS(step_delay_ms));
    }
}
 
void display_message(Customer customer, Message message)
{
    const CustomerInfo *info = get_customer_info(customer);
    int name_len  = (int)strlen(info->name);
    int msg_len   = (int)strlen(message.text);
 
    if (message.mode == DISPLAY_TEXT)
    {
        // If name or message is longer than 16 chars, scroll that row
        // otherwise just print it statically
        int name_steps = (name_len > 16) ? name_len + 16 : 1;
        int msg_steps  = (msg_len  > 16) ? msg_len  + 16 : 1;
        int total_steps = (name_steps > msg_steps) ? name_steps : msg_steps;
 
        for (int i = 0; i < total_steps; i++)
        {
            // Row 0: customer name
            if (name_len > 16)
            {
                lcd_set(0, 0);
                for (int col = 0; col < 16; col++)
                {
                    int idx = i - 16 + col;
                    lcd_char((idx >= 0 && idx < name_len) ? info->name[idx] : ' ');
                }
            }
            else if (i == 0)
            {
                lcd_set(0, 0);
                lcd_print(info->name);
            }
 
            // Row 1: message text
            if (msg_len > 16)
            {
                lcd_set(0, 1);
                for (int col = 0; col < 16; col++)
                {
                    int idx = i - 16 + col;
                    lcd_char((idx >= 0 && idx < msg_len) ? message.text[idx] : ' ');
                }
            }
            else if (i == 0)
            {
                lcd_set(0, 1);
                lcd_print(message.text);
            }
 
            vTaskDelay(pdMS_TO_TICKS(300));
        }
 
        // If both rows were static, wait out the remaining 20 seconds
        if (name_len <= 16 && msg_len <= 16)
            vTaskDelay(pdMS_TO_TICKS(20000));
    }
    else if (message.mode == DISPLAY_SCROLL)
    {
        // Both rows scroll together at the same speed
        int name_steps = name_len + 16;
        int msg_steps  = msg_len  + 16;
        int total_steps = (name_steps > msg_steps) ? name_steps : msg_steps;
 
        for (int i = 0; i < total_steps; i++)
        {
            // Row 0: customer name
            lcd_set(0, 0);
            for (int col = 0; col < 16; col++)
            {
                int idx = i - 16 + col;
                lcd_char((idx >= 0 && idx < name_len) ? info->name[idx] : ' ');
            }
 
            // Row 1: message text
            lcd_set(0, 1);
            for (int col = 0; col < 16; col++)
            {
                int idx = i - 16 + col;
                lcd_char((idx >= 0 && idx < msg_len) ? message.text[idx] : ' ');
            }
 
            vTaskDelay(pdMS_TO_TICKS(300));
        }
    }
    else if (message.mode == DISPLAY_BLINK)
    {
        // Blink for 20 seconds (20 x 1000ms)
        for (int i = 0; i < 20; i++)
        {
            // Show - scroll name/text if too long, otherwise print static
            lcd_clear();
            lcd_print_row(info->name,    0, 0);
            lcd_print_row(message.text,  0, 1);
            vTaskDelay(pdMS_TO_TICKS(500));
 
            // Hide
            lcd_clear();
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}
 