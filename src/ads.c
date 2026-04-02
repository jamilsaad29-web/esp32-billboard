#include "ads.h"
#include "billboard.h"
#include <stdlib.h>

Message choose_message(Customer customer)
{
    Message msg;

    switch (customer)
    {
        case CUSTOMER_HARRY:
        {
            int r = rand() % 3; // random 3

            if (r == 0)
            {
                msg.text = "Buy a car from Harry";
                msg.mode = DISPLAY_SCROLL;
            }
            else if (r == 1)
            {
                msg.text = "A good car deal for Harry";
                msg.mode = DISPLAY_TEXT;
            }
            else
            {
                msg.text = "Honest Harry Cars";
                msg.mode = DISPLAY_BLINK;
            }
            break;
        }

        case CUSTOMER_GRANDMA:
        {
            int r = rand() % 2;

            if (r == 0)
            {
                msg.text = "Buy pie from Grandma Duck";
                msg.mode = DISPLAY_SCROLL;
            }
            else
            {
                msg.text = "Hurry before Martin eats all pies";
                msg.mode = DISPLAY_TEXT;
            }
            break;
        }

        case CUSTOMER_PETTER:
        {
            uint32_t minute = billboard_minutes_running();

            if (minute % 2 == 0) // even
            {
                msg.text = "Let Petter build for you";
                msg.mode = DISPLAY_SCROLL;
            }
            else // odd
            {
                msg.text = "Build in secret Call Petter";
                msg.mode = DISPLAY_TEXT;
            }
            break;
        }

        case CUSTOMER_GOOFY:
        {
            int r = rand() % 2;

            if (r == 0)
                msg.text = "Mysteries Call Goofy";
            else
                msg.text = "Goofy solves the case";

            msg.mode = DISPLAY_TEXT;
            break;
        }

        case CUSTOMER_IOT:
        default:
        {
            msg.text = "Advertise here IOT Agency";
            msg.mode = DISPLAY_TEXT;
            break;
        }
    }

    return msg;
}