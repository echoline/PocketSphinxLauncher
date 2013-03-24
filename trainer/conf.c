#include <gtk/gtk.h>

extern char *adcdev;
extern char *modeldir;
extern char *traindir;

void sphinx_gui_config_save ();

void traindir_changed (GtkEditable *editable, gpointer __unused) {
	if (traindir)
		g_free(traindir);
	traindir = g_strdup(gtk_entry_get_text (GTK_ENTRY (editable)));
	sphinx_gui_config_save();
}

void modeldir_changed (GtkEditable *editable, gpointer __unused) {
	if (modeldir)
		g_free(modeldir);
	modeldir = g_strdup(gtk_entry_get_text (GTK_ENTRY (editable)));
	sphinx_gui_config_save();
}

void sphinx_gui_config_load () {
	GKeyFile *key_file = g_key_file_new();
	gchar *conf = g_strconcat(g_get_user_config_dir(),
				"/pslauncher/config.ini",
				NULL);

	if (g_key_file_load_from_file (key_file, conf, 0, NULL)) {
		if (adcdev != NULL)
			g_free(traindir);
		if (traindir != NULL)
			g_free(traindir);
		if (modeldir != NULL)
			g_free(modeldir);

		adcdev = g_key_file_get_string(key_file, "pstrainer",
						"adcdev", NULL);
		traindir = g_key_file_get_string(key_file, "pstrainer",
						"traindir", NULL);
		modeldir = g_key_file_get_string(key_file, "pstrainer",
						"modeldir", NULL);
	}

	if (adcdev == NULL)
		adcdev = g_strdup("default");

	if (modeldir == NULL)
		modeldir = g_strdup(MODELDIR);

	if (traindir == NULL)
		traindir = g_strdup(TRAINDIR);

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
		g_key_file_set_string (key_file, "pstrainer", "adcdev", adcdev);
	if (traindir != NULL)
		g_key_file_set_string (key_file, "pstrainer", "traindir", traindir);
	if (modeldir != NULL)
		g_key_file_set_string (key_file, "pstrainer", "modeldir", modeldir);

	data = g_key_file_to_data (key_file, NULL, NULL);
	g_file_set_contents(conf, data, -1, NULL);

	g_key_file_free (key_file);
	g_free(data);
	g_free(conf);
}

