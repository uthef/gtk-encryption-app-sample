#include "mw_context.h"
#include "css_styles.h"
#include "glib-object.h"
#include "gtk/gtk.h"
#include "gtk/gtkshortcut.h"
#include "mw_actions.h"
#include "gtk_helpers.h"

void mw_context_init_layout(MwContext*);
void mw_context_connect_signals(MwContext*);

GtkWidget* mw_context_get_root(MwContext* ctx) {
    return ctx->root_widget;
}

void mw_context_init(MwContext* ctx) {
    ctx->cipher_ctx = EVP_CIPHER_CTX_new();
    mw_context_init_layout(ctx);
    mw_context_connect_signals(ctx);
}   

void mw_context_clean(MwContext* ctx) {
    EVP_CIPHER_CTX_free(ctx->cipher_ctx);

    ctx->root_widget = 0;
    ctx->key_input_field = 0;
    ctx->gen_key_button = 0;
    ctx->iv_input_field = 0;
    ctx->gen_iv_button = 0;
    ctx->text_field = 0;
    ctx->hex_field = 0;
    ctx->cipher_ctx = 0;
    ctx->dec_mode_check = 0;
    ctx->enc_mode_check = 0;
    ctx->error_label = 0;
}

void mw_context_init_layout(MwContext* ctx) {
    const int margin = 16;

    ctx->root_widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, margin);
    gtk_widget_set_margin(ctx->root_widget, margin);

    GtkBox* root = GTK_BOX(ctx->root_widget);

    // Key section
    GtkWidget* key_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, margin);
    GtkWidget* key_label = gtk_label_new_aligned("Key hex value", GTK_ALIGN_START);
    ctx->gen_key_button = gtk_button_new_with_label("Generate");
    ctx->key_input_field = gtk_entry_new();
    gtk_box_append_n(GTK_BOX(key_hbox), 3, key_label, ctx->key_input_field, ctx->gen_key_button);
    gtk_widget_set_hexpand(ctx->key_input_field, TRUE);

    // IV section
    GtkWidget* iv_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, margin);
    GtkWidget* iv_label = gtk_label_new_aligned("IV hex value", GTK_ALIGN_START);
    ctx->gen_iv_button = gtk_button_new_with_label("Generate");
    ctx->iv_input_field = gtk_entry_new();
    gtk_box_append_n(GTK_BOX(iv_hbox), 3, iv_label, ctx->iv_input_field, ctx->gen_iv_button);
    gtk_widget_set_hexpand(ctx->iv_input_field, TRUE);

    // Mode section
    GtkWidget* mode_label = gtk_label_new_aligned("Mode", GTK_ALIGN_START);
    GtkWidget* mode_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, margin);
    ctx->enc_mode_check = gtk_check_button_new_with_label("Encryption");
    ctx->dec_mode_check = gtk_check_button_new_with_label("Decryption");
    gtk_check_button_set_group(GTK_CHECK_BUTTON(ctx->dec_mode_check), GTK_CHECK_BUTTON(ctx->enc_mode_check));
    gtk_check_button_set_active(GTK_CHECK_BUTTON(ctx->enc_mode_check), TRUE);
    gtk_box_append_n(GTK_BOX(mode_hbox), 3, mode_label, ctx->enc_mode_check, ctx->dec_mode_check);

    // Text field section
    GtkWidget* text_field_label = gtk_label_new_aligned("Plain text", GTK_ALIGN_START);
    GtkWidget* text_field_window = gtk_scrolled_window_new();
    ctx->text_field = gtk_text_view_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(text_field_window), ctx->text_field);
    gtk_widget_add_css_class(ctx->text_field, CLASS_DEFAULT_TEXT_VIEW);
    gtk_widget_set_size_request(text_field_window, 640, 200);
    gtk_widget_set_vexpand(ctx->text_field, TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(ctx->text_field), GTK_WRAP_WORD_CHAR);

    // Hex field section
    GtkWidget* hex_field_window = gtk_scrolled_window_new();
    GtkWidget* hex_field_label = gtk_label_new_aligned("Hex cipher", GTK_ALIGN_START);
    ctx->hex_field = gtk_text_view_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(hex_field_window), ctx->hex_field);
    gtk_widget_set_size_request(hex_field_window, 640, 200);
    gtk_widget_set_vexpand(ctx->hex_field, TRUE);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(ctx->hex_field), FALSE);
    gtk_widget_add_css_class(ctx->hex_field, CLASS_DEFAULT_TEXT_VIEW);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(ctx->hex_field), GTK_WRAP_WORD_CHAR);

    // Error section
    ctx->error_label = gtk_label_new_aligned("", GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(ctx->error_label, CLASS_ERROR);
    gtk_widget_add_css_class(ctx->error_label, CLASS_INVIS);

    // Composition
    gtk_box_append_n(root, 8, 
        key_hbox, iv_hbox,
        mode_hbox,
        text_field_label, text_field_window,
        hex_field_label, hex_field_window,
        ctx->error_label
    );
}

void mw_context_connect_signals(MwContext* ctx) {
    GtkTextBuffer* text_field_buffer = gtk_text_view_get_buffer(
        GTK_TEXT_VIEW(ctx->text_field)
    );

    GtkTextBuffer* hex_field_buffer = gtk_text_view_get_buffer(
        GTK_TEXT_VIEW(ctx->hex_field)
    );

    g_signal_connect(ctx->key_input_field, "changed", G_CALLBACK(on_key_changed), ctx);
    g_signal_connect(ctx->iv_input_field, "changed", G_CALLBACK(on_iv_changed), ctx);
    
    g_signal_connect(ctx->gen_key_button, "clicked", G_CALLBACK(on_gen_key_button_clicked), ctx);
    g_signal_connect(ctx->gen_iv_button, "clicked", G_CALLBACK(on_gen_iv_button_clicked), ctx);

    g_signal_connect(ctx->enc_mode_check, "toggled", G_CALLBACK(on_mode_changed), ctx);

    g_signal_connect(text_field_buffer, "changed", G_CALLBACK(on_text_field_change), ctx);
    g_signal_connect(hex_field_buffer, "changed", G_CALLBACK(on_hex_field_change), ctx);
}
