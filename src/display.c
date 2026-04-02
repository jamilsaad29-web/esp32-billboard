#include "display.h"
#include "customers.h"

#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

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
        lcd_char(*s++);
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

    // FIXAD INIT (VIKTIGT)
    lcd_send_nibble(0x30, 0);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_send_nibble(0x30, 0);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_send_nibble(0x30, 0);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_send_nibble(0x20, 0);
    vTaskDelay(pdMS_TO_TICKS(5));

    lcd_cmd(0x28); // 4-bit mode
    lcd_cmd(0x0C); // display ON
    lcd_cmd(0x06); // entry mode
    lcd_clear();
}

void display_message(Customer customer, Message message)
{
    const CustomerInfo *info = get_customer_info(customer);

    lcd_clear();

    lcd_set(0, 0);
    lcd_print(info->name);

    lcd_set(0, 1);
    lcd_print(message.text);

    vTaskDelay(pdMS_TO_TICKS(20000));
}