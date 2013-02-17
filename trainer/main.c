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

char *adcdev = "default";

void destroyed() {
	char *args[] = { "PocketSphinxLauncher", "-adcdev", adcdev, NULL };
	GError *error = NULL;
	GtkWidget *window;

	if (g_spawn_async(NULL, args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL,
				NULL, &error))
		gtk_main_quit();

	window = gtk_message_dialog_new (NULL, 0, GTK_MESSAGE_ERROR,
			GTK_BUTTONS_NONE,
			"Unable to load launcher: %s", error->message);
	g_error_free(error);
	gtk_dialog_run(GTK_DIALOG(window));
}

void record(GtkButton *button, gpointer __unused) {
	gchar *args[] = { "arecord", "-D", adcdev, "-r", "16000", "-f",
		"S16_LE", "-t", "wav", "/tmp/voicedata.wav", NULL };
	GError *error = NULL;
	GtkWidget *window;

	if (!g_spawn_async(NULL, args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL,
				NULL, &error)) {
		window = gtk_message_dialog_new (NULL, 0, GTK_MESSAGE_ERROR,
				GTK_BUTTONS_NONE,
				"Error stopping: %s", error->message);
		g_error_free(error);
		gtk_dialog_run(GTK_DIALOG(window));
	}
}

void stop(GtkButton *button, gpointer __unused) {
	gchar *args[] = { "killall", "arecord", "aplay", NULL };
	GError *error = NULL;
	GtkWidget *window;

	if (!g_spawn_async(NULL, args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL,
				NULL, &error)) {
		window = gtk_message_dialog_new (NULL, 0, GTK_MESSAGE_ERROR,
				GTK_BUTTONS_NONE,
				"Error stopping: %s", error->message);
		g_error_free(error);
		gtk_dialog_run(GTK_DIALOG(window));
	}
}

void play(GtkButton *button, gpointer __unused) {
	gchar *args[] = { "aplay", "/tmp/voicedata.wav", NULL };
	GError *error = NULL;
	GtkWidget *window;

	if (!g_spawn_async(NULL, args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL,
				NULL, &error)) {
		window = gtk_message_dialog_new (NULL, 0, GTK_MESSAGE_ERROR,
				GTK_BUTTONS_NONE,
				"Error stopping: %s", error->message);
		g_error_free(error);
		gtk_dialog_run(GTK_DIALOG(window));
	}
}

void doone(char *text) {
	GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	GtkWidget *vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	GtkWidget *hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);

	GtkWidget *label = gtk_label_new (text);
	gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 2);

	hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
	GtkWidget *button = gtk_button_new_from_stock (GTK_STOCK_MEDIA_RECORD);
	g_signal_connect(button, "clicked", G_CALLBACK(record), NULL);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 2);

	button = gtk_button_new_from_stock (GTK_STOCK_MEDIA_STOP);
	g_signal_connect(button, "clicked", G_CALLBACK(stop), NULL);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 2);

	button = gtk_button_new_from_stock (GTK_STOCK_MEDIA_PLAY);
	g_signal_connect(button, "clicked", G_CALLBACK(play), NULL);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 2);

	gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 2);
	gtk_container_add (GTK_CONTAINER (window), vbox);
	gtk_widget_show_all (window);
}

int main(int argc, char *argv[]) {
	GtkWidget *window;

	if (argc > 1)
		adcdev = argv[1];

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (window, "destroy", destroyed, NULL);

	gtk_widget_show_all (window);
	doone("blah blah blah");
	gtk_main ();

	return 0;
}
