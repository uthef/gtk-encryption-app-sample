#ifndef GTK_HELPERS_H
#define GTK_HELPERS_H

#include <gtk/gtk.h>

void gtk_box_append_n(GtkBox* box, int n, ...);
void gtk_widget_set_margin(GtkWidget* widget, int margin);
GtkWidget* gtk_label_new_aligned(const char* str, GtkAlign alignment);

#endif