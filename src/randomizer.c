#include "randomizer.h"
#include "customers.h"
#include <stdlib.h>

Customer choose_customer(Customer previous_customer)
{
    int total_weight = 0;

    // sum weights
    for (int i = 0; i < CUSTOMER_COUNT; i++)
    {
        if (i != previous_customer)
        {
            total_weight += get_customer_info((Customer)i)->weight;
        }
    }

    int random_value = rand() % total_weight;
    int sum = 0;

    for (int i = 0; i < CUSTOMER_COUNT; i++)
    {
        if (i == previous_customer)
            continue;

        sum += get_customer_info((Customer)i)->weight;

        if (random_value < sum)
            return (Customer)i;
    }

    return CUSTOMER_IOT;
}