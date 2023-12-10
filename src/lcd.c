#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/spi/spidev.h>
#include "spi.h"
#include "lcdfont.h"
#include "lcd.h"
#include <string.h>

void lcd_send_cmd(uint8_t dat)
{
    GPIO_SET_VAL(LCD_DC_PIN, 0);
    spi_send_data(&dat, NULL, 1);
    GPIO_SET_VAL(LCD_DC_PIN, 1);
}
void lcd_send_data(uint8_t dat)
{
    spi_send_data(&dat, NULL, 1);
}
void lcd_send_data_word(uint16_t dat)
{
    uint8_t buf[2] = {dat >> 8, dat & 0xff};
    spi_send_data(buf, NULL, 2);
}

void lcd_gpio_init(int pin)
{
    char path[128];
    char cmd[128] = {0};

    sprintf(path, "/sys/class/gpio/gpio%d", pin);
    if (access(path, F_OK))
    {
        sprintf(cmd, "echo %d >/sys/class/gpio/export", pin);
        system(cmd);

        memset(cmd, 0x00, sizeof(cmd));
        sprintf(cmd, "echo out >/sys/class/gpio/gpio%d/direction", pin);
        system(cmd);
    }
}

void lcd_init()
{
    lcd_gpio_init(LCD_DC_PIN);
    lcd_gpio_init(LCD_BLK_PIN);
    lcd_gpio_init(LCD_RST_PIN);
    spi_dev_init();

    GPIO_SET_VAL(LCD_RST_PIN, 0);
    usleep(1000);
    GPIO_SET_VAL(LCD_RST_PIN, 1);
    usleep(1000);
    GPIO_SET_VAL(LCD_BLK_PIN, 1);;
    usleep(1000);

    lcd_send_cmd(0xEF);
    lcd_send_data(0x03);
    lcd_send_data(0x80);
    lcd_send_data(0x02);

    lcd_send_cmd(0xCF);
    lcd_send_data(0x00);
    lcd_send_data(0XC1);
    lcd_send_data(0X30);

    lcd_send_cmd(0xED);
    lcd_send_data(0x64);
    lcd_send_data(0x03);
    lcd_send_data(0X12);
    lcd_send_data(0X81);

    lcd_send_cmd(0xE8);
    lcd_send_data(0x85);
    lcd_send_data(0x00);
    lcd_send_data(0x78);

    lcd_send_cmd(0xCB);
    lcd_send_data(0x39);
    lcd_send_data(0x2C);
    lcd_send_data(0x00);
    lcd_send_data(0x34);
    lcd_send_data(0x02);

    lcd_send_cmd(0xF7);
    lcd_send_data(0x20);

    lcd_send_cmd(0xEA);
    lcd_send_data(0x00);
    lcd_send_data(0x00);

    lcd_send_cmd(ILI9341_PWCTR1);
    lcd_send_data(0x23);

    lcd_send_cmd(ILI9341_PWCTR2);
    lcd_send_data(0x10);

    lcd_send_cmd(ILI9341_VMCTR1);
    lcd_send_data(0x3e);
    lcd_send_data(0x28);

    lcd_send_cmd(ILI9341_VMCTR2);
    lcd_send_data(0x86);

    lcd_send_cmd(ILI9341_MADCTL);
    lcd_send_data(TFT_MAD_MX | TFT_MAD_COLOR_ORDER);

    lcd_send_cmd(ILI9341_PIXFMT);
    lcd_send_data(0x55);

    lcd_send_cmd(ILI9341_FRMCTR1);
    lcd_send_data(0x00);
    lcd_send_data(0x13);

    lcd_send_cmd(ILI9341_DFUNCTR);
    lcd_send_data(0x08);
    lcd_send_data(0x82);
    lcd_send_data(0x27);

    lcd_send_cmd(0xF2);
    lcd_send_data(0x00);

    lcd_send_cmd(ILI9341_GAMMASET);
    lcd_send_data(0x01);

    lcd_send_cmd(ILI9341_GMCTRP1);
    lcd_send_data(0x0F);
    lcd_send_data(0x31);
    lcd_send_data(0x2B);
    lcd_send_data(0x0C);
    lcd_send_data(0x0E);
    lcd_send_data(0x08);
    lcd_send_data(0x4E);
    lcd_send_data(0xF1);
    lcd_send_data(0x37);
    lcd_send_data(0x07);
    lcd_send_data(0x10);
    lcd_send_data(0x03);
    lcd_send_data(0x0E);
    lcd_send_data(0x09);
    lcd_send_data(0x00);

    lcd_send_cmd(ILI9341_GMCTRN1);
    lcd_send_data(0x00);
    lcd_send_data(0x0E);
    lcd_send_data(0x14);
    lcd_send_data(0x03);
    lcd_send_data(0x11);
    lcd_send_data(0x07);
    lcd_send_data(0x31);
    lcd_send_data(0xC1);
    lcd_send_data(0x48);
    lcd_send_data(0x08);
    lcd_send_data(0x0F);
    lcd_send_data(0x0C);
    lcd_send_data(0x31);
    lcd_send_data(0x36);
    lcd_send_data(0x0F);

    lcd_send_cmd(TFT_MADCTL);
    lcd_send_data(TFT_MAD_MX | TFT_MAD_COLOR_ORDER);

    lcd_send_cmd(ILI9341_SLPOUT);
    
    lcd_send_cmd(ILI9341_DISPON);

}

