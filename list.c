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

static void list_save(GtkTreeModel *store) {
	gchar *conf = g_strconcat(g_get_user_config_dir(), "/pslauncher.conf",
				NULL);
	gchar *phrase, *command;
	gchar *contents = g_strdup("");
	gchar *tmp;
	GtkTreeIter iter;
	if (gtk_tree_model_get_iter_first(store, &iter)) do {
		gtk_tree_model_get(store, &iter, 0, &phrase, 1, &command, -1);
		tmp = g_strconcat(contents, phrase, "\t", command, "\n", NULL);
		g_free(phrase);
		g_free(command);
		g_free(contents);
		contents = tmp;
	} while (gtk_tree_model_iter_next(store, &iter));

	if (!g_file_set_contents(conf, contents, -1, NULL)) {
		fprintf(stderr, "file write error\n");
	}
	g_free(contents);
	g_free(conf);
}

static void list_load(GtkListStore *store) {
	gchar *conf = g_strconcat(g_get_user_config_dir(), "/pslauncher.conf",
				NULL);
	gchar *contents;
	gchar **lines;
	gchar **tokens;
	guint i = 0, j;
	GtkTreeIter iter;
	if (g_file_get_contents(conf, &contents, NULL, NULL)) {
		lines = g_strsplit(contents, "\n", 0);
		while (lines[i] != NULL) {
			tokens = g_strsplit(lines[i], "\t", 0);
			j = 0;
			while (tokens[j++] != NULL);
			if (j == 3) { // 2 tokens
				gtk_list_store_append(store, &iter);
				gtk_list_store_set(store, &iter, 0, tokens[0],
						1, tokens[1], -1);
			}
			g_strfreev(tokens);
			i++;
		}
		g_strfreev(lines);
	}
	g_free(conf);
}

static void list_edit(GtkCellRendererText *renderer, gchar *path,
			gchar *new_text, gpointer user_data) {
	GtkTreeView *tree = user_data;
	GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(tree));
	gint column = (gint)g_object_get_qdata(G_OBJECT(renderer),
			g_quark_from_static_string("column"));
	GtkTreePath *tree_path = gtk_tree_path_new_from_string(path);
	GtkTreeIter iter;
	gtk_tree_model_get_iter(GTK_TREE_MODEL(store), &iter, tree_path);
	gtk_tree_path_free(tree_path);
	gtk_list_store_set(store, &iter, column, new_text, -1);

	list_save(GTK_TREE_MODEL(store));
}

GtkWidget *sphinx_gui_list_new() {
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeIter iter;
	GtkListStore *store = gtk_list_store_new(2, G_TYPE_STRING,
						G_TYPE_STRING);
	GtkTreeView *list = GTK_TREE_VIEW(gtk_tree_view_new_with_model(
			GTK_TREE_MODEL(store)));
	GtkTreeSelection *sel = gtk_tree_view_get_selection(list);
	gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);

	list_load(store);

	column = gtk_tree_view_column_new();
	renderer = gtk_cell_renderer_text_new();
	g_object_set_qdata_full(G_OBJECT(renderer),
		g_quark_from_static_string("column"), (gpointer)0, NULL);
	g_object_set(G_OBJECT(renderer), "editable", TRUE, NULL);
	g_signal_connect(renderer, "edited", G_CALLBACK(list_edit), list);
	gtk_tree_view_insert_column_with_attributes(list, -1, "Phrase",
						renderer, "text", 0, NULL);

	column = gtk_tree_view_column_new();
	renderer = gtk_cell_renderer_text_new();
	g_object_set_qdata_full(G_OBJECT(renderer),
		g_quark_from_static_string("column"), (gpointer)1, NULL);
	g_object_set(G_OBJECT(renderer), "editable", TRUE, NULL);
	g_signal_connect(renderer, "edited", G_CALLBACK(list_edit), list),
	gtk_tree_view_insert_column_with_attributes(list, -1, "Command",
						renderer, "text", 1, NULL);

	return GTK_WIDGET(list);
}

void sphinx_gui_list_add(GtkWidget *button, gpointer user_data) {
	GtkTreeView *tree = user_data;
	GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(tree));
	GtkTreeIter iter;
	GtkWidget *phrase;
	GtkWidget *command;
	GtkWidget *label;
	GtkWidget *box;

	GtkWidget *dialog = gtk_dialog_new_with_buttons ("Add an action",
		GTK_WINDOW(gtk_widget_get_toplevel(button)),
		GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_STOCK_OK,
		GTK_RESPONSE_ACCEPT,
		GTK_STOCK_CANCEL,
		GTK_RESPONSE_REJECT,
		NULL);

	GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	label = gtk_label_new("Phrase");
	gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 2);
	phrase = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(box), phrase, TRUE, TRUE, 2);
	gtk_container_add(GTK_CONTAINER(content), box);

	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	label = gtk_label_new("Command");
	gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 2);
	command = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(box), command, TRUE, TRUE, 2);
	gtk_container_add(GTK_CONTAINER(content), box);

	gtk_widget_show_all(content);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter,
				0, gtk_entry_get_text(GTK_ENTRY(phrase)),
				1, gtk_entry_get_text(GTK_ENTRY(command)), -1);
	}

	gtk_widget_destroy(dialog);

	list_save(GTK_TREE_MODEL(store));
}

void sphinx_gui_list_remove(GtkWidget *button, gpointer userdata) {
	GtkTreeView *tree = userdata;
	GtkTreeModel *store = gtk_tree_view_get_model(tree);
	GtkTreeIter iter;
	GtkTreeSelection *sel = gtk_tree_view_get_selection(tree);

	if (gtk_tree_selection_get_selected(sel, &store, &iter)) {
		gtk_list_store_remove(GTK_LIST_STORE(store), &iter);
	}

	list_save(store);
}
