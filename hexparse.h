#ifndef HEX_PARSE_H
#define HEX_PARSE_H

#include "stddef.h"

void write_raw_data_to_hex_buffer(unsigned char* raw_data, size_t raw_len, char* hex_buffer);
int is_valid_hex_char(char c);
int parse_hex_data(const char* hex_data, unsigned char* out_buff, int exp_len, int* out_len);

#endif