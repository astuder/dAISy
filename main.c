#include <msp430.h> 
#include <inttypes.h>

#include "fifo.h"
#include "dec_to_str.h"

#include "radio.h"
#include "packet_handler.h"

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
		uint8_t error = ph_get_last_error();
		if(error == PH_ERROR_NOEND || error == PH_ERROR_STUFFBIT || error == PH_ERROR_CRC)
		{
			LED_TOGGLE;
		}
		uint8_t state = ph_get_state();
		uint16_t size = fifo_get_packet();

		if(size > 0)	// if a new packet arrived
		{
			// TODO: process packet, e.g. create NMEA message
			fifo_remove_packet();
		}
	}
}

#ifdef TEST
// AIS test messages, more samples see http://www.aishub.net/nmea-sample.html
const char test_message_0[] = "133sVfPP00PD>hRMDH@jNOvN20S8";
const char test_message_1[] = "13u?etPv2;0n:dDPwUM1U1Cb069D";
const char test_message_2[] = "100h00PP0@PHFV`Mg5gTH?vNPUIp";


void test_main(void)
{
	// testing dec_to_str
	char test_buf[10];
	const uint32_t a = 1023;
	udec_to_str(test_buf, 9, a);
	const int32_t b = -1023;
	dec_to_str(test_buf, 9, b);

	// setup packet handler
	ph_setup();
	test_ph_setup();

	// start packet receiving
	ph_start();

	while (1) {
		// testing packet handler
		_delay_cycles(1000000);
		test_packet_handler(test_message_0);
		_delay_cycles(1000000);
		test_packet_handler(test_message_1);
		_delay_cycles(1000000);
		test_packet_handler(test_message_2);
	}
}

uint8_t out_buffer[32];		// max message size = 256 bit, typical AIS message is 168+16 bit

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
	if (!test_ph_verify_packet(message))
		test_error();

	// copy packet into buffer for some sanity checks
	packet_size = fifo_get_packet();		// reset FIFO to read packet
	uint16_t i = 0;
	while(i < packet_size)
		out_buffer[i++] = fifo_read_byte();

	uint8_t ais_msg_type = out_buffer[0] >> 2;
	uint32_t ais_mmsi;

	if (ais_msg_type == 1 || ais_msg_type == 2 || ais_msg_type == 3)
		ais_mmsi = (((uint32_t) (out_buffer[1])) << 22 |
					((uint32_t) (out_buffer[2])) << 14 |
					((uint32_t) (out_buffer[3])) << 6 |
							    (out_buffer[4] >> 2));
	else
		ais_mmsi = 0;

	char ais_mmsi_str[9];
	udec_to_str(ais_mmsi_str, 9, ais_mmsi);

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
