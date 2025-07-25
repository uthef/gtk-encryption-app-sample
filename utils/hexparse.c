#include "hexparse.h"
#include "stdio.h"
#include "stdlib.h"

void write_raw_data_to_hex_buffer(unsigned char* raw_data, size_t raw_len, char* hex_buffer) {
    for (int i = 0; i < raw_len; i++) {
        sprintf((hex_buffer + i * 2), "%02x", raw_data[i]);
    }

    hex_buffer[raw_len * 2] = 0;
}

int is_valid_hex_char(char c) {
    return (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
}

int parse_hex_data(const char* hex_data, unsigned char* raw_buffer, int exp_len, int* out_len) {
    const char* ptr = hex_data;
    int idx = 0;

    while (*ptr) {        
        if (*ptr == ' ') {
            ptr++;
            continue;
        }

        char num_buffer[3] = { 0, 0, 0 };

        if (!is_valid_hex_char(*ptr) || (exp_len != -1 && idx >= exp_len)) return 0;

        num_buffer[0] = *ptr;

        if (!*(++ptr) || !is_valid_hex_char(*ptr)) return 0;

        num_buffer[1] = *ptr;

        raw_buffer[idx++] = strtol(num_buffer, NULL, 16);

        ptr++;
    }

    if (out_len) *out_len = idx;
    if (exp_len != -1 && idx != exp_len) return 0;

    return 1;
}