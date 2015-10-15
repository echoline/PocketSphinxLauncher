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
#include "arctic20.h"

GtkWidget *window;
GtkWidget *playbutton;
GtkWidget *stopbutton;
GtkWidget *recordbutton;
//char *adcdev = NULL;
char *modeldir = NULL;
char *hmmdir = NULL;
char *lmdict = NULL;
char *lmdump = NULL;

gchar **lines;
gchar *wavfname = NULL;

gboolean lastdestroyed(gpointer __unused) {
	train_go();
	gtk_main_quit();

	return FALSE;
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
		gtk_widget_hide (window);
		g_idle_add (lastdestroyed, NULL);
	}
}

int main(int argc, char *argv[]) {
	GtkWidget *entry;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *button;
	GError *error;
	gchar *contents;
	gchar *path;

//	if (argc > 1)
//		adcdev = g_strdup(argv[1]);

	fprintf (stderr, "PocketSphinx Copyright (c) 1999-2013 Carnegie Mellon University.\n", argv[0]);
	fprintf (stderr, "%s Copyright (c) 2013 Eli Cohen\n", argv[0]);

	gtk_init (&argc, &argv);
	sphinx_gui_config_load();

	if (g_chdir (modeldir)) {
		fprintf (stderr, "Failed to change directory to %s.", modeldir);

		return -1;
	}

	path = g_strconcat(modeldir, "/arctic20.dic", NULL);
	if (!g_file_test (path, G_FILE_TEST_EXISTS)) {
		g_file_set_contents (path, arctic20_dic, -1, NULL);
	}
	g_free (path);

	path = g_strconcat(modeldir, "/arctic20.listoffiles", NULL);
	if (!g_file_test (path, G_FILE_TEST_EXISTS)) {
		g_file_set_contents (path, arctic20_listoffiles, -1, NULL);
	}
	g_free (path);

	path = g_strconcat(modeldir, "/arctic20.transcription", NULL);
	if (!g_file_test (path, G_FILE_TEST_EXISTS)) {
		g_file_set_contents (path, arctic20_transcription, -1, NULL);
	}
	g_free (path);

	path = g_strconcat(modeldir, "/arctic20.txt", NULL);
	if (!g_file_test (path, G_FILE_TEST_EXISTS)) {
		g_file_set_contents (path, arctic20_txt, -1, NULL);
	}

	if (!g_file_get_contents (path, &contents, NULL, &error)) {
		window = gtk_message_dialog_new (NULL, 0, GTK_MESSAGE_ERROR,
				GTK_BUTTONS_NONE,
				"Error loading training data: %s",
				error->message);
		gtk_dialog_run(GTK_DIALOG(window));
	} else {
		lines = g_strsplit(contents, "\n", 0);

		next (NULL, NULL);
		gtk_main ();
		sphinx_gui_config_save();
	}

	g_free(path);

	return 0;
}
