#ifndef INPUT_TRANSFORMER_H
#define INPUT_TRANSFORMER_H

#include "mw_context.h"

static const char* INVALID_KEY_ERROR = "Invalid key";
static const char* INVALID_IV_ERROR = "Invalid initialization vector";
static const char* INVALID_BOTH_ERROR = "Invalid key and initialization vector";

int parse_key_and_iv(MwContext* ctx, unsigned char* key_buff, int key_len, unsigned char* iv_buff, int iv_len, const char** err);
void transform_input(MwContext* ctx, size_t* in_len, size_t* out_len);

#endif
