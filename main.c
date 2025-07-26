#include "glib.h"
#include "gtk/gtkcssprovider.h"
#include "main_window/mw_context.h"
#include "design/css_data.h"
#include <gtk/gtk.h>

void on_app_activated(GtkApplication*, MwContext*);
void add_css();
int main(int, char**);

int main(int argc, char** argv) {
    MwContext ctx;
    ctx.timer = 0;
    ctx.cipher_ctx = 0;

    GtkApplication* app = gtk_application_new(
        "com.uthef.gtk-encryption-app-sample", 
        G_APPLICATION_DEFAULT_FLAGS
    );

    g_signal_connect(app, "activate", G_CALLBACK(on_app_activated), &ctx);

    int exit_code = g_application_run(G_APPLICATION(app), argc, argv);
    mw_context_clean(&ctx);
    g_object_unref(app);

    return exit_code;
}

void on_app_activated(GtkApplication* app, MwContext* ctx) {
    mw_context_init(ctx);

    GtkWidget* main_window = gtk_application_window_new(app);
    GtkWidget* root_widget = mw_context_get_root(ctx);

    gtk_window_set_title(GTK_WINDOW(main_window), "GTK Encryption App Sample");

    if (!root_widget) {
        g_error("window layout initialization failure");
        return;
    }

    add_css();

    gtk_window_set_child(GTK_WINDOW(main_window), root_widget);
    gtk_window_present(GTK_WINDOW(main_window));
}

void add_css() {
    GtkCssProvider* css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(css_provider, MAIN_STYLESHEET);

    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(), 
        GTK_STYLE_PROVIDER(css_provider), 
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );
}
