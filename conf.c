#include <gtk/gtk.h>

void sphinx_gui_configure(GtkWidget* configbutton, gpointer __unused) {
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);

	GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
	GtkWidget *button = gtk_button_new_with_label ("Train");
	gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, TRUE, 2);

	gtk_box_pack_start (GTK_BOX(vbox), hbox, FALSE, TRUE, 2);
	gtk_container_add (GTK_CONTAINER (window), vbox);

	gtk_widget_show_all (window);
}
