/*
 * Simple UART library for MSP430 USCI A0
 * Author: Adrian Studer
 */

#include <msp430.h>
#include <inttypes.h>
#include "uart.h"

#define UART_BAUD	9600	// baud rate, use 9600 for MSP430G2 LaunchPad, better safe than sorry

#define UART_RX		BIT1	// RX pin 1.1
#define UART_TX		BIT2	// TX pin 1.2

// timing configuration according to table 15.4 in TI MSP430x2xx Family Guide (SLAU144J)
#if (UART_BAUD == 9600)
#define UART_PRESCALER 1666
#define UART_MODULATOR 6
#elif (UART_BAUD == 19200)
#define UART_PRESCALER 833
#define UART_MODULATOR 2
#elif (UART_BAUD == 38400)
#define UART_PRESCALER 416
#define UART_MODULATOR 6
#else
#error "This UART library only supports 9600, 19200 or 38400 baud"
#endif

void uart_init(void)
{
	// configure UCSI A0 for UART

	// put USCI A0 into reset state
	UCA0CTL1 |= UCSWRST;

	// initialize USCI A0 registers
	UCA0CTL0 = 0;									// 8N1 vanilla serial
	UCA0CTL1 |= UCSSEL_2;							// clock source SMCLK (keep USCI in reset state)
	UCA0BR0 = UART_PRESCALER & 0xff;				// configure clock generation according to selected baud rate
	UCA0BR1 = UART_PRESCALER >> 8;
	UCA0MCTL = UART_MODULATOR << 1;

	// initialize UART pins on port 1
	P1DIR |= UART_TX;								// set TX pin as output
	P1DIR &= ~UART_RX;								// set RX pin as input
	P1SEL |= UART_RX | UART_TX;						// connect pins to USCI (secondary peripheral)
	P1SEL2 |= UART_RX | UART_TX;					// connect pins to USCI (secondary peripheral)

	UCA0CTL1 &= ~UCSWRST;							// enable USCI A0
}

void uart_send_string(const char* buffer)
{
	uint16_t i = 0;
	while (buffer[i])
	{
		while (!(IFG2 & UCA0TXIFG));					// wait for UART to be ready for TX
		UCA0TXBUF = buffer[i];
		i++;
	}
}

void uart_send_byte(uint8_t data)
{
	while (!(IFG2 & UCA0TXIFG));					// wait for UART to be ready for TX
	UCA0TXBUF = data;
}

