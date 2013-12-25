/*
 * dAISy 0.2 - A simple AIS receiver
 * Author: Adrian Studer
 * License: CC BY-NC-SA Creative Commons Attribution-NonCommercial-ShareAlike
 * 			http://creativecommons.org/licenses/by-nc-sa/4.0/
 * 			Please contact the author if you want to use this work in a commercial product
 */

#include <msp430.h> 
#include <inttypes.h>

#include "fifo.h"
#include "uart.h"
#include "dec_to_str.h"

#include "radio.h"
#include "packet_handler.h"
#include "nmea.h"

#define DEBUG_MESSAGES			// un-comment to send error messages over UART

#define LED1	BIT0

#define LED_ON		P1OUT |= LED1
#define LED_OFF 	P1OUT &= ~LED1
#define LED_TOGGLE	P1OUT ^= LED1

#ifdef TEST
void test_main(void);
void test_packet_handler(const char* message);
void test_error(void);
#endif

int main(void)
{
	// configure WDT
	WDTCTL = WDTPW | WDTHOLD;				// stop watch dog timer

	// set clock to 16MHz
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;
	BCSCTL2 = 0;							// MCLK and SMCLK = DCO = 16MHz

#ifdef TEST
	// when compiled in test mode, use different main
	// disconnect radio when testing to avoid damage!
	test_main();
#endif

	// configure LED1 and turn it off, we'll use that for error and other stuff
	P1DIR |= LED1;
	LED_OFF;

	// setup uart
	uart_init();

	// setup packet handler
	ph_setup();

	// setup an configure radio
	radio_setup();
	radio_configure();

	// verify that radio configuration was successful
	radio_get_chip_status(0);
	if (radio_buffer.chip_status.chip_status & RADIO_CMD_ERROR) {	// check for command error
		while (1) {
			LED_TOGGLE;
			_delay_cycles(8000000);			// blink LED if there was an error
		}
	}

	// start packet receiving
	ph_start();

#ifdef DEBUG_MESSAGES
	uart_send_string("dAISy 0.2 started\r\n");

	// some information
	uint8_t current_state;
	uint8_t previous_state = ph_get_state();
#endif

	while (1) {

#ifdef DEBUG_MESSAGES
		// debug code to monitor signal strength (RSSI)
		current_state = ph_get_state();
		if (current_state != previous_state) {
			switch(current_state) {
			case PH_STATE_PREFETCH:																// found preamble and start flag
				// report current signal strength
				radio_get_modem_status(0);														// check modem for current RSSI
				int16_t rssi = ((int) radio_buffer.modem_status.curr_rssi >> 1) - 0x40 - 70;	// calculate dBm: RSSI / 2 - RSSI_COMP - 70
				dec_to_str(radio_buffer.data, 3, rssi);											// convert to decimal string (reuse radio buffer)
				radio_buffer.data[4] = 0;															// terminate string
				uart_send_string("packet start, RSSI=");												// send message to UART
				uart_send_string(radio_buffer.data);
				uart_send_string("dBm\r\n");
				break;
			}
			previous_state = current_state;
		}
#endif

		// retrieve last packet handler error
		uint8_t error = ph_get_last_error();
#ifdef DEBUG_MESSAGES
		// report error if packet handler failed after finding preamble and start flag
		if (error == PH_ERROR_NOEND) {
			uart_send_string("error: no end flag\r\n");
		} else if (error == PH_ERROR_STUFFBIT) {
			uart_send_string("error: invalid stuff bit\r\n");
		} else if (error == PH_ERROR_CRC) {
			uart_send_string("error: CRC error\r\n");
		}
#else
		// toggle LED if packet handler failed after finding preamble and start flag
		if (error == PH_ERROR_NOEND || error == PH_ERROR_STUFFBIT || error == PH_ERROR_CRC)
			LED_TOGGLE;
#endif

		// check if a new valid packet arrived
		uint16_t size = fifo_get_packet();
		if (size > 0) {								// if so, process packet
			nmea_process_packet();					// process packet (NMEA message will be sent over UART)
			fifo_remove_packet();					// remove processed packet from FIFO
		}

		// enter low power mode LPM0 (everything off)
		// TODO: wait for UART to complete transmission
		// TODO: suspend UART
		// TODO: enter low power mode
	}
}

#ifdef TEST
// AIS test messages, more samples see http://www.aishub.net/nmea-sample.html
const char test_message_0[] = "133sVfPP00PD>hRMDH@jNOvN20S8";
const char test_message_1[] = "13u?etPv2;0n:dDPwUM1U1Cb069D";
const char test_message_2[] = "100h00PP0@PHFV`Mg5gTH?vNPUIp";


void test_main(void)
{
	radio_shutdown();	// put radio into shutdown to avoid damage when writing to radio output pins

	// testing dec_to_str
	char test_buf[10];
	const uint32_t a = 1023;
	udec_to_str(test_buf, 9, a);
	const int32_t b = -1023;
	dec_to_str(test_buf, 9, b);

	// setup uart
	uart_init();

	// setup packet handler
	ph_setup();
	test_ph_setup();

	// start packet receiving
	ph_start();

	while (1) {
		// testing packet handler
		_delay_cycles(10000000);
		test_packet_handler(test_message_0);
		_delay_cycles(10000000);
		test_packet_handler(test_message_1);
		_delay_cycles(10000000);
		test_packet_handler(test_message_2);
	}
}

void test_packet_handler(const char* message)
{
	// send/receive fake AIS message
	test_ph_send_packet(message);

	// verify packet handler operation
	if (ph_get_last_error() != PH_ERROR_NONE)
		test_error();

	// verify if a new packet showed up in FIFO
	uint16_t packet_size = fifo_get_packet();
	if (packet_size == 0)
		test_error();

	// verify if new packet is identical with sent message
	if (!test_nmea_verify_packet(message))
		test_error();

	nmea_process_packet();

	// remove packet from FIFO
	fifo_remove_packet();
}

void test_error(void)
{
	while (1) {
		LED_TOGGLE;
		_delay_cycles(8000000);			// blink LED if there was an error
	}
}

#endif

// handler for unuxpected interrupts
#pragma vector=ADC10_VECTOR,COMPARATORA_VECTOR,NMI_VECTOR,PORT1_VECTOR,	\
			   TIMER0_A0_VECTOR,TIMER0_A1_VECTOR,TIMER1_A0_VECTOR,TIMER1_A1_VECTOR,		\
			   USCIAB0RX_VECTOR,USCIAB0TX_VECTOR,WDT_VECTOR
__interrupt void ISR_trap(void)
{
	// trap CPU & code execution here with an infinite loop
	while (1);
}
