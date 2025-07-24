#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "mw_context.h"
#include <gtk/gtk.h>

void on_text_field_change(GtkTextBuffer* buffer, MwContext* ctx);
void on_hex_field_change(GtkTextBuffer* buffer, MwContext* ctx);
void on_key_changed(GtkEntry* entry, MwContext* ctx);
void on_iv_changed(GtkEntry* entry, MwContext* ctx);
void on_gen_key_button_clicked(GtkButton* button, MwContext* ctx);
void on_gen_iv_button_clicked(GtkButton* button, MwContext* ctx);
void on_mode_changed(GtkCheckButton* button, MwContext* ctx);

#endif
