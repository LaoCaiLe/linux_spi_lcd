#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "lcd.h"
uint16_t color_vector[] = {
    WHITE,
    BLACK,
    BLUE,
    BRED,
    GRED,
    GBLUE,
    RED,
    MAGENTA,
    GREEN,
    CYAN,
    YELLOW,
    BROWN,
    BRRED,
    GRAY,
    DARKBLUE,
    LIGHTBLUE,
    GRAYBLUE,
    LIGHTGREEN,
    LGRAY,
    LGRAYBLUE,
    LBBLUE,
};

int main(int argc, char* argv[])
{
    uint8_t i = 0;
    lcd_init();
    
    while(1)
    {
        lcd_fill(0, 0, LCD_WIDTH, LCD_HEIGHT, color_vector[i++]);
        sleep(1);
        i %= (sizeof(color_vector) / sizeof(uint16_t));
    }
    lcd_deinit();

    return 0;
}

