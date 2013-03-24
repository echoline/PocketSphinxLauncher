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
#include "fns.h"

GtkWidget *window;
GtkWidget *playbutton;
GtkWidget *stopbutton;
GtkWidget *recordbutton;
char *adcdev = NULL;
char *modeldir = NULL;
char *traindir = NULL;
gchar **lines;
gint lineindex = 0;

void launchlauncher() {
	char *args[] = { "PocketSphinxLauncher", "-adcdev", adcdev, NULL };
	GError *error = NULL;

	if (g_spawn_async(NULL, args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL,
				NULL, &error))
		gtk_main_quit();

	window = gtk_message_dialog_new (NULL, 0, GTK_MESSAGE_ERROR,
			GTK_BUTTONS_NONE,
			"Unable to load launcher: %s", error->message);
	g_error_free(error);
	gtk_dialog_run(GTK_DIALOG(window));

	gtk_main_quit();
}

void lastdestroyed() {
	launchlauncher();
}

void next(GtkButton *button, gpointer __unused) {
	gtk_widget_hide (window);

	if ((lines[lineindex] != NULL) && g_strcmp0(lines[lineindex], "")) {
		doone(lines[lineindex]);
		lineindex++;
	}
	else {
		lastdestroyed();
	}
}

int main(int argc, char *argv[]) {
	GtkWidget *entry;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *button;
	gchar *contents;
	gchar *path;

	if (argc > 1)
		adcdev = argv[1];

	gtk_init (&argc, &argv);

	sphinx_gui_config_load();

	path = g_strconcat(traindir, "/arctic20.txt", NULL);
	if (!g_file_get_contents (path, &contents, NULL, NULL)) {
		g_free(path);

		window = gtk_message_dialog_new (NULL, 0, GTK_MESSAGE_ERROR,
				GTK_BUTTONS_NONE,
				"Error loading training data");
		gtk_dialog_run(GTK_DIALOG(window));

		return -1;
	} else {
		g_free(path);
		lines = g_strsplit(contents, "\n", 0);

		window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
		g_signal_connect(window, "destroy", gtk_main_quit, NULL);

		vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
		hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);

		button = gtk_button_new_from_stock (GTK_STOCK_GO_FORWARD);
		g_signal_connect(button, "clicked", G_CALLBACK(next), NULL);
		gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 2);
		gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 2);

		gtk_container_add (GTK_CONTAINER (window), vbox);
		gtk_widget_show_all (window);
	}
	gtk_main ();

	return 0;
}
