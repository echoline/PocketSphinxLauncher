// these programs are the sphinxtrain tools
//

gchar* train_programs[][100] = {
	{ "sphinx_fe", "-argfile", "feat.params", "-samprate",
	  "16000", "-c", "arctic20.listoffiles", "-di", ".", "-do", ".", "-ei",
	  "wav", "-eo", "mfc", "-mswav", "yes", NULL },
	{ "pocketsphinx_mdef_convert", "-text", "mdef",
	  "./mdef.txt", NULL },
	{ "bw", "-hmmdir", ".", "-moddeffn", "./mdef.txt",
	  "-ts2cbfn", ".semi.", "-feat", "1s_c_d_dd", "-svspec",
	  "0-12/13-25/26-38", "-cmn", "current", "-agc", "none", "-dictfn",
	  "arctic20.dic", "-ctlfn", "arctic20.listoffiles", "-lsnfn",
	  "arctic20.transcription", "-accumdir", ".", NULL },
	{ "mllr_solve", "-meanfn", "means", "-varfn", "variances",
	  "-outmllrfn", "mllr_matrix", "-accumdir", ".", NULL },
	{ "map_adapt", "-meanfn", "means", "-varfn", "variances",
	  "-mixwfn", "mixture_weights", "-tmatfn", "transition_matrices",
	  "-accumdir", ".", "-mapmeanfn", "./means",
	  "-mapvarfn", "./variances", "-mapmixwfn", "./mixture_weights",
	  "-maptmatfn", "./transition_matrices", NULL },
	{ "mk_s2sendump", "-pocketsphinx", "yes", "-moddeffn", "./mdef.txt",
          "-mixwfn", "./mixture_weights", "-sendumpfn", "./sendump", NULL },
	{ "pocketsphinx_mdef_convert", "-bin", "./mdef.txt", "mdef", NULL },
	NULL,
};

gchar *model_files[] = {
	"means", "variances", "mixture_weights", "transition_matrices",
       	"sendump", NULL,
};
