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

GtkWidget *sphinx_gui_list_new();
void sphinx_gui_list_add(GtkWidget*, gpointer);
void sphinx_gui_list_remove(GtkWidget*, gpointer);
GtkWidget *sphinx_gui_menu_bar_new();
gboolean sphinx_gui_listen(sphinx_gui_listen_t*);
gboolean sphinx_gui_listen_timeout(gpointer);
void* sphinx_gui_listen_main(void*);
void sphinx_gui_configure(GtkWidget*, gpointer);
