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

// functions to test packet handler operation, DISCONNECT MODEM BEFORE TESTING!
#ifdef TEST
void test_ph_setup(void);						// setup pins for emulation
void test_ph_send_packet(const char* buffer);	// send AIS packet in buffer (NMEA encoded AIS payload http://gpsd.berlios.de/AIVDM.html#_aivdm_aivdo_payload_armoring)
#endif

#endif /* PACKET_HANDLER_H_ */
