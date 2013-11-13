#include <msp430.h> 
#include <inttypes.h>

#include "radio.h"

#define LED1	BIT0

#define LED_ON		P1OUT |= LED1
#define LED_OFF 	P1OUT &= ~LED1
#define LED_TOGGLE	P1OUT ^= LED1

int main(void)
{
	// configure WDT
	WDTCTL = WDTPW | WDTHOLD;				// stop watch dog timer

	// set clock to 16MHz
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;
	BCSCTL2 = 0;							// MCLK and SMCLK = DCO = 16MHz

	// configure LED1 and turn it off, we'll use that for error and other stuff
	P1DIR |= LED1;
	LED_OFF;

	// configure radio
	radio_setup();
	radio_configure();

	// verify that configuration was successful
	radio_get_chip_status(0);
	if (radio_buffer.chip_status.chip_status & RADIO_CMD_ERROR) {	// check for command error
		while (1) {
			LED_TOGGLE;
			_delay_cycles(8000000);			// blink LED if there was an error
		}
	}

	// put radio into receive mode (first parameter = channel)
	radio_start_rx(0, 0, 0, RADIO_STATE_NO_CHANGE, RADIO_STATE_NO_CHANGE, RADIO_STATE_NO_CHANGE);

	while (1) {
		// to do: control logic
	}
}

// handler for unuxpected interrupts
#pragma vector=ADC10_VECTOR,COMPARATORA_VECTOR,NMI_VECTOR,PORT1_VECTOR,PORT2_VECTOR,	\
			   TIMER0_A0_VECTOR,TIMER0_A1_VECTOR,TIMER1_A0_VECTOR,TIMER1_A1_VECTOR,		\
			   USCIAB0RX_VECTOR,USCIAB0TX_VECTOR,WDT_VECTOR
__interrupt void ISR_trap(void)
{
	// Trapping the CPU & code execution here with an infinite loop
	while (1);
}
