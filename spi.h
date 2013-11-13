/*
 * Simple SPI library for MSP430 USCI B0
 * Author: Adrian Studer
 */

#ifndef SPI_H_
#define SPI_H_

#define SPI_CLK		BIT5	// clock pin 1.5
#define SPI_MISO	BIT6	// MISO pin 1.6
#define SPI_MOSI	BIT7	// MOSI pin 1.7

void spi_init(void);
uint8_t spi_transfer(const uint8_t data);

#endif /* SPI_H_ */
