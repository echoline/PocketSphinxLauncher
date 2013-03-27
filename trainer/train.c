#include <gtk/gtk.h>
#include "train.h"

gint
train_count () {
	gint r;

	for (r = 0; train_programs[r] != NULL; r++);

	return r;
}

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
	gint steps = train_count();

	for (step = 0; step < steps; step++) {
		if (!g_spawn_sync (NULL, train_programs[step], NULL,
				G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, NULL,
				NULL, NULL)) {
			window = gtk_message_dialog_new (NULL, 0,
					GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
					"%s has failed to run.",
					train_programs[step][0]);
			gtk_dialog_run (GTK_DIALOG (window));

			break;
		} else {
			printf ("PROGRESS%f\n", train_progress(steps));
		}
	}
}
