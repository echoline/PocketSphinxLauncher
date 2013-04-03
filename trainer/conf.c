#include <gtk/gtk.h>

extern char *adcdev;
extern char *modeldir;
extern char *hmmdir;
extern char *lmdump;
extern char *lmdict;

void sphinx_gui_config_load () {
	gchar *hmm, *lm, *dict, *from;
	GFile *file;
	GKeyFile *key_file = g_key_file_new();
	gchar *copyargs[] = { "cp", "-vr", NULL, NULL, NULL };
	gchar *conf = g_strconcat(g_get_user_config_dir(),
				"/pslauncher/config.ini",
				NULL);
	GtkWidget *window;
	GError *error;
	gchar here[2] = ".";
	gchar *newmodeldir;

	if (g_key_file_load_from_file (key_file, conf, 0, NULL)) {
		if (adcdev != NULL)
			g_free(adcdev);
		adcdev = g_key_file_get_string(key_file, "pslauncher",
						"adcdev", NULL);

		if (modeldir != NULL)
			g_free(modeldir);
		modeldir = g_key_file_get_string(key_file, "pslauncher",
						"modeldir", NULL);

		if (hmmdir != NULL)
			g_free(hmmdir);
		hmmdir = g_key_file_get_string(key_file, "pslauncher",
						"hmmdir", NULL);

		if (lmdump != NULL)
			g_free(lmdump);
		lmdump = g_key_file_get_string(key_file, "pslauncher",
						"lmdump", NULL);

		if (lmdict != NULL)
			g_free(lmdict);
		lmdict = g_key_file_get_string(key_file, "pslauncher",
						"lmdict", NULL);
	}

	if (adcdev == NULL)
		adcdev = g_strdup("default");

	if (modeldir == NULL)
		modeldir = g_strdup(MODELDIR);

	newmodeldir = g_strconcat(g_get_user_config_dir(), "/pslauncher",
		       			"/model", NULL);

	if (hmmdir == NULL)
		hmmdir = g_strdup(HMMDIR);

	from = g_strconcat(newmodeldir, "/sendump", NULL);

	if (!g_file_test (from, G_FILE_TEST_EXISTS)) {
		g_free (from);
		from = g_strdup(modeldir);

		g_mkdir_with_parents (newmodeldir, 0700);

		if (g_chdir (newmodeldir) || g_chdir("..")) {
			window = gtk_message_dialog_new (NULL, 0,
				GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
				"Failed to change directory.");

			gtk_dialog_run (GTK_DIALOG (window));
			gtk_main_quit ();
		}

		copyargs[2] = from;
		copyargs[3] = here;

		if (!g_spawn_sync(NULL, copyargs, NULL, G_SPAWN_SEARCH_PATH,
					NULL, NULL, NULL, NULL, NULL,
					&error)) {
			gtk_widget_hide(window);
			window = gtk_message_dialog_new (NULL, 0,
					GTK_MESSAGE_ERROR,
					GTK_BUTTONS_NONE,
					"Error copying HMM: %s",
				       	error->message);
			g_error_free(error);
			gtk_dialog_run(GTK_DIALOG(window));
			gtk_main_quit();
		}

		g_free (modeldir);

		modeldir = g_strconcat(newmodeldir, NULL);
	}

	g_free (from);
	g_free (newmodeldir);
	g_key_file_free (key_file);
}

void sphinx_gui_config_save () {
	GKeyFile *key_file = g_key_file_new();
	gchar *conf = g_strconcat(g_get_user_config_dir(),
				"/pslauncher",
				NULL);
	gchar *data;

	g_mkdir_with_parents (conf, 0700);
	g_free (conf);

	conf = g_strconcat(g_get_user_config_dir(),
				"/pslauncher/config.ini",
				NULL);

	if (adcdev != NULL)
		g_key_file_set_string (key_file, "pslauncher", "adcdev",
					adcdev);
	
	if (modeldir != NULL)
		g_key_file_set_string (key_file, "pslauncher", "modeldir",
					modeldir);

	if (hmmdir != NULL)
		g_key_file_set_string (key_file, "pslauncher", "hmmdir",
					hmmdir);

	if (lmdict != NULL)
		g_key_file_set_string (key_file, "pslauncher", "lmdict",
					lmdict);

	if (lmdump != NULL)
		g_key_file_set_string (key_file, "pslauncher", "lmdump",
					lmdump);

	data = g_key_file_to_data (key_file, NULL, NULL);
	g_file_set_contents(conf, data, -1, NULL);

	g_key_file_free (key_file);
	g_free(data);
	g_free(conf);
}

