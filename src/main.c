#include <stdlib.h>
#include <time.h>

#include "billboard.h"
#include "display.h"
#include "ads.h"
#include "randomizer.h"

void app_main(void)
{
    srand((unsigned int)time(NULL));

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