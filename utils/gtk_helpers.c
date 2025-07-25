#include "gtk_helpers.h"

void gtk_box_append_n(GtkBox* box, int n, ...) {
    va_list args;
    va_start(args, n);

    for (int i = 0; i < n; i++) {
        gtk_box_append(box, va_arg(args, GtkWidget*));
    }

    va_end(args);
}

void gtk_widget_set_margin(GtkWidget* widget, int margin) {
    gtk_widget_set_margin_top(widget, margin);
    gtk_widget_set_margin_bottom(widget, margin);
    gtk_widget_set_margin_start(widget, margin);
    gtk_widget_set_margin_end(widget, margin);
}

GtkWidget* gtk_label_new_aligned(const char* str, GtkAlign alignment) {
    GtkWidget* label = gtk_label_new(str);
    gtk_widget_set_halign(label, alignment);
    return label;
}