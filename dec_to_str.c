/*
 * Simple 32bit decimal to string w/o division
 * Author: Adrian Studer
 */

#include <inttypes.h>
#include "dec_to_str.h"

const uint32_t divider[12] = { 10,
							   100,
							   1000,			//K
							   10000,
							   100000,
							   1000000,			//M
							   10000000,
							   100000000,
							   1000000000 };	//B

void udec_to_str(char* buf, uint8_t digits, uint32_t val)
{
	uint8_t i = digits - 1;
	char d;

	do {
		i--;
		d = '0';
		while (val >= divider[i]) {
			d++;
			val -= divider[i];
		}
		*buf++ = d;
	} while(i != 0);

	*buf++ = val + '0';
}

void dec_to_str(char* buf, uint8_t digits, int32_t val)
{
	uint8_t i = digits - 1;
	char d;

	if (val < 0) {
		*buf++ = '-';
		val = -val;
	} else
		*buf++ = ' ';

	do {
		i--;
		d = '0';
		while (val >= divider[i]) {
			d++;
			val -= divider[i];
		}
		*buf++ = d;
	} while(i != 0);

	*buf++ = val + '0';
}

