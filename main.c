#include <msp430.h> 
#include <inttypes.h>

#include "radio.h"
#include "packet_handler.h"

#define LED1	BIT0

#define LED_ON		P1OUT |= LED1
#define LED_OFF 	P1OUT &= ~LED1
#define LED_TOGGLE	P1OUT ^= LED1

#ifdef TEST
void test_main(void);
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

	while (1) {
		// TODO: control logic
	}
}

#ifdef TEST
// AIS test messages, more samples see http://www.aishub.net/nmea-sample.html
const char* test_message_0 = "14eG;o@034o8sd<L9i:a;WF>062D";
const char* test_message_1 = "13u?etPv2;0n:dDPwUM1U1Cb069D";
const char* test_message_2 = "15MgK45P3@G?fl0E`JbR0OwT0@MS";

void test_main(void)
{
	// setup packet handler
	ph_setup();
	test_ph_setup();

	// start packet receiving
	ph_start();

	while (1) {
		// Send fake AIS messages
		_delay_cycles(1000000);
		test_ph_send_packet(test_message_0);
		if (ph_get_last_error() != PH_ERROR_NONE)
			test_error();

		_delay_cycles(1000000);
		test_ph_send_packet(test_message_1);
		if (ph_get_last_error() != PH_ERROR_NONE)
			test_error();

		_delay_cycles(1000000);
		test_ph_send_packet(test_message_2);
		if (ph_get_last_error() != PH_ERROR_NONE)
			test_error();
	}
}
#endif

void test_error()
{
	while (1) {
		LED_TOGGLE;
		_delay_cycles(8000000);			// blink LED if there was an error
	}
}

// handler for unuxpected interrupts
#pragma vector=ADC10_VECTOR,COMPARATORA_VECTOR,NMI_VECTOR,PORT1_VECTOR,	\
			   TIMER0_A0_VECTOR,TIMER0_A1_VECTOR,TIMER1_A0_VECTOR,TIMER1_A1_VECTOR,		\
			   USCIAB0RX_VECTOR,USCIAB0TX_VECTOR,WDT_VECTOR
__interrupt void ISR_trap(void)
{
	// trap CPU & code execution here with an infinite loop
	while (1);
}
