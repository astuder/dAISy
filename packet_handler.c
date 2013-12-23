/*
 * AIS packet handler for MSP430 + Si4362
 * Author: Adrian Studer
 */

#include <msp430.h>
#include <inttypes.h>

#include "radio.h"
#include "fifo.h"
#include "packet_handler.h"

// sync word for AIS
#define AIS_SYNC_WORD		0x7e

// pins that packet handler uses to receive data
#define	PH_DATA_CLK_PIN		BIT2	// 2.2 RX data clock
#define PH_DATA_PIN			BIT3	// 2.3 RX data
#define PH_DATA_PORT		2	  	// data pins are on port 2 (only ports 1 and 2 supported)

// data port dependent defines
#if (PH_DATA_PORT == 1)
#define PH_DATA_IN P1IN
#define PH_DATA_OUT P1OUT
#define PH_DATA_SEL P1SEL
#define PH_DATA_DIR P1DIR
#define PH_DATA_IFG P1IFG
#define PH_DATA_IE P1IE
#define PH_DATA_IES P1IES
#define PH_DATA_PORT_VECTOR PORT1_VECTOR
#elif (PH_DATA_PORT == 2)
#define PH_DATA_IN P2IN
#define PH_DATA_OUT P2OUT
#define PH_DATA_SEL P2SEL
#define PH_DATA_DIR P2DIR
#define PH_DATA_IFG P2IFG
#define PH_DATA_IE P2IE
#define PH_DATA_IES P2IES
#define PH_DATA_PORT_VECTOR PORT2_VECTOR
#else
#error "Packet handler only supports port 1 and 2."
#endif

volatile uint8_t ph_state = PH_STATE_OFF;
volatile uint8_t ph_last_error = PH_ERROR_NONE;

uint8_t ph_error_counts[PH_ERROR_CRC+1];

void ph_setup(void)
{
	// configure data pins as inputs
	PH_DATA_SEL &= ~(PH_DATA_CLK_PIN | PH_DATA_PIN);
	PH_DATA_DIR &= ~(PH_DATA_CLK_PIN | PH_DATA_PIN);
	fifo_reset();
}

void ph_start(void)
{
	// enable interrupt on positive edge of pin wired to DATA_CLK (GPIO2 as configured in radio_config.h)
	PH_DATA_IES &= ~PH_DATA_CLK_PIN;
	PH_DATA_IE |= PH_DATA_CLK_PIN;
	_BIS_SR(GIE);      			// enable interrupts

	// transition radio into receive state
#ifndef TEST
	radio_start_rx(0, 0, 0, RADIO_STATE_NO_CHANGE, RADIO_STATE_NO_CHANGE, RADIO_STATE_NO_CHANGE);
#endif

	// reset packet handler state machine
	ph_last_error = PH_ERROR_NONE;
	ph_state = PH_STATE_INIT;
}

