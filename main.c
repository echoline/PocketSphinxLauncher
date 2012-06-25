/* PocketSphinxLauncher
 * Copyright (C) 2012 Eli Cohen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <gtk/gtk.h>
#include "dat.h"
#include "fns.h"

int main(int argc, char *argv[]) {
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *text;
	GtkWidget *button;
	GtkWidget *scrolled;
	sphinx_gui_listen_t listen_stuff;

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (window, "destroy", gtk_main_quit, NULL);
	gtk_widget_set_size_request(window, 640, 240);

	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 3);
	gtk_container_add (GTK_CONTAINER (window), vbox);

	scrolled = gtk_scrolled_window_new(NULL, NULL);
	listen_stuff.list = sphinx_gui_list_new();
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled),
						listen_stuff.list);
	gtk_box_pack_start (GTK_BOX(vbox), scrolled, TRUE, TRUE, 2);

	hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
	button = gtk_button_new_from_stock("gtk-add");
	gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, TRUE, 2);
	g_signal_connect(button, "clicked", G_CALLBACK(sphinx_gui_list_add),
			listen_stuff.list);
	button = gtk_button_new_from_stock("gtk-remove");
	gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, TRUE, 2);
	g_signal_connect(button, "clicked", G_CALLBACK(sphinx_gui_list_remove),
			listen_stuff.list);
	gtk_box_pack_start (GTK_BOX(vbox), hbox, FALSE, TRUE, 2);

	hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
	listen_stuff.status = gtk_image_new_from_stock("gtk-no",
							GTK_ICON_SIZE_MENU);
	gtk_box_pack_start (GTK_BOX(hbox), listen_stuff.status, FALSE, TRUE, 2);
	listen_stuff.label = gtk_label_new("");
	gtk_box_pack_start (GTK_BOX(hbox), listen_stuff.label, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX(vbox), hbox, FALSE, TRUE, 2);

	if (!sphinx_gui_listen(&listen_stuff))
		return -1;

	g_timeout_add(1, &sphinx_gui_listen_timeout, &listen_stuff);

	gtk_widget_show_all (window);
	gtk_main ();

	return 0;
}
