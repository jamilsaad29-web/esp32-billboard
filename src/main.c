
#include <stdlib.h>
 
#include "esp_timer.h"
#include "billboard.h"
#include "display.h"
#include "ads.h"
#include "randomizer.h"
 
void app_main(void)
{
    // esp_timer_get_time() ger mikrosekunder sedan start - fungerar pa ESP32
    // time(NULL) fungerar inte pa ESP32 utan riktig klocka (returnerar 0)
    srand((unsigned int)esp_timer_get_time());
 
    billboard_init();
    display_init();
 
    Customer last = CUSTOMER_COUNT;
 
    while (1)
    {
        Customer current = choose_customer(last);
        Message msg = choose_message(current);
 
        display_message(current, msg);
 
        last = current;
    }
}
 