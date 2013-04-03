#include <gtk/gtk.h>
extern char *modeldir;
extern char *hmmdir;

gdouble
train_progress (gint steps) {
	static gdouble counter = 0.0;

	if (counter < 1.0)
		counter += 1.0 / (steps + 1);

	return counter;
}

void
train_go () {
	gint step;
	GtkWidget *window;
	gint steps = 0;
	gchar *hmmfeatparams = g_strconcat (hmmdir, "/feat.params", NULL);
	gchar *hmmmdef = g_strconcat (hmmdir, "/mdef", NULL);
	gchar *hmmvariances = g_strconcat (hmmdir, "/variances", NULL);
	gchar *hmmmeans = g_strconcat (hmmdir, "/means", NULL);
	gchar *hmmmixtureweights = g_strconcat (hmmdir, "/mixture_weights", NULL);
	gchar *hmmtransitionmatrices = g_strconcat (hmmdir, "/transition_matrices",
						NULL);
	gchar* train_programs[][64] = {
		{ "sphinx_fe", "-argfile", hmmfeatparams, "-samprate",
		  "16000", "-c", "arctic20.listoffiles", "-di", ".", "-do",
		  ".", "-ei", "wav", "-eo", "mfc", "-mswav", "yes", NULL },
		{ "pocketsphinx_mdef_convert", "-text", hmmmdef,
		  "./mdef.txt", NULL },
		{ "/usr/local/libexec/sphinxtrain/bw", "-hmmdir", hmmdir,
		  "-moddeffn", "./mdef.txt",
		  "-ts2cbfn", ".semi.", "-feat", "1s_c_d_dd", 
		  "-svspec", "0-12/13-25/26-38",
		  "-cmn", "current", "-agc", "none",
		  "-dictfn", "arctic20.dic", "-ctlfn", "arctic20.listoffiles",
		  "-lsnfn", "arctic20.transcription", "-accumdir", ".", NULL },
		{ "/usr/local/libexec/sphinxtrain/mllr_solve", "-meanfn",
		  hmmmeans, "-varfn", hmmvariances,
		  "-outmllrfn", "mllr_matrix", "-accumdir", ".", NULL },
		{ "/usr/local/libexec/sphinxtrain/map_adapt", "-meanfn",
		  hmmmeans, "-varfn",
		  hmmvariances, "-mixwfn", hmmmixtureweights,
		  "-tmatfn", hmmtransitionmatrices, "-accumdir", ".",
		  "-mapmeanfn", "./means", "-mapvarfn", "./variances",
		  "-mapmixwfn", "./mixture_weights", "-maptmatfn",
		  "./transition_matrices", NULL },
		{ "/usr/local/libexec/sphinxtrain/mk_s2sendump",
		  "-pocketsphinx", "yes", "-moddeffn",
		  "./mdef.txt", "-mixwfn", "./mixture_weights", "-sendumpfn",
		  "./sendump", NULL },
		{ "pocketsphinx_mdef_convert", "-bin", "./mdef.txt", "mdef",
		  NULL },
		{ "mv", "means", "variances", "mixture_weights",
		  "transition_matrices", "sendump", hmmdir, NULL },
		{ NULL },
	};

	while (train_programs[steps][0] != NULL)
		steps++;

	for (step = 0; step < steps; step++) {
		printf ("PROGRESS%f\n", train_progress(steps));
		fflush (stdout);

		g_spawn_sync (NULL, train_programs[step], NULL,
				G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, NULL,
				NULL, NULL);
	}

	g_free (hmmfeatparams);
	g_free (hmmmdef);
	g_free (hmmvariances);
	g_free (hmmmeans);
	g_free (hmmmixtureweights);
	g_free (hmmtransitionmatrices);
}
