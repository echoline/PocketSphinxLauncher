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
#include "dat.h"
#include "fns.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

gboolean sphinx_gui_listen(sphinx_gui_listen_t *listen_stuff) {
	int val;
	pthread_t thread;
	int pfd[2];

	pipe(pfd);
	dup2(pfd[1], 1);
	dup2(pfd[1], 2);

	listen_stuff->fd = pfd[0]; 

	val = fcntl(listen_stuff->fd, F_GETFL, 0); 
	fcntl(listen_stuff->fd, F_SETFL, val | O_NONBLOCK);

	val = pthread_create(&thread, NULL, sphinx_gui_listen_main, NULL);

	printf("listener thread creation returned %d\n", val);

	return TRUE;
}

static gboolean readtonl(gint listenfd, char *buf, size_t buf_sz) {
	size_t offs = 0;

	while (read(listenfd, &buf[offs], 1) == 1) {
		if (buf[offs] == '\n') {
			buf[offs] = '\0';
			return TRUE;
		} else
			offs++;
	}

	if (errno != EAGAIN) {
		close(listenfd);
	}

	return FALSE;
}

static gboolean find_by_phrase(GtkTreeModel *model, GtkTreePath *path,
				GtkTreeIter *iter, gpointer data) {
	char *phrase, *command;
	int argc;
	char **argv;
	gtk_tree_model_get (model, iter, 0, &phrase, -1);

	if (!strcasecmp((char*)data, phrase)) {
		gtk_tree_model_get (model, iter, 1, &command, -1);

		if (!g_shell_parse_argv(command, &argc, &argv, NULL))
			return FALSE;

		if (!g_spawn_async_with_pipes(NULL, argv, NULL,
				G_SPAWN_SEARCH_PATH|G_SPAWN_STDERR_TO_DEV_NULL,
				NULL, NULL, NULL, NULL, NULL, NULL, NULL))
			return FALSE;

		return TRUE;
	}

	return FALSE;
}

gboolean sphinx_gui_listen_timeout(gpointer arg) {
	char buf[1024];
	char *ptr;
	sphinx_gui_listen_t *listen_stuff = arg;
	GtkTreeModel *model;
	GtkTreeIter *iter;

	if (readtonl(listen_stuff->fd, buf, sizeof(buf))) {
		if (!strncasecmp(buf, "READY", 5))
			gtk_image_set_from_stock(
					GTK_IMAGE(listen_stuff->status),
					"gtk-yes", GTK_ICON_SIZE_MENU);
		else if (!strncasecmp(buf, "Listening", 9))
			gtk_image_set_from_stock(
					GTK_IMAGE(listen_stuff->status),
					"gtk-no", GTK_ICON_SIZE_MENU);
		else if (isdigit(buf[0])) {
			ptr = strchr(buf, ' ');
			if (ptr != NULL) {
				ptr++;
				gtk_label_set_text(
					GTK_LABEL(listen_stuff->label), ptr);

				model = gtk_tree_view_get_model(
					GTK_TREE_VIEW(listen_stuff->list));
				gtk_tree_model_foreach(model, find_by_phrase,
							ptr);
			}
		}
	}

	return TRUE;
}
