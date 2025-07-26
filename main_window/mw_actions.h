#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "mw_context.h"
#include <gtk/gtk.h>

int get_mode(MwContext* ctx);
void clear_buffer(GtkTextBuffer* text_buffer);
void display_error(MwContext* ctx, const char* msg);
int check_context(MwContext* ctx);
void gen_buffer_data(GtkEntryBuffer* entry_buffer, size_t len);
void perform_input_transformation(MwContext* ctx);

void on_text_field_changed(GtkTextBuffer* buffer, MwContext* ctx);
void on_hex_field_changed(GtkTextBuffer* buffer, MwContext* ctx);
void on_key_changed(GtkEntry* entry, MwContext* ctx);
void on_iv_changed(GtkEntry* entry, MwContext* ctx);
void on_gen_key_button_clicked(GtkButton* button, MwContext* ctx);
void on_gen_iv_button_clicked(GtkButton* button, MwContext* ctx);
void on_mode_changed(GtkCheckButton* button, MwContext* ctx);

#endif
