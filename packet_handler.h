/*
 * AIS packet handler for MSP430 + Si4362
 * Author: Adrian Studer
 * License: CC BY-NC-SA Creative Commons Attribution-NonCommercial-ShareAlike
 * 			http://creativecommons.org/licenses/by-nc-sa/4.0/
 * 			Please contact the author if you want to use this work in a commercial product
 */

#ifndef PACKET_HANDLER_H_
#define PACKET_HANDLER_H_

// functions to manage packet handler operation
void ph_setup(void);				// setup packet handler, e.g. configuring input pins
void ph_start(void);				// start receiving packages
void ph_loop(void);					// maintenance of packet handler, e.g. channel hop, call in main loop
void ph_stop(void);					// stop receiving packages

// packet handler states
enum PH_STATE {
	PH_STATE_OFF = 0,
	PH_STATE_RESET,					// reset/restart packet handler
	PH_STATE_WAIT_FOR_PREAMBLE,		// wait for preamble (010101..)
	PH_STATE_WAIT_FOR_START,		// wait for start flag (0x7e)
	PH_STATE_PREFETCH,				// receive first 8 bits of packet
	PH_STATE_RECEIVE_PACKET			// receive packet payload
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
uint8_t ph_get_last_error(void);	// get last packet handler error, will clear error
uint8_t ph_get_radio_channel(void);	// get current radio channel
int16_t ph_get_radio_rssi(void);	// get RSSI in dBm at last sync
uint8_t ph_get_message_type(void);	// get last AIS message type

// functions to test packet handler operation, DISCONNECT MODEM BEFORE TESTING!
#ifdef TEST
void test_ph_setup(void);						// setup pins for emulation
void test_ph_send_packet(const char* message);	// send AIS packet in message (NMEA encoded AIS payload http://gpsd.berlios.de/AIVDM.html#_aivdm_aivdo_payload_armoring)
#endif

#endif /* PACKET_HANDLER_H_ */
