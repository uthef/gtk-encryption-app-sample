#ifndef MAIN_WINDOW_LAYOUT_H
#define MAIN_WINDOW_LAYOUT_H

#include <gtk/gtk.h>
#include <openssl/evp.h>

typedef struct MwContext MwContext;

struct MwContext {
    EVP_CIPHER_CTX* cipher_ctx;

    GtkWidget* root_widget;

    GtkWidget* key_input_field;
    GtkWidget* gen_key_button;

    GtkWidget* iv_input_field;
    GtkWidget* gen_iv_button;

    GtkWidget* enc_mode_check;
    GtkWidget* dec_mode_check;
    
    GtkWidget* text_field;
    GtkWidget* hex_field;

    GtkWidget* error_label;
};

GtkWidget* mw_context_get_root(MwContext*);
void mw_context_init(MwContext*);
void mw_context_clean(MwContext*);

#endif