// interrupt handler for receiving data via DATA/DATA_CLK
#pragma vector=PH_DATA_PORT_VECTOR
__interrupt void ph_irq_handler(void)
{
	static uint16_t rx_prev_bit;				// previous bit for NRZI decoding
	static uint16_t rx_bitstream;				// shift register with incoming data
	static uint16_t rx_bit_count;				// bit counter for various purposes
	static uint8_t rx_one_count;				// counter of 1's to identify stuff bits
	static uint8_t rx_data_byte;				// byte to receive actual package data
	static uint16_t rx_crc;						// word for CRC calculation

	uint16_t rx_this_bit, rx_bit;

	if (PH_DATA_IFG & PH_DATA_CLK_PIN) {		// verify this interrupt is from DATA_CLK/GPIO_2 pin

		if (PH_DATA_IN & PH_DATA_PIN)	{		// read bit and decode NRZI
			rx_this_bit = 1;
		} else {
			rx_this_bit = 0;
		}
		rx_bit = !(rx_prev_bit ^ rx_this_bit); 	// NRZI decoding: change = 0-bit, no change = 1-bit, i.e. 00,11=>1, 01,10=>0, i.e. NOT(A XOR B)
		rx_prev_bit = rx_this_bit;

		rx_bitstream >>= 1;						// add bit to bit-stream (receiving LSB first)
		if (rx_bit)
		{
			rx_bitstream |= 0x8000;
		}

		switch (ph_state) {
		case PH_STATE_OFF:									// state: off, do nothing
			break;

		case PH_STATE_INIT:									// state: initialize
			ph_error_counts[ph_last_error]++;
			rx_bitstream = 0;								// reset bit-stream
			fifo_new_packet();								// reset fifo packet
			ph_state = PH_STATE_WAIT_FOR_PREAMBLE;			// next state: wait for training sequence
			break;

		case PH_STATE_WAIT_FOR_PREAMBLE:					// state: waiting for at least 16 bit of training sequence
			if (rx_bitstream == 0x5555) {					// look for 16 alternating bits
				rx_bit_count = 0;							// reset bit counter
				ph_state = PH_STATE_WAIT_FOR_START;			// next state: wait for start flag
			}
			break;

		case PH_STATE_WAIT_FOR_START:						// state: wait for start flag 0x7e
			if ((rx_bitstream & 0xff00) == 0x7e00) {		// if we found the start flag
				rx_bit_count = 0;							// reset bit counter
				ph_state = PH_STATE_RECEIVE_START;			// next state: start receiving packet
				break;
			}

			rx_bit_count++;									// increase bit counter
			if (rx_bit_count > 24) {						// if start flag was not found within 24 bit of preamble
				ph_last_error = PH_ERROR_NOSTART;			// report missing start flag error
				ph_state = PH_STATE_INIT;					// reset state machine
			}
			break;

		case PH_STATE_RECEIVE_START:						// state: pre-fill receive buffer with 8 bits
			rx_bit_count++;									// increase bit counter
			if (rx_bit_count == 8) {						// after 8 bits arrived
				rx_bit_count = 0;							// reset bit counter
				rx_one_count = 0;							// reset counter for stuff bits
				rx_data_byte = 0;							// reset buffer for data byte
				rx_crc = 0xffff;							// init CRC calculation
				ph_state = PH_STATE_RECEIVE_PACKET;			// next state: receive and process packet
				break;
			}

			break;											// do nothing for the first 8 bits to fill buffer

		case PH_STATE_RECEIVE_PACKET:						// state: receiving packet data
			rx_bit = rx_bitstream & 0x80;					// extract data bit for processing

			if (rx_one_count == 5) {						// if we expect a stuff-bit..
				if (rx_bit) {								// if stuff bit is not zero the packet is invalid
					ph_last_error = PH_ERROR_STUFFBIT;		// report invalid stuff-bit error
					ph_state = PH_STATE_INIT;				// restart
				}
				else
					rx_one_count = 0;						// else ignore bit and reset stuff-bit counter
				break;
			}

			rx_data_byte = rx_data_byte >> 1 | rx_bit;		// shift bit into current data byte

			if (rx_bit) {
				rx_one_count++;								// count 1's to identify stuff bit
				rx_bit = 1;									// avoid shifting for CRC
			}
			else rx_one_count = 0;							// or reset stuff-bit counter

			if (rx_bit ^ (rx_crc & 0x0001))					// CCITT CRC calculation (according to Dr. Dobbs)
				rx_crc = (rx_crc >> 1) ^ 0x8408;
			else
				rx_crc >>= 1;

			if ((rx_bit_count & 0x07)==0x07) {				// every 8th bit.. (counter started at 0)
				fifo_write_byte(rx_data_byte);
				rx_data_byte = 0;
			}

			rx_bit_count++;									// count valid, de-stuffed data bits

			if ((rx_bitstream & 0xff00) == 0x7e00) {			// if we found the end flag 0x7e we're done
				if (rx_crc != 0xf0b8) {						// if CRC verification failed
					ph_last_error = PH_ERROR_CRC;			// report CRC error
					ph_state = PH_STATE_INIT;				// restart
					break;
				}
				fifo_commit_packet();
				ph_state = PH_STATE_INIT;					// reset state machine
				break;
			}

			if (rx_bit_count > 1020) {						// if packet is too long, it's probably invalid
				ph_last_error = PH_ERROR_NOEND;				// report error
				ph_state = PH_STATE_INIT;					// reset state machine
				break;
			}

			break;
		}
	}

	PH_DATA_IFG = 0;							// clear all interrupt flags
}

void ph_stop(void)
{
	// disable interrupt on pin wired to GPIO2
	PH_DATA_IE &= ~PH_DATA_CLK_PIN;

	// transition radio from RX to READY
	radio_change_state(RADIO_STATE_READY);
}

uint8_t ph_get_state(void)
{
	return ph_state;
}

uint8_t ph_get_last_error(void)
{
	return ph_last_error;
}

#ifdef TEST
void test_ph_setup(void)
{
	// configure data pins as outputs to test packet handler interrupt routine
	PH_DATA_SEL &= ~(PH_DATA_CLK_PIN | PH_DATA_PIN);
	PH_DATA_DIR |= PH_DATA_CLK_PIN | PH_DATA_PIN;
}

void test_ph_send_bit_nrzi(uint8_t tx_bit)
{
	// send negative clock edge
	PH_DATA_OUT &= ~PH_DATA_CLK_PIN;

	// NRZI = toggle data pin for sending 0, keep state for sending 1
	if (tx_bit == 0)
		PH_DATA_OUT ^= PH_DATA_PIN;

	// wait a while to roughly simulate 9600 baud @16Mhz
	_delay_cycles(800);

	// send positive clock edge
	PH_DATA_OUT |= PH_DATA_CLK_PIN;

	// wait a while to roughly simulate 9600 baud @16Mhz
	_delay_cycles(800);
}

