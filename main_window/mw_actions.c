#include "mw_actions.h"
#include "input_transformer.h"
#include "gtk/gtk.h"
#include "mw_context.h"
#include "openssl/rand.h"
#include <openssl/evp.h>
#include "utils/hexparse.h"

void gen_buffer_data(GtkEntryBuffer* entry_buffer, size_t len) {
    unsigned char raw_buffer[len];
    char hex_buffer[len * 2 + 1];

    RAND_bytes(raw_buffer, len);
    write_raw_data_to_hex_buffer(raw_buffer, len, hex_buffer);

    gtk_entry_buffer_set_text(entry_buffer, hex_buffer, len * 2);
}

void on_text_field_changed(GtkTextBuffer* buffer, MwContext* ctx) {
    if (!get_mode(ctx)) return;

    transform_input(ctx);
}

void on_hex_field_changed(GtkTextBuffer* buffer, MwContext* ctx) {
    if (get_mode(ctx)) return;

    transform_input(ctx);
}   

void on_key_changed(GtkEntry* entry, MwContext* ctx) {
    transform_input(ctx);
}

void on_iv_changed(GtkEntry* entry, MwContext* ctx) {
    transform_input(ctx);
}

void on_gen_key_button_clicked(GtkButton* button, MwContext* ctx) {
    GtkEntryBuffer* entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(ctx->key_input_field));
    gen_buffer_data(entry_buffer, 128 / 8);
}

void on_gen_iv_button_clicked(GtkButton* button, MwContext* ctx) {
    GtkEntryBuffer* entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(ctx->iv_input_field));
    gen_buffer_data(entry_buffer, 128 / 8);
}

void on_mode_changed(GtkCheckButton* button, MwContext* ctx) {
    transform_input(ctx);
}
