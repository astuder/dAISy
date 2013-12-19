/*
 * Basic FIFO implementation
 * Author: Adrian Studer
 */

#ifndef FIFO_H_
#define FIFO_H_

void fifo_reset(void);					// reset FIFO, all unread data is lost

void fifo_new_packet(void);				// start a new packet, discards any non-committed data
void fifo_write_byte(uint8_t data);		// add next byte to current packet
void fifo_commit_packet(void);			// commit data of current packet, starts a new packet

uint16_t fifo_get_packet(void);			// start reading packet from FIFO, returns size of packet, 0=no packet available
uint8_t fifo_read_byte(void);			// read next byte from current packet
void fifo_remove_packet(void);			// remove packet from FIFO, advance to next slot

#endif /* FIFO_H_ */
