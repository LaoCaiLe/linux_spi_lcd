#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/spi/spidev.h>
#include "spi.h"
#include <string.h>

static uint32_t mode = SPI_MODE_2;
static uint8_t bits = 8;
static uint32_t speed = 200000000;
static uint16_t delay;

char *spi_dev_fd_name = "/dev/spidev3.0";
int spi_dev_fd;

void pabort(const char *s)
{
    perror(s);
    abort();
}

uint8_t spi_dev_init()
{
    int ret;
    spi_dev_fd = open(spi_dev_fd_name, O_RDWR);
    ret = ioctl(spi_dev_fd, SPI_IOC_WR_MODE32, &mode);
    if (ret == -1){
        pabort("can't set spi mode");
        return FAIL;
    }

    ret = ioctl(spi_dev_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
    {
        pabort("can't set bits per word");
        return FAIL;
    }

    ret = ioctl(spi_dev_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
    {
        pabort("can't set max speed hz");
        return FAIL;
    }

    return SUCCESS;
}
void spi_dev_deinit()
{
    close(spi_dev_fd);
}

void spi_send_data(uint8_t const *tx, uint8_t const *rx, size_t len)
{
    int ret;

    if(len > MAX_BUF_LEN)
    {
        pabort("send buffer length is too long\n");
    }
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = len,
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
        .tx_nbits = 1,
        .rx_nbits = 1
    };

    ret = ioctl(spi_dev_fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
    {
      pabort("can't send spi message");
    }
}


