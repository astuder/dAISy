/*
 * Simple SPI library for MSP430 USCI B0
 * Author: Adrian Studer
 */

#include <msp430.h>
#include <inttypes.h>
#include "spi.h"

#define SPI_CLK		BIT5	// clock pin 1.5
#define SPI_MISO	BIT6	// MISO pin 1.6
#define SPI_MOSI	BIT7	// MOSI pin 1.7

void spi_init(void)
{
	// initialize SPI pins
	P1DIR |= SPI_CLK | SPI_MOSI;					// set SPI clock, MOSI pins to outputs
	P1DIR &= ~SPI_MISO;								// set SPI MISO as input
	P1SEL |= SPI_CLK | SPI_MOSI | SPI_MISO;			// connect pins to USCI (secondary peripheral)
	P1SEL2 |= SPI_CLK | SPI_MOSI | SPI_MISO;		// connect pins to USCI (secondary peripheral)
	P1OUT &= ~(SPI_CLK | SPI_MOSI);					// set clock and MOSI pins to low

	// configure UCSI B0 for SPI
	UCB0CTL1 |= UCSWRST;							// reset USCI B0
	UCB0CTL0 = UCCKPH | UCMST | UCMSB | UCMODE_0 | UCSYNC;	// read on rising edge, inactive clk low, 8 bit, master mode, 3 pin SPI, synchronous
	UCB0BR0 = 8; UCB0BR1 = 0;						// clock scaler = 2, i.e 8 MHz SPI clock (Si4362 max is 10 MHz)
	UCB0CTL1 = UCSSEL_2;							// clock source SMCLK, clear UCSWRST to enable USCI B0
	UCB0CTL1 &= ~UCSWRST;							// enable USCI B0
}

uint8_t spi_transfer(const uint8_t data)
{
	UCB0TXBUF = data;
	while (UCB0STAT & UCBUSY);
	return UCB0RXBUF;
}

