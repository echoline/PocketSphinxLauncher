#include <gtk/gtk.h>

extern char *adcdev;
extern char *modeldir;
extern char *traindir;
extern char *hmmdir;
extern char *lmdump;
extern char *lmdict;

void sphinx_gui_config_load () {
	GKeyFile *key_file = g_key_file_new();
	gchar *conf = g_strconcat(g_get_user_config_dir(),
				"/pslauncher/config.ini",
				NULL);

	if (g_key_file_load_from_file (key_file, conf, 0, NULL)) {
		if (adcdev != NULL)
			g_free(adcdev);
		adcdev = g_key_file_get_string(key_file, "pslauncher",
						"adcdev", NULL);

		if (traindir != NULL)
			g_free(traindir);
		traindir = g_key_file_get_string(key_file, "pslauncher",
						"traindir", NULL);

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

	if (traindir == NULL)
		traindir = g_strdup(TRAINDIR);

	if (hmmdir == NULL)
		hmmdir = g_strdup(HMMDIR);

	if (lmdump == NULL)
		lmdump = g_strdup(LMDUMP);

	if (lmdict == NULL)
		lmdict = g_strdup(LMDICT);

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
		g_key_file_set_string (key_file, "pslauncher", "adcdev", adcdev);
	if (traindir != NULL)
		g_key_file_set_string (key_file, "pslauncher", "traindir", traindir);
	if (modeldir != NULL)
		g_key_file_set_string (key_file, "pslauncher", "modeldir", modeldir);

	if (hmmdir != NULL)
		g_key_file_set_string (key_file, "pslauncher", "hmmdir", hmmdir);

	if (lmdict != NULL)
		g_key_file_set_string (key_file, "pslauncher", "lmdict", lmdict);

	if (lmdump != NULL)
		g_key_file_set_string (key_file, "pslauncher", "lmdump", lmdump);

	data = g_key_file_to_data (key_file, NULL, NULL);
	g_file_set_contents(conf, data, -1, NULL);

	g_key_file_free (key_file);
	g_free(data);
	g_free(conf);
}

