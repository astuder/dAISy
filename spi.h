/*
 * Simple SPI library for MSP430 USCI B0
 * Author: Adrian Studer
 */

#ifndef SPI_H_
#define SPI_H_

void spi_init(void);

inline uint8_t spi_transfer(const uint8_t data)
{
	UCB0TXBUF = data;
	while (UCB0STAT & UCBUSY);
	return UCB0RXBUF;
}

#endif /* SPI_H_ */
