#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "lcd.h"
const uint rs = 16;
const uint e = 17;

const uint d4 = 18;
const uint d5 = 19;
const uint d6 = 20;
const uint d7 = 21;

const char text_before[] = "Distance: ";
const char text_after[] = "cm.";
const int8_t line_size = 16;
int8_t free_space;
void init_lcd()
{
    gpio_init(rs);
    gpio_init(e);

    gpio_init(d4);
    gpio_init(d5);
    gpio_init(d6);
    gpio_init(d7);
    // uint32_t mask = 0;
    /*   mask = mask | (1 << rs) | (1 << e) | (1 << d4) | (1 << d5) | (1 << d6) | (1 << d7);
      gpio_set_dir_out_masked(mask);
   */
    gpio_set_dir(rs, 1);
    gpio_set_dir(e, 1);
    gpio_set_dir(d4, 1);
    gpio_set_dir(d5, 1);
    gpio_set_dir(d6, 1);
    gpio_set_dir(d7, 1);
}

void pulseE()
{
    gpio_put(e, 1);
    sleep_us(40);
    gpio_put(e, 0);
    sleep_us(40);
}

void send2lcd4(char binary)
{
    gpio_put(d4, (binary & 0b00000001) >> 0);
    gpio_put(d5, (binary & 0b00000010) >> 1);
    gpio_put(d6, (binary & 0b00000100) >> 2);
    gpio_put(d7, (binary & 0b00001000) >> 3);
    pulseE();
}

void send2lcd8(char binary)
{
    gpio_put(d4, (binary & 0b00010000) >> 4);
    gpio_put(d5, (binary & 0b00100000) >> 5);
    gpio_put(d6, (binary & 0b01000000) >> 6);
    gpio_put(d7, (binary & 0b10000000) >> 7);
    pulseE();
    gpio_put(d4, (binary & 0b00000001) >> 0);
    gpio_put(d5, (binary & 0b00000010) >> 1);
    gpio_put(d6, (binary & 0b00000100) >> 2);
    gpio_put(d7, (binary & 0b00001000) >> 3);
    pulseE();
}

void setup()
{
    gpio_put(rs, 0);

    send2lcd4(0b0011);     // #8 bit
    send2lcd4(0b0011);     // #8 bit
    send2lcd4(0b0011);     // #8 bit
    send2lcd4(0b0010);     // #4 bit
    send2lcd8(0b00101000); // #4 bit,2 lines?,5*8 bots
    send2lcd8(0b00001100); // #lcd on, blink off, cursor off.
    send2lcd8(0b00000110); // #increment cursor, no display shift
    send2lcd8(0b00000001); // #clear screen
    sleep_ms(2);           // #clear screen needs a long delay

    // print constant text
    gpio_put(rs, 1);
    for (const char *ptr = text_before; *ptr != '\0'; ptr++)
    {
        send2lcd8(*ptr);
    }
    // count free space in lcd line
    free_space = line_size - strlen(text_before);
}
void print2lcd(int16_t distance)
{
    gpio_put(rs, 1);

    char text[free_space + 1];
    if (distance < 0)
    {
        strcpy(text, " -out-");
    }
    else
    {
        sprintf(text, "%3d", distance);
        strcat(text, text_after);
   }

 
    for (const char *ptr = text; *ptr != '\0'; ptr++)
    {
        send2lcd8(*ptr);
    }

    // reset cursor position
    gpio_put(rs, 0);

    int8_t cursor2move = free_space;
    while (cursor2move > 0)
    {
        cursor2move--;
        send2lcd8(0b00010000); // move cursor left
    }
}