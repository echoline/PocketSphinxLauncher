#include <gtk/gtk.h>

extern char *adcdev;
extern char *modeldir;
float progress = 0.0;
GtkWidget *trainwindow = NULL;

void sphinx_gui_config_save ();

void adcdev_changed (GtkEditable *editable, gpointer __unused) {
	if (adcdev)
		g_free(adcdev);
	adcdev = g_strdup(gtk_entry_get_text (GTK_ENTRY (editable)));
	sphinx_gui_config_save();
}

void modeldir_changed (GtkEditable *editable, gpointer __unused) {
	if (modeldir)
		g_free(modeldir);
	modeldir = g_strdup(gtk_entry_get_text (GTK_ENTRY (editable)));
	sphinx_gui_config_save();
}

static gboolean
cb_out_watch( GIOChannel   *channel,
              GIOCondition  cond,
              GtkProgressBar *progress) {
	gchar *string;
	gsize  size;

	if( cond == G_IO_HUP )
	{
		g_io_channel_unref( channel );
		return( FALSE );
	}

	g_io_channel_read_line( channel, &string, &size, NULL, NULL );
	if (g_str_has_prefix (string, "PROGRESS")) {
		gtk_progress_bar_set_fraction (progress,
					g_strtod (&string[8], NULL));
	}

	g_free( string );

	return( TRUE );
}

static gboolean
cb_err_watch( GIOChannel   *channel,
              GIOCondition  cond,
              gpointer __unused) {
	gchar *string;
	gsize  size;

	if( cond == G_IO_HUP )
	{
		g_io_channel_unref( channel );
		return( FALSE );
	}

	g_io_channel_read_line( channel, &string, &size, NULL, NULL );
	fprintf (stderr, "%s", string);
	fflush (stderr);
	g_free( string );

	return( TRUE );
}

static void
cb_child_watch( GPid  pid,
                gint  status,
		gboolean __unused) {
	if (trainwindow != NULL) {
		gtk_widget_destroy (trainwindow);
	}
}

void
train_clicked (GtkButton *button, gpointer __unused) {
	gchar *args[] = { "PSTrainer.sh", adcdev, NULL };
	gint out, err;
	GIOChannel *out_ch, *err_ch;
	GtkWidget *box;
	GtkWidget *progress;
	GPid pid;

	if (g_spawn_async_with_pipes(NULL, args, NULL,
			G_SPAWN_DO_NOT_REAP_CHILD | G_SPAWN_SEARCH_PATH, NULL,
			NULL, &pid, NULL, &out, &err, NULL)) {
        	progress = gtk_progress_bar_new ();
		gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(progress), 0.0);

		out_ch = g_io_channel_unix_new(out);
		err_ch = g_io_channel_unix_new(err);

		g_child_watch_add(pid, (GChildWatchFunc)cb_child_watch, NULL);

    		g_io_add_watch(out_ch, G_IO_IN | G_IO_HUP,
					(GIOFunc)cb_out_watch, progress);
		g_io_add_watch(err_ch, G_IO_IN | G_IO_HUP,
					(GIOFunc)cb_err_watch, NULL);

		trainwindow = gtk_message_dialog_new (NULL, 0, GTK_MESSAGE_INFO,
				GTK_BUTTONS_NONE,
				"PocketSphinx Training...");

		box = gtk_dialog_get_content_area (GTK_DIALOG (trainwindow));
		gtk_box_pack_start (GTK_BOX (box), progress, FALSE, FALSE, 2);
		gtk_widget_show_all (trainwindow);

		gtk_dialog_run(GTK_DIALOG(trainwindow));
	}
}

void sphinx_gui_config_load () {
	GKeyFile *key_file = g_key_file_new();
	gchar *conf = g_strconcat(g_get_user_config_dir(),
				"/pslauncher/config.ini",
				NULL);

	if (g_key_file_load_from_file (key_file, conf, 0, NULL)) {
		if (adcdev != NULL)
			g_free(adcdev);
		if (modeldir != NULL)
			g_free(modeldir);

		adcdev = g_key_file_get_string(key_file, "pslauncher",
						"adcdev", NULL);
		modeldir = g_key_file_get_string(key_file, "pslauncher",
						"modeldir", NULL);
	} else {
		modeldir = g_strdup("./lib/model");
	}

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
	if (modeldir != NULL)
		g_key_file_set_string (key_file, "pslauncher", "modeldir", modeldir);

	data = g_key_file_to_data (key_file, NULL, NULL);
	g_file_set_contents(conf, data, -1, NULL);

	g_key_file_free (key_file);
	g_free(conf);
}

void sphinx_gui_configure(GtkWidget* configbutton, gpointer __unused) {
	GtkWidget *label;
	GtkWidget *entry;
	GtkWidget *button;

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);

	GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
	label = gtk_label_new ("ALSA Device");
	gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 2);
	entry = gtk_entry_new();
	g_signal_connect(entry, "changed", G_CALLBACK (adcdev_changed), NULL);
	if (adcdev != NULL)
		gtk_entry_set_text (GTK_ENTRY (entry), adcdev);
	gtk_box_pack_start (GTK_BOX (hbox), entry, TRUE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX(vbox), hbox, FALSE, TRUE, 2);

	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
	label = gtk_label_new ("Model Directory");
	gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 2);
	entry = gtk_entry_new();
	g_signal_connect(entry, "changed", G_CALLBACK (modeldir_changed), NULL);
	if (modeldir != NULL)
		gtk_entry_set_text (GTK_ENTRY (entry), modeldir);
	gtk_box_pack_start (GTK_BOX (hbox), entry, TRUE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX(vbox), hbox, FALSE, TRUE, 2);

	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
	button = gtk_button_new_with_label ("Train");
	g_signal_connect (button, "clicked", G_CALLBACK(train_clicked), NULL);
	gtk_box_pack_start (GTK_BOX(hbox), button, FALSE, TRUE, 2);
	label = gtk_label_new ("Train acoustic model to your voice");
	gtk_box_pack_start (GTK_BOX(hbox), label, FALSE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX(vbox), hbox, FALSE, TRUE, 2);
	gtk_container_add (GTK_CONTAINER (window), vbox);

	gtk_widget_show_all (window);
}
