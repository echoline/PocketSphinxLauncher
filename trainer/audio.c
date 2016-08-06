#include <gtk/gtk.h>
#include "fns.h"

extern GtkWidget *window;
extern GtkWidget *playbutton;
extern GtkWidget *stopbutton;
extern GtkWidget *recordbutton;
//extern char *adcdev;
extern char *wavfname;

void record(GtkButton *button, gpointer __unused) {
	gchar *args[] = { "arecord", "-r", "16000", "-f",
		"S16_LE", "-t", "wav", wavfname, NULL };
	GError *error = NULL;

	gtk_widget_set_sensitive (recordbutton, FALSE);
	gtk_widget_set_sensitive (stopbutton, TRUE);
	gtk_widget_set_sensitive (playbutton, FALSE);

	if (!g_spawn_async(NULL, args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL,
				NULL, &error)) {
		gtk_widget_hide(window);
		window = gtk_message_dialog_new (NULL, 0, GTK_MESSAGE_ERROR,
				GTK_BUTTONS_NONE,
				"Error recording: %s", error->message);
		g_error_free(error);
		gtk_dialog_run(GTK_DIALOG(window));
		g_signal_connect(window, "destroy", gtk_main_quit, NULL);
	}
}

void stop(GtkButton *button, gpointer arg) {
	gchar *args[] = { "killall", "arecord", "aplay", NULL };
	GError *error = NULL;

	gtk_widget_set_sensitive (recordbutton, TRUE);
	gtk_widget_set_sensitive (stopbutton, FALSE);
	gtk_widget_set_sensitive (playbutton, TRUE);
	gtk_widget_set_sensitive (GTK_WIDGET (arg), TRUE);

	if (!g_spawn_async(NULL, args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL,
				NULL, &error)) {
		gtk_widget_hide(window);
		window = gtk_message_dialog_new (NULL, 0, GTK_MESSAGE_ERROR,
				GTK_BUTTONS_NONE,
				"Error stopping: %s", error->message);
		g_error_free(error);
		gtk_dialog_run(GTK_DIALOG(window));
		g_signal_connect(window, "destroy", gtk_main_quit, NULL);
	}
}

void play(GtkButton *button, gpointer arg) {
	gchar *args[] = { "aplay", wavfname, NULL };
	GError *error = NULL;

	gtk_widget_set_sensitive (recordbutton, FALSE);
	gtk_widget_set_sensitive (stopbutton, TRUE);
	gtk_widget_set_sensitive (playbutton, FALSE);

	if (!g_spawn_async(NULL, args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL,
				NULL, &error)) {
		gtk_widget_hide(window);
		window = gtk_message_dialog_new (NULL, 0, GTK_MESSAGE_ERROR,
				GTK_BUTTONS_NONE,
				"Error playing: %s", error->message);
		g_error_free(error);
		gtk_dialog_run(GTK_DIALOG(window));
		g_signal_connect(window, "destroy", gtk_main_quit, NULL);
	}
}

void doone(char *text) {
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	GtkWidget *vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
	GtkWidget *hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
	GtkWidget *label, *button;
	gchar *str;

	g_signal_connect(window, "destroy", gtk_main_quit, NULL);

        recordbutton = gtk_button_new_from_stock (GTK_STOCK_MEDIA_RECORD);
	g_signal_connect(recordbutton, "clicked", G_CALLBACK(record), NULL);
	gtk_box_pack_start (GTK_BOX (hbox), recordbutton, TRUE, TRUE, 2);

	stopbutton = gtk_button_new_from_stock (GTK_STOCK_MEDIA_STOP);
	gtk_widget_set_sensitive (stopbutton, FALSE);
//	g_signal_connect(stopbutton, "clicked", G_CALLBACK(stop), NULL);
	gtk_box_pack_start (GTK_BOX (hbox), stopbutton, TRUE, TRUE, 2);

	playbutton = gtk_button_new_from_stock (GTK_STOCK_MEDIA_PLAY);
	g_signal_connect(playbutton, "clicked", G_CALLBACK(play), NULL);
	gtk_box_pack_start (GTK_BOX (hbox), playbutton, TRUE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 2);

	hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
	label = gtk_label_new ("Please record yourself reading the following:");
	gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 2);

	label = gtk_label_new (NULL);

	str = g_strconcat("<b>", text, "</b>", NULL);
	gtk_label_set_markup (GTK_LABEL (label), str);
	g_free (str);
	hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
	gtk_box_pack_start (GTK_BOX (hbox), label, TRUE, TRUE, 2);
	gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 2);

	hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
	button = gtk_button_new_from_stock (GTK_STOCK_GO_FORWARD);
	g_signal_connect(stopbutton, "clicked", G_CALLBACK(stop), button);
//	gtk_widget_set_sensitive (button, FALSE);
	g_signal_connect(button, "clicked", G_CALLBACK(next), NULL);
	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, FALSE, 2);

	gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 2);
	gtk_container_add (GTK_CONTAINER (window), vbox);
	gtk_widget_show_all (window);
}

