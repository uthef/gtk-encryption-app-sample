#include "mw_actions.h"
#include "input_transformer.h"
#include "gtk/gtk.h"
#include "mw_context.h"
#include "openssl/rand.h"
#include <openssl/evp.h>
#include "design/css_data.h"
#include "utils/hexparse.h"

int check_context(MwContext* ctx) {
    if (!ctx->cipher_ctx) {
        g_warning("cryptographical context is not initialized");
        return 0;
    }

    return 1;
}

int get_mode(MwContext* ctx) {
    return gtk_check_button_get_active(GTK_CHECK_BUTTON(ctx->enc_mode_check));
}

void display_error(MwContext* ctx, const char* msg) {
    if (!msg) {
        gtk_widget_remove_css_class(ctx->error_label, CLASS_VIS);
        gtk_widget_add_css_class(ctx->error_label, CLASS_INVIS);
        return;
    }

    gtk_label_set_text(GTK_LABEL(ctx->error_label), msg);


    gtk_widget_remove_css_class(ctx->error_label, CLASS_INVIS);
    gtk_widget_add_css_class(ctx->error_label, CLASS_VIS);
}

void clear_buffer(GtkTextBuffer* text_buffer) {
    gtk_text_buffer_set_text(text_buffer, "", 0);
}

void gen_buffer_data(GtkEntryBuffer* entry_buffer, size_t len) {
    unsigned char raw_buffer[len];
    char hex_buffer[len * 2 + 1];

    RAND_bytes(raw_buffer, len);
    write_raw_data_to_hex_buffer(raw_buffer, len, hex_buffer);

    gtk_entry_buffer_set_text(entry_buffer, hex_buffer, len * 2);
}

void perform_input_transformation(MwContext* ctx) {
    GTimer* timer = ctx->timer;
    size_t in_len = 0, out_len = 0;

    g_timer_reset(timer);

    gulong us = 0;
    double ms = 0;

    g_timer_start(timer);

    transform_input(ctx, &in_len, &out_len);

    g_timer_stop(timer);   
    g_timer_elapsed(timer, &us);
    ms = us / 1000.0;

    const char* fmt = "Last operation took %.3lf ms | Bytes in: %zu | Bytes out: % zu";

    int buffer_size = snprintf(0, 0, fmt, ms, in_len, out_len);
    char buffer[buffer_size];

    sprintf(buffer, fmt, ms, in_len, out_len);
    gtk_label_set_text(GTK_LABEL(ctx->status_bar_label), buffer);
}

void on_text_field_changed(GtkTextBuffer* buffer, MwContext* ctx) {
    if (!get_mode(ctx)) return;
    perform_input_transformation(ctx);
}

void on_hex_field_changed(GtkTextBuffer* buffer, MwContext* ctx) {
    if (get_mode(ctx)) return;
    perform_input_transformation(ctx);
}   

void on_key_changed(GtkEntry* entry, MwContext* ctx) {
    perform_input_transformation(ctx);
}

void on_iv_changed(GtkEntry* entry, MwContext* ctx) {
    perform_input_transformation(ctx);
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
    perform_input_transformation(ctx);
}
