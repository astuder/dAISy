/*
 * AIS packet handler for MSP430 + Si4362
 * Author: Adrian Studer
 */

#include <msp430.h>
#include <inttypes.h>

#include "radio.h"
#include "packet_handler.h"

#define	DATA_CLK_PIN		BIT2	// 2.2 RX data clock
#define DATA_PIN			BIT3	// 2.3 RX data

#define RX_DATA_CLK			(P2IN & DATA_CLK_PIN)
#define RX_DATA				(P2IN & DATA_PIN)

void ph_setup(void)
{
	// configure data pins as inputs
	P2SEL &= ~(DATA_CLK_PIN | DATA_PIN);
	P2DIR &= ~(DATA_CLK_PIN | DATA_PIN);
}

void ph_start(void)
{
	// enable interrupt on positive edge of pin wired to DATA_CLK (GPIO2 as configured in radio_config.h)
	P2IES &= ~DATA_CLK_PIN;
	P2IE |= DATA_CLK_PIN;
	_BIS_SR(GIE);      			// enable interrupts

	// transition radio into receive state
	radio_start_rx(0, 0, 0, RADIO_STATE_NO_CHANGE, RADIO_STATE_NO_CHANGE, RADIO_STATE_NO_CHANGE);
}

// interrupt handler for receiving data via DATA/DATA_CLK
#pragma vector=PORT2_VECTOR
__interrupt void ph_irq_handler(void)
{
	static uint8_t rx_prev_bit = 0;				// previous bit for NRZI decoding
	static uint16_t rx_bitstream = 0;			// last 16 bits processed

	uint8_t rx_this_bit, rx_bit;

	if(P2IFG & DATA_CLK_PIN) {					// verify this interrupt is from DATA_CLK/GPIO_2 pin

		if(P2IN & DATA_PIN)	{					// read bit and decode NRZI
			rx_this_bit = 1;
		} else {
			rx_this_bit = 0;
		}
		rx_bit = !(rx_prev_bit^rx_this_bit); 	// NRZI decoding: change = 0-bit, no change = 1-bit, i.e. 00,11=>1, 01,10=>0, i.e. NOT(A XOR B)
		rx_prev_bit = rx_this_bit;

		rx_bitstream = (rx_bitstream << 1) + rx_bit;
	}
}

void ph_stop(void)
{
	// disable interrupt on pin wired to GPIO2
	P2IE &= ~DATA_CLK_PIN;

	// transition radio from RX to READY
	radio_change_state(RADIO_STATE_READY);
}
