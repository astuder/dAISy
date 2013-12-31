/*
 * Basic FIFO packet buffer
  * Author: Adrian Studer
 */

#include <inttypes.h>
#include "fifo.h"

#define FIFO_BUFFER_SIZE		128					// size of FIFO in bytes (must be 2^x)
#define FIFO_PACKETS			8					// max number of individual packets in FIFO (must be 2^x, should be approx. FIFO_SIZE/avg message size)

#if (FIFO_SIZE > 256)								// determine smallest data type required to hold FIFO pointers
#define FIFO_PTR_TYPE	uint16_t					// 16 bit for FIFO larger than 256 bytes
#else
#define FIFO_PTR_TYPE	uint8_t						// 8 bit for FIFO smaller than 256 bytes
#endif

#define FIFO_BUFFER_MASK (FIFO_BUFFER_SIZE - 1)		// mask for easy warping of buffer
#define FIFO_PACKET_MASK (FIFO_PACKETS - 1)			// mask for easy warping of packet table

uint8_t fifo_buffer[FIFO_BUFFER_SIZE];				// buffer to hold packet data
FIFO_PTR_TYPE fifo_packets[FIFO_PACKETS];			// table with start offsets of received packets

FIFO_PTR_TYPE fifo_bytes_in;						// counter for bytes written into current packet
FIFO_PTR_TYPE fifo_bytes_out;						// counter for bytes read from current packet
volatile uint8_t fifo_packet_in;					// table index of incoming packet
uint8_t fifo_packet_out;							// table index of outgoing packet

void fifo_reset(void)
{
	// reset FIFO
	fifo_bytes_in = 0;
	fifo_bytes_out = 0;
	fifo_packet_in = 0;
	fifo_packet_out = 0;
	fifo_packets[0] = 0;							// ensure valid entry for first packet
}

void fifo_new_packet(void)
{
	// reset offset to (re)start packet
	fifo_bytes_in = 0;
}

void fifo_write_byte(uint8_t data)
{
	// add byte to the incoming packet
	FIFO_PTR_TYPE position = (fifo_packets[fifo_packet_in] + fifo_bytes_in) & FIFO_BUFFER_MASK;		// calculate position in buffer
	fifo_buffer[position] = data;					// store byte at position
	fifo_bytes_in++;								// increase byte counter
}

void fifo_commit_packet(void)
{
	// complete incoming packet by advancing to next slot in FIFO
	FIFO_PTR_TYPE new_position = (fifo_packets[fifo_packet_in] + fifo_bytes_in) & FIFO_BUFFER_MASK;	// calculate position in buffer for next packet
	fifo_packet_in = (fifo_packet_in + 1) & FIFO_PACKET_MASK;
	fifo_packets[fifo_packet_in] = new_position;	// store new position in packet table
	fifo_bytes_in = 0;								// reset offset to be ready to store data
}

uint16_t fifo_get_packet(void)
{
	// if available, initiate reading from packet from FIFO
	if (fifo_packet_in == fifo_packet_out)			// if no packets are in FIFO
		return 0;									// return 0

	fifo_bytes_out = 0;								// reset read offset within current packet

	// calculate and size of available packet
	FIFO_PTR_TYPE next_packet = (fifo_packet_out + 1) & FIFO_PACKET_MASK;
	return (FIFO_BUFFER_SIZE -	fifo_packets[fifo_packet_out] + fifo_packets[next_packet]) & FIFO_BUFFER_MASK;
}

uint8_t fifo_read_byte(void)
{
	// retrieve byte from current packet
	FIFO_PTR_TYPE position = (fifo_packets[fifo_packet_out] + fifo_bytes_out) & FIFO_BUFFER_MASK;	// calculate current read position
	fifo_bytes_out++;								// increase current read offset
	return fifo_buffer[position];					// return byte from calculated position
}

void fifo_remove_packet(void)
{
	// remove packet from FIFO, advance to next slot
	if(fifo_packet_in != fifo_packet_out)			// but only do so, if there's actually a packet available
		fifo_packet_out = (fifo_packet_out + 1) & FIFO_PACKET_MASK;
}
