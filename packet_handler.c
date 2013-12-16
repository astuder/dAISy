/*
 * AIS packet handler for MSP430 + Si4362
 * Author: Adrian Studer
 */

#include <msp430.h>
#include <inttypes.h>

#include "radio.h"
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

void ph_setup(void)
{
	// configure data pins as inputs
	PH_DATA_SEL &= ~(PH_DATA_CLK_PIN | PH_DATA_PIN);
	PH_DATA_DIR &= ~(PH_DATA_CLK_PIN | PH_DATA_PIN);
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
}

// interrupt handler for receiving data via DATA/DATA_CLK
#pragma vector=PH_DATA_PORT_VECTOR
__interrupt void ph_irq_handler(void)
{
	static uint8_t rx_prev_bit = 0;				// previous bit for NRZI decoding
	static uint16_t rx_bitstream = 0;			// last 16 bits processed

	uint8_t rx_this_bit, rx_bit;

	if(PH_DATA_IFG & PH_DATA_CLK_PIN) {			// verify this interrupt is from DATA_CLK/GPIO_2 pin

		if(PH_DATA_IN & PH_DATA_PIN)	{		// read bit and decode NRZI
			rx_this_bit = 1;
		} else {
			rx_this_bit = 0;
		}
		rx_bit = !(rx_prev_bit^rx_this_bit); 	// NRZI decoding: change = 0-bit, no change = 1-bit, i.e. 00,11=>1, 01,10=>0, i.e. NOT(A XOR B)
		rx_prev_bit = rx_this_bit;

		rx_bitstream >>= 1;						// add bit to bitstream (receiving LSB first)
		if(rx_bit)
		{
			rx_bitstream |= 0x8000;
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
	if(tx_bit == 0) {
		PH_DATA_OUT ^= PH_DATA_PIN;
	}

	// wait a while to roughly simulate 9600 baud @16Mhz
	_delay_cycles(800);

	// send positive clock edge
	PH_DATA_OUT |= PH_DATA_CLK_PIN;

	// wait a while to roughly simulate 9600 baud @16Mhz
	_delay_cycles(800);
}

void test_ph_send_packet(const char* buffer)
{
	// emulating AIS communication received from modem
	// wraps buffer into packet and sends it to packet handler

	uint8_t tx_bit = 0;
	uint8_t tx_byte;
	uint8_t asc_byte;
	unsigned int asc_bit_count;
	unsigned int tx_bit_count;
	unsigned int one_count;

	unsigned int i;
	unsigned int j;


	// send preamble, up to 24 bits
	for(i=20; i!=0; i--) {
		test_ph_send_bit_nrzi(tx_bit);
		tx_bit ^= 1;
	}

	// send sync word
	tx_byte = AIS_SYNC_WORD;
	for(i=8; i!=0; i--) {
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
	while(buffer[j]) {
		// (re)fill transmission byte with next 8 bits
		while(tx_bit_count != 8) {
			if(asc_bit_count == 0) {
				// fetch next character and convert to 6-bit binary
				if(buffer[j]) {
					asc_byte = buffer[j] - 48;
					if(asc_byte > 40) asc_byte -= 6;
					asc_bit_count = 6;
					j++;
				} else {
					// no more bits - should not happen as test messages are 8 bit
					break;
				}
			}

			// shift new ascii bit into transmission byte
			tx_byte <<= 1;
			if(asc_byte & 0x20) tx_byte |= 0x01;
			tx_bit_count++;
			asc_byte <<= 1;
			asc_bit_count--;
		}

		// send bits, LSB first
		while(tx_bit_count != 0) {
			if(tx_byte & 0x01) {
				test_ph_send_bit_nrzi(1);
				one_count++;
			} else {
				test_ph_send_bit_nrzi(0);
				one_count = 0;
			}
			tx_byte >>= 1;
			tx_bit_count--;

			// stuff with 0 after five consecutive 1's
			if(one_count == 5)
			{
				test_ph_send_bit_nrzi(0);
				one_count = 0;
			}
		}
	};

	// TODO: send CRC

	// send sync word
	tx_byte = AIS_SYNC_WORD;
	for(i=8; i!=0; i--) {
		if(tx_byte & 0x01) {
			test_ph_send_bit_nrzi(1);
		} else {
			test_ph_send_bit_nrzi(0);
		}
		tx_byte >>= 1;
	}
}

#endif
