/*
 * Simple 32bit decimal to string w/o division
 * Author: Adrian Studer
 */

#ifndef DEC_TO_STR_H_
#define DEC_TO_STR_H_

void udec_to_str(char* buf, uint8_t digits, uint32_t val);
void dec_to_str(char* buf, uint8_t digits, int32_t val);

#endif /* DEC_TO_STR_H_ */