void test_ph_send_packet(const char* message)
{
	// emulating AIS communication received from modem
	// wraps buffer into packet and sends it to packet handler

	uint8_t tx_bit = 0;
	uint8_t tx_byte;
	uint16_t tx_crc = 0xffff;
	uint8_t asc_byte;

	unsigned int asc_bit_count;
	unsigned int tx_bit_count;
	unsigned int one_count;

	unsigned int i;
	unsigned int j;


	// send preamble, up to 24 bits
	for (i = 20; i != 0; i--) {
		test_ph_send_bit_nrzi(tx_bit);
		tx_bit ^= 1;
	}

	// send sync word
	tx_byte = AIS_SYNC_WORD;
	for (i = 8; i != 0; i--) {
		if(tx_byte & 0x01) {
			test_ph_send_bit_nrzi(1);
		} else {
			test_ph_send_bit_nrzi(0);
		}
		tx_byte >>= 1;
	}

	// send packet
	one_count = 0;
	asc_bit_count = 0;
	asc_byte = 0;
	tx_bit_count = 0;
	tx_byte = 0;
	j = 0;
	while (message[j]) {
		// (re)fill transmission byte with next 8 bits
		while (tx_bit_count != 8) {
			if (asc_bit_count == 0) {
				// fetch next character and convert NMEA ASCII to 6-bit binary
				if (message[j]) {
					asc_byte = message[j] - 48;
					if (asc_byte > 40)
						asc_byte -= 8;
					asc_bit_count = 6;
					j++;
				} else {
					// no more bits - should not happen as test messages are 8 bit
					break;
				}
			}

			// shift new ascii bit into transmission byte
			tx_byte <<= 1;
			if (asc_byte & 0x20)
				tx_byte |= 0x01;
			tx_bit_count++;
			asc_byte <<= 1;
			asc_bit_count--;
		}

		// send bits, LSB first
		while (tx_bit_count != 0) {
			if (tx_byte & 0x01) {
				test_ph_send_bit_nrzi(1);
				one_count++;
			} else {
				test_ph_send_bit_nrzi(0);
				one_count = 0;
			}

			if ((tx_byte & 0x01) ^ (tx_crc & 0x0001)) 				// CCITT CRC calculation (according to Dr. Dobbs)
				tx_crc = (tx_crc >> 1) ^ 0x8408;
			else
				tx_crc >>= 1;

			tx_byte >>= 1;
			tx_bit_count--;

			// stuff with 0 after five consecutive 1's
			if (one_count == 5) {
				test_ph_send_bit_nrzi(0);
				one_count = 0;
			}
		}
	};

	// finish calculation of CRC and send CRC LSB first
	tx_crc = ~tx_crc;
//	tx_crc = (tx_crc << 8) | (tx_crc >> 8 & 0xff);			// no byte swap required as we send big-endian
	for (i = 16; i != 0; i--) {
		if (tx_crc & 0x01) {
			test_ph_send_bit_nrzi(1);
			one_count++;
		} else {
			test_ph_send_bit_nrzi(0);
			one_count = 0;
		}
		tx_crc >>= 1;

		// stuff with 0 after five consecutive 1's
		if (one_count == 5) {
			test_ph_send_bit_nrzi(0);
			one_count = 0;
		}
	}

	// send sync word
	tx_byte = AIS_SYNC_WORD;
	for (i = 8; i != 0; i--) {
		if (tx_byte & 0x01)
			test_ph_send_bit_nrzi(1);
		else
			test_ph_send_bit_nrzi(0);
		tx_byte >>= 1;
	}
}

uint8_t test_ph_verify_packet(const char* message)
{
	// read packet from FIFO and encode into NMEA ASCII
	uint8_t packet_size = fifo_get_packet();

	if(!message || packet_size == 0)
		return 0;		// error, no data to verify

	packet_size -= 2;	// ignore CRC

	uint8_t raw_byte;
	uint8_t raw_bit;

	uint8_t nmea_byte;
	uint8_t nmea_bit;
	uint8_t nmea_in;

	nmea_in = 0;
	nmea_byte = 0;
	nmea_bit = 6;

	while (packet_size != 0) {
		raw_byte = fifo_read_byte();
		raw_bit = 8;

		while (raw_bit > 0) {
			nmea_byte <<= 1;
			if (raw_byte & 0x80)
				nmea_byte |= 1;
			nmea_bit--;

			if (nmea_bit == 0) {
				if (nmea_byte > 39)
					nmea_byte += 8;
				nmea_byte += 48;
				if (message[nmea_in++] != nmea_byte)
					return 0;	// error, NMEA message not identical
				nmea_byte = 0;
				nmea_bit = 6;
			}

			raw_byte <<= 1;
			raw_bit--;
		}

		packet_size--;
	}

	// if we have an unfinished NMEA character
	if (nmea_bit != 6)
	{
		// stuff with 0 bits if needed
		while (nmea_bit != 0) {
			nmea_byte <<= 1;
			nmea_bit--;
		}

		// .. and convert and store last byte
		if (nmea_byte > 39)
			nmea_byte += 8;
		nmea_byte += 48;
		if (message[nmea_in] != nmea_byte)
			return 0;	// error, NMEA message not identical
	}

	return 1;	// verification successful, no errors found
}

#endif
