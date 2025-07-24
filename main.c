#include "glib.h"
#include "gtk/gtkcssprovider.h"
#include "mw_context.h"
#include "css_styles.h"
#include <gtk/gtk.h>

void activate(GtkApplication*, MwContext*);
int main(int, char**);

int main(int argc, char** argv) {
    MwContext ctx;

    GtkApplication* app = gtk_application_new(
        "com.uthef.encryptor_gui", 
        G_APPLICATION_DEFAULT_FLAGS
    );

    g_signal_connect(app, "activate", G_CALLBACK(activate), &ctx);

    int exit_code = g_application_run(G_APPLICATION(app), argc, argv);
    mw_context_clean(&ctx);

    return exit_code;
}

void activate(GtkApplication* app, MwContext* ctx) {
    GtkWidget* main_window = gtk_application_window_new(app);

    GtkCssProvider* css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(css_provider, MAIN_STYLESHEET);

    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(), 
        GTK_STYLE_PROVIDER(css_provider), 
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    mw_context_init(ctx);
    GtkWidget* root_widget = mw_context_get_root(ctx);

    if (!root_widget) {
        g_error("window layout initialization failure");
        return;
    }

    gtk_window_set_child(GTK_WINDOW(main_window), root_widget);
    gtk_window_present(GTK_WINDOW(main_window));
}
