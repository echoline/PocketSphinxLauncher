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
char *hmmdir = NULL;
char *lmdict = NULL;
char *lmdump = NULL;

gchar **lines;
gchar *wavfname = NULL;

void launchlauncher() {
	char *args[] = { "PocketSphinxLauncher", "-adcdev", adcdev, NULL };
	GError *error = NULL;

	if (g_spawn_async(NULL, args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL,
				NULL, &error)) {
		window = gtk_message_dialog_new (NULL, 0, GTK_MESSAGE_ERROR,
				GTK_BUTTONS_NONE,
				"Unable to load launcher: %s", error->message);
		g_error_free(error);
		gtk_dialog_run(GTK_DIALOG(window));

		gtk_main_quit();
	}
}

void lastdestroyed() {
//	launchlauncher();
	gtk_main_quit();
}

void next(GtkButton *button, gpointer __unused) {
	static gint lineindex = 0;
	if (lineindex > 0)
		gtk_widget_hide (window);

	if ((lines[lineindex] != NULL) && g_strcmp0(lines[lineindex], "")) {
		lineindex++;
		if (wavfname != NULL)
			g_free (wavfname);

		wavfname = g_strdup_printf ("%s/arctic_%04d.wav", modeldir, lineindex);
		doone(lines[lineindex-1]);
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
		adcdev = g_strdup(argv[1]);

	fprintf (stderr, "%s Copyright (C) 2013 Eli Cohen\n", argv[0]);

	gtk_init (&argc, &argv);

	sphinx_gui_config_load();

	path = g_strconcat(modeldir, "/arctic20.txt", NULL);
	if (!g_file_get_contents (path, &contents, NULL, NULL)) {
		g_free(path);

		window = gtk_message_dialog_new (NULL, 0, GTK_MESSAGE_ERROR,
				GTK_BUTTONS_NONE,
				"Error loading training data");
		gtk_dialog_run(GTK_DIALOG(window));
	} else {
		g_free(path);
		lines = g_strsplit(contents, "\n", 0);

		next (NULL, NULL);
		gtk_main ();
		sphinx_gui_config_save();
	}

	return 0;
}
