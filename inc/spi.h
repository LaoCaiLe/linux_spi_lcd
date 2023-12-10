#ifndef _SPI_H
#define _SPI_H

#define SUCCESS 0
#define FAIL 1
#define MAX_BUF_LEN 4096
#include <stdint.h>

uint8_t spi_dev_init();
void spi_dev_deinit();
void spi_send_data(uint8_t const *tx, uint8_t const *rx, size_t len);

#endif