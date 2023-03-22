#include "pico/stdlib.h"
const uint trigger = 3;
const uint echo = 2;
uint32_t time_1, time_0;

void init_dstmeter()
{
    gpio_init(trigger);
    gpio_init(echo);
    gpio_set_dir(trigger, 1);
    gpio_set_dir(echo, 0);
}

int16_t measure()
{

    gpio_put(trigger, 0);
    sleep_ms(10);

    gpio_put(trigger, 1);
    sleep_us(10);

    gpio_put(trigger, 0);

    while (gpio_get(echo) == 0)
    {
        time_0 = time_us_32();
    }

    while (gpio_get(echo) == 1)
    {
        time_1 = time_us_32();
    }

    uint32_t time_diff = time_1 - time_0;
    uint distance = (time_diff * 34300) / 1000000 / 2;
    if (distance > 200)
    {
        return -1;
    }
    else{
        return distance;
    }
}