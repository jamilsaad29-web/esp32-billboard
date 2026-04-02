#ifndef BILLBOARD_H
#define BILLBOARD_H

#include <stdint.h>

typedef enum
{
    CUSTOMER_HARRY,
    CUSTOMER_GRANDMA,
    CUSTOMER_PETTER,
    CUSTOMER_GOOFY,
    CUSTOMER_IOT,
    CUSTOMER_COUNT
} Customer;

typedef enum
{
    DISPLAY_TEXT,
    DISPLAY_SCROLL,
    DISPLAY_BLINK
} DisplayMode;

typedef struct
{
    const char *text;
    DisplayMode mode;
} Message;

typedef struct
{
    const char *name;
    int weight;
} CustomerInfo;

void billboard_init(void);
uint32_t billboard_minutes_running(void);

#endif