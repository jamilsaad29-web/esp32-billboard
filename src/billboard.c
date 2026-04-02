#include "billboard.h"
#include "esp_timer.h"

static int64_t start_time_us = 0;

void billboard_init(void)
{
    start_time_us = esp_timer_get_time(); // save start time
}

uint32_t billboard_minutes_running(void)
{
    int64_t now_us = esp_timer_get_time();
    return (uint32_t)((now_us - start_time_us) / 1000000ULL / 60ULL); // minutes
}