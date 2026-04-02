#include "customers.h"

static const CustomerInfo customers[CUSTOMER_COUNT] =
{
    {"Honest Harry Cars",      5000},
    {"Grandma Duck Pies",      3000},
    {"Blackbuilder Petter",    1500},
    {"Goofy Detective Agency", 4000},
    {"IOT Advertising",        1000}
};

const CustomerInfo *get_customer_info(Customer customer)
{
    return &customers[customer]; // return customer
}