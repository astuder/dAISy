/*
 * Simple UART library for MSP430 USCI A0
 * Author: Adrian Studer
 */

#ifndef UART_H_
#define UART_H_

void uart_init(void);							// setup UART peripheral
void uart_send_string(const char* buffer);		// send 0-terminated buffer
void uart_send_byte(uint8_t data);				// send a single byte

#endif /* UART_H_ */
