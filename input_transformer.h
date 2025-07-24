#ifndef INPUT_TRANSFORMER_H
#define INPUT_TRANSFORMER_H

#include "mw_context.h"

void transform_input(MwContext* ctx);
int get_mode(MwContext* ctx);
void clear_buffer(GtkTextBuffer* text_buffer);
void display_error(MwContext* ctx, const char* msg);
int check_context(MwContext* ctx);

#endif