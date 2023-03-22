#include <stdio.h>
#include "pico/stdlib.h"
#include "dstmeter.h"
#include "lcd.h"

int main()
{

    init_dstmeter();
    init_lcd();
    sleep_ms(2000);
    stdio_init_all();
    setup();
    while (true)
    {
        int distance = measure();
        
        printf("Distance read: %d cm\r\n", distance);
        print2lcd(distance);
        sleep_ms(200);
    }
}