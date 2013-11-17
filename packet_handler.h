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

#endif /* PACKET_HANDLER_H_ */