void lcd_deinit()
{
    spi_dev_deinit();
    lcd_gpio_deinit(LCD_DC_PIN);
    lcd_gpio_deinit(LCD_BLK_PIN);
    lcd_gpio_deinit(LCD_RST_PIN);
}

void lcd_gpio_deinit(int pin)
{
    char path[128];
    char cmd[128] = {0};

    sprintf(path, "/sys/class/gpio/gpio%d", pin);
    if (!access(path, F_OK))
    {
        sprintf(cmd, "echo %d >/sys/class/gpio/unexport", pin);
        system(cmd);
    }
}


void lcd_set_address(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
    lcd_send_cmd(0x2a);
    lcd_send_data_word(x1);
    lcd_send_data_word(x2);
    lcd_send_cmd(0x2b);
    lcd_send_data_word(y1);
    lcd_send_data_word(y2);
    lcd_send_cmd(0x2c);
}

void lcd_drawpoint(uint16_t x,uint16_t y,uint16_t color)
{
    lcd_set_address(x,y,x,y); 
    lcd_send_data_word(color);
}

void lcd_char(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey)
{
    uint8_t temp,sizex,t,m=0;
    uint16_t i,TypefaceNum;
    uint16_t x0=x;
    sizex=sizey/2;
    TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
    num=num-' ';
    lcd_set_address(x,y,x+sizex-1,y+sizey-1);
    for(i=0;i<TypefaceNum;i++)
    { 
        if(sizey<=12)temp=ascii_1206[num][i];
        else if(sizey<=16)temp=ascii_1608[num][i];
        else if(sizey<=24)temp=ascii_2412[num][i];
        else return;
        for(t=0;t<8;t++)
        {
            if(temp&(0x01<<t))lcd_send_data_word(fc);
            else lcd_send_data_word(bc);
            m++;
            if(m%sizex==0)
            {
                m=0;
                break;
            }
        }
    }
}

void lcd_string(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey)
{
    while(*p!='\0')
    {
        lcd_char(x,y,*p,fc,bc,sizey);
        x+=sizey/2;
        p++;
    }
}

void lcd_fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{
    uint16_t i,j;
    lcd_set_address(xsta,ysta,xend,yend);
    uint8_t buffer[MAX_BUF_LEN];
    int all = (xend - xsta) * (yend - ysta)*2;
    int time = all / 4096;

    for (i = 0; i < MAX_BUF_LEN; i += 2)
    {
        buffer[i] = color >> 8;
        buffer[i + 1] = color & 0x00;
    }
    while(time--)
        spi_send_data(buffer, NULL, MAX_BUF_LEN);
    spi_send_data(buffer, NULL, all%MAX_BUF_LEN);
}
