#include "input_transformer.h"
#include "css_styles.h"
#include "gtk/gtk.h"
#include "hexparse.h"

static const char* INVALID_KEY_ERROR = "Invalid key";
static const char* INVALID_IV_ERROR = "Invalid initialization vector";
static const char* INVALID_BOTH_ERROR = "Invalid key and initialization vector";

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

int parse_key_and_iv(MwContext* ctx, unsigned char* key_buff, int key_len, unsigned char* iv_buff, int iv_len, const char** err) {
    GtkEntryBuffer* key_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(ctx->key_input_field));
    GtkEntryBuffer* iv_entry_buffer = gtk_entry_get_buffer(GTK_ENTRY(ctx->iv_input_field));
    const char* key_hex_data =  gtk_entry_buffer_get_text(key_entry_buffer);
    const char* iv_hex_data =  gtk_entry_buffer_get_text(iv_entry_buffer);
    int flags = 0;

    if (parse_hex_data(key_hex_data, key_buff, key_len, 0)) flags |= 1;
    if (parse_hex_data(iv_hex_data, iv_buff, iv_len, 0)) flags |= 2;

    if (flags == 0) {
        *err = INVALID_BOTH_ERROR;
        return 0;
    }
    else if ((flags & 1) != 1) {
        *err = INVALID_KEY_ERROR;
        return 0;
    }
    else if ((flags & 2) != 2) {
        *err = INVALID_IV_ERROR;
        return 0;
    }

    return 1;
}

void transform_input(MwContext* ctx) {
    if (!check_context(ctx)) return;

    int key_len = 128 / 8, iv_len = 128 / 8, block_size = 128 / 8;
    int mode = get_mode(ctx);

    GtkTextBuffer* output_buffer = gtk_text_view_get_buffer(
        GTK_TEXT_VIEW(mode ? ctx->hex_field : ctx->text_field)
    );

    gtk_text_view_set_editable(GTK_TEXT_VIEW(ctx->text_field), mode);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(ctx->hex_field), mode == 0);

    unsigned char key_buff[key_len], iv_buff[iv_len];
    const char* err = 0;

    if (!parse_key_and_iv(ctx, key_buff, key_len, iv_buff, iv_len, &err)) {
        clear_buffer(output_buffer);
        display_error(ctx, err);
        g_warning("%s\n", err);

        return;
    }

    GtkTextBuffer* input_buffer = gtk_text_view_get_buffer(
        GTK_TEXT_VIEW(mode ? ctx->text_field : ctx->hex_field)
    );

    GtkTextIter in_start, in_end;
    gtk_text_buffer_get_start_iter(input_buffer, &in_start);
    gtk_text_buffer_get_end_iter(input_buffer, &in_end);

    const char* in_text = gtk_text_buffer_get_text(
        input_buffer, 
        &in_start, 
        &in_end, 
        TRUE
    );

    int inl = strlen(in_text);

    if (inl == 0) {
        clear_buffer(output_buffer);
        display_error(ctx, "No input data");
        return;
    }

    unsigned char dehex[inl / 2];
    unsigned char* in_raw = mode ? (unsigned char*)in_text : dehex;
    unsigned char out_raw[mode ? (inl + block_size) : (inl / 2)];

    int outl = 0, total_out_len = 0, result = 1;
    int hex_out_len = 0;

    if (!mode) {
        if (!parse_hex_data(in_text, dehex, -1, &hex_out_len)) {
            clear_buffer(output_buffer);
            display_error(ctx, "Invalid hex data");
            g_warning("invalid hex data");
            return;
        }
    }

    result &= EVP_CipherInit(ctx->cipher_ctx, EVP_aes_128_cbc(), key_buff, iv_buff, mode);

    if (result) {
        result &= EVP_CipherUpdate(ctx->cipher_ctx, out_raw, &outl, in_raw, mode ? inl : hex_out_len);
        total_out_len += outl;
    }

    if (result) {
        result &= EVP_CipherFinal(ctx->cipher_ctx, out_raw + outl, &outl);
        total_out_len += outl;
    }

    if (!result) {
        clear_buffer(output_buffer);
        display_error(ctx, mode ? "Encryption error" : "Decryption error");
        g_warning("input transformation error");
        return;
    }

    if (mode) {
        char hex_buffer[total_out_len * 2 + 1];
        write_raw_data_to_hex_buffer(out_raw, total_out_len, hex_buffer);
        gtk_text_buffer_set_text(output_buffer, hex_buffer, total_out_len * 2);
    }
    else {
        gtk_text_buffer_set_text(output_buffer, (char*)out_raw, total_out_len);
    }

    display_error(ctx, 0);
}
