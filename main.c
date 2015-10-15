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

//char *adcdev = NULL;
char *modeldir = NULL;
char *hmmdir = NULL;
char *lmdump = NULL;
char *lmdict = NULL;

void sphinx_gui_visibility(GtkStatusIcon *status_icon, gpointer user_data) {
	sphinx_gui_listen_t *listen_stuff = (sphinx_gui_listen_t*)user_data;

	listen_stuff->visible = !listen_stuff->visible;

	gtk_widget_set_visible(listen_stuff->window, listen_stuff->visible);
}

int main(int argc, char *argv[]) {
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *text;
	GtkWidget *button;
	GtkWidget *scrolled;
	sphinx_gui_listen_t listen_stuff;
	int i;

	fprintf (stderr, "PocketSphinx Copyright (c) 1999-2013 Carnegie Mellon University.\n", argv[0]);
	fprintf (stderr, "%s Copyright (c) 2013 Eli Cohen\n", argv[0]);

//	for (i = 0; i < argc; i++) {
//		if (!g_strcmp0(argv[i], "-adcdev")) {
//			i++;

//			if (i < argc)
//				adcdev = g_strdup(argv[i]);
//		}
//	}

	gtk_init (&argc, &argv);

	sphinx_gui_config_load();
	sphinx_gui_config_save();

	listen_stuff.window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (listen_stuff.window, "destroy", gtk_main_quit, NULL);
	gtk_widget_set_size_request(listen_stuff.window, 640, 240);

	vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	gtk_container_set_border_width (GTK_CONTAINER (vbox), 3);
	gtk_container_add (GTK_CONTAINER (listen_stuff.window), vbox);

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

	listen_stuff.status = gtk_image_new_from_stock("gtk-no",
							GTK_ICON_SIZE_MENU);
	gtk_box_pack_start (GTK_BOX(hbox), listen_stuff.status, FALSE, TRUE, 2);
	listen_stuff.label = gtk_label_new("");
	gtk_box_pack_start (GTK_BOX(hbox), listen_stuff.label, TRUE, TRUE, 2);

	button = gtk_button_new_with_label("Configure");
	gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, TRUE, 2);
	g_signal_connect(button, "clicked", G_CALLBACK(sphinx_gui_configure),
							&listen_stuff);
	gtk_box_pack_start (GTK_BOX(vbox), hbox, FALSE, TRUE, 2);

	listen_stuff.tray = gtk_status_icon_new_from_stock("gtk-no");
	g_signal_connect(listen_stuff.tray, "activate",
			G_CALLBACK(sphinx_gui_visibility), &listen_stuff);

	if (!sphinx_gui_listen(&listen_stuff)) {
		listen_stuff.status = gtk_image_new_from_stock("gtk-no",
							GTK_ICON_SIZE_MENU);
		listen_stuff.tray = gtk_status_icon_new_from_stock("gtk-no");
	}

	g_timeout_add(1, &sphinx_gui_listen_timeout, &listen_stuff);

	listen_stuff.visible = TRUE;
	gtk_widget_show_all (listen_stuff.window);
	gtk_main ();

	return 0;
}
