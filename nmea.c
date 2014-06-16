/*
 * NMEA library. Encodes raw AIS data from FIFO and sends through UART
 * Author: Adrian Studer
 * License: CC BY-NC-SA Creative Commons Attribution-NonCommercial-ShareAlike
 * 			http://creativecommons.org/licenses/by-nc-sa/4.0/
 * 			Please contact the author if you want to use this work in a commercial product
 */

#include <msp430.h>
#include <inttypes.h>

#include "fifo.h"
#include "uart.h"
#include "nmea.h"

void nmea_push_char(char c);
uint8_t nmea_push_packet(uint8_t packet_size);

#define NMEA_MAX_AIS_PAYLOAD 42		// number of AIS bytes per NMEA sentence, to keep total NMEA sentence always below 82 characters
#define NMEA_AIS_BITS (NMEA_MAX_AIS_PAYLOAD * 8)
#define NMEA_AIS_BITS_ENCODED ((NMEA_AIS_BITS + 5) / 6)

const char nmea_lead[] = "!AIVDM,";		// static start of NMEA sentence
char nmea_buffer[8+NMEA_AIS_BITS_ENCODED+5+1];  // buffer for dynamic part of NMEA sentence
                                                // fragment and channel info, AIS payload, stuff-bit and crc, 0-termination
uint8_t nmea_buffer_index;				// current buffer position

#define NMEA_LEAD_CRC	'A' ^ 'I' ^ 'V' ^ 'D' ^ 'M' ^ ','		// CRC for static start of sentence
uint8_t nmea_crc;						// calculated CRC

uint8_t nmea_message_id = 0;			// sequential message id for multi-sentence message

const char nmea_hex[] = { '0', '1', '2', '3',		// lookup table for hex conversion of CRC
						  '4', '5', '6', '7',
						  '8', '9', 'A', 'B',
						  'C', 'D', 'E', 'F' };

// process AIS next packet in FIFO and transmit as NMEA sentence(s) through UART
void nmea_process_packet(void)
{
	uint16_t packet_size = fifo_get_packet();

	if (packet_size == 0)
		return;									// no packet available in FIFO, nothing to send

	uint8_t radio_channel = fifo_read_byte() + 'A';	// retrieve radio channel (0=A, 1=B)

	// calculate number of fragments, NMEA allows 82 characters per sentence
	//			-> max 62 6-bit characters payload
	//			-> max 46 AIS bytes (368 bits) per sentence
	packet_size -= 3;							// subtract channel and AIS CRC from packet length
	uint8_t curr_fragment = 1;
	uint8_t total_fragments = 1;
	uint16_t packet_bits = packet_size * 8;
	while (packet_bits > NMEA_AIS_BITS) {
		packet_bits -= NMEA_AIS_BITS;
		total_fragments++;
	}

	// maintain message id if this is a multi-sentence message
	if (total_fragments > 1)
	{
		nmea_message_id++;
		if (nmea_message_id > 9)
			nmea_message_id = 1;		// keep message id < 10
	}

	// create fragments
	while (packet_size > 0) {
		// reset buffer and CRC
		nmea_buffer_index = 0;
		nmea_crc = NMEA_LEAD_CRC;

		// write fragment information, I assume total fragments always < 10
		nmea_push_char(total_fragments + '0');
		nmea_push_char(',');
		nmea_push_char(curr_fragment + '0');
		nmea_push_char(',');
		curr_fragment++;

		// write message id if there are multiple fragments
		if (total_fragments > 1)
			nmea_push_char(nmea_message_id + '0');
		nmea_push_char(',');

		// write channel information
		nmea_push_char(radio_channel);
		nmea_push_char(',');

		// encode and write next NMEA_MAX_AIS_PAYLOAD bytes from AIS packet
		uint8_t fragment_size = packet_size;
		if (fragment_size > NMEA_MAX_AIS_PAYLOAD)
		{
			fragment_size = NMEA_MAX_AIS_PAYLOAD;
		}
		uint8_t stuff_bits = nmea_push_packet(fragment_size);
		packet_size -= fragment_size;

		// write stuff bit
		nmea_push_char(',');
		nmea_push_char(stuff_bits + '0');

		// write CRC
		uint8_t final_crc = nmea_crc;		// copy CRC as push_char will modify it
		nmea_push_char('*');
		nmea_push_char(nmea_hex[final_crc >> 4]);
		nmea_push_char(nmea_hex[final_crc & 0x0f]);

		// terminate message with 0
		nmea_push_char(0);

		// send NMEA sentence over UART
		uart_send_string(nmea_lead);
		uart_send_string(nmea_buffer);
		uart_send_string("\r\n");
	}
}

// adds char to buffer and updates CRC
inline void nmea_push_char(char c)
{
	nmea_crc ^= c;
	nmea_buffer[nmea_buffer_index++] = c;
}

// encodes and adds AIS packet to buffer, returns # of stuff bits
uint8_t nmea_push_packet(uint8_t packet_size)
{
	uint8_t raw_byte;
	uint8_t raw_bit;

	uint8_t nmea_byte;
	uint8_t nmea_bit;

	nmea_byte = 0;
	nmea_bit = 6;

	while (packet_size != 0) {
		raw_byte = fifo_read_byte();
		raw_bit = 8;

		while (raw_bit > 0) {
			nmea_byte <<= 1;
			if (raw_byte & 0x80)
				nmea_byte |= 1;
			nmea_bit--;

			if (nmea_bit == 0) {
				if (nmea_byte > 39)
					nmea_byte += 8;
				nmea_byte += 48;
				nmea_push_char(nmea_byte);
				nmea_byte = 0;
				nmea_bit = 6;
			}

			raw_byte <<= 1;
			raw_bit--;
		}

		packet_size--;
	}

	// stuff unfinished NMEA character
	uint8_t stuff_bits = 0;
	if (nmea_bit != 6)
	{
		// stuff with 0 bits as needed
		while (nmea_bit != 0) {
			nmea_byte <<= 1;
			nmea_bit--;
			stuff_bits++;
		}

		// .. and convert and store last byte
		if (nmea_byte > 39)
			nmea_byte += 8;
		nmea_byte += 48;
		nmea_push_char(nmea_byte);
	}

	return stuff_bits;
}

#ifdef TEST

// verify if AIS packet in FIFO is the same as the NMEA payload sent through self-test
uint8_t test_nmea_verify_packet(const char* message)
{
	// read packet from FIFO and encode into NMEA ASCII
	uint8_t packet_size = fifo_get_packet();

	if (!message || packet_size == 0)
		return 0;		// error, no data to verify

	packet_size -= 3;	// ignore CRC and readio channel
	fifo_read_byte();	// discard radio channel

	// encode message into nmea_buffer
	nmea_buffer_index = 0;
	nmea_push_packet(packet_size);

	// compare result with message
	uint8_t i = 0;
	while (i < nmea_buffer_index && message[i]) {
		if (message[i] != nmea_buffer[i])
			return 0;		// error, data not identical
		i++;
	}

	if (i != nmea_buffer_index)
		return 0;			// error, message length not identical

	return 1;	// verification successful, no errors found
}

#endif	// TEST
