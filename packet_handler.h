/*
 * AIS packet handler for MSP430 + Si4362
 * Author: Adrian Studer
 */

#ifndef PACKET_HANDLER_H_
#define PACKET_HANDLER_H_

// functions to manage packet handler operation
void ph_setup(void);							// setup packet handler, e.g. configuring input pins
void ph_start(void);							// start receiving packages
void ph_stop(void);								// stop receiving packages

// packet handler states
enum PH_STATE {
	PH_STATE_OFF = 0,
	PH_STATE_INIT,
	PH_STATE_WAIT_FOR_PREAMBLE,
	PH_STATE_WAIT_FOR_START,
	PH_STATE_RECEIVE_START,
	PH_STATE_RECEIVE_PACKET
};

// packet handler errors
enum PH_ERROR {
	PH_ERROR_NONE = 0,
	PH_ERROR_NOSTART,		// no start flag after preamble
	PH_ERROR_STUFFBIT,		// invalid stuff-bit
	PH_ERROR_NOEND,			// no end flag after more than 1020 bits, message too long
	PH_ERROR_CRC			// CRC error
};

uint8_t ph_get_state(void);			// get current state of packet handler
uint8_t ph_get_last_error(void);	// get last packet handler error

// functions to test packet handler operation, DISCONNECT MODEM BEFORE TESTING!
#ifdef TEST
void test_ph_setup(void);						// setup pins for emulation
void test_ph_send_packet(const char* message);	// send AIS packet in message (NMEA encoded AIS payload http://gpsd.berlios.de/AIVDM.html#_aivdm_aivdo_payload_armoring)
uint8_t test_ph_verify_packet(const char* message);	// verify if packet in FIFO is identical with in NMEA encoded message
#endif

#endif /* PACKET_HANDLER_H_ */
