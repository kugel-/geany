/*
 *      sidebar.c - this file is part of Geany, a fast and lightweight IDE
 *
 *      Copyright 2005 The Geany contributors
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License along
 *      with this program; if not, write to the Free Software Foundation, Inc.,
 *      51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*
 * Sidebar related code for the Symbol list and Open files GtkTreeViews.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "sidebar.h"

#include "app.h"
#include "callbacks.h" /* FIXME: for ignore_callback */
#include "documentprivate.h"
#include "filetypesprivate.h"
#include "geanyobject.h"
#include "keyfile.h"
#include "navqueue.h"
#include "stash.h"
#include "support.h"
#include "symbols.h"
#include "ui_utils.h"
#include "utils.h"
#include "keybindings.h"

#include <string.h>

#include <gdk/gdkkeysyms.h>


SidebarTreeviews tv = {NULL, NULL, NULL};
/* while typeahead searching, editor should not get focus */
static gboolean may_steal_focus = FALSE;

enum
{
	TREEVIEW_SYMBOL = 0,
	TREEVIEW_OPENFILES
};

enum
{
	OPENFILES_ACTION_REMOVE = 0,
	OPENFILES_ACTION_SAVE,
	OPENFILES_ACTION_RELOAD
};

/* documents tree model columns */
enum
{
	DOCUMENTS_ICON,
	DOCUMENTS_SHORTNAME,	/* dirname for parents, basename for children */
	DOCUMENTS_DOCUMENT,
	DOCUMENTS_COLOR,
	DOCUMENTS_FILENAME		/* full filename */
};

/* how to display sidebar documents */
static enum {
	SHOW_PATHS_NONE = 0,
	SHOW_PATHS_LIST,
	SHOW_PATHS_TREE,

	MAX_SHOW_PATHS
} documents_show_paths;

static struct
{
	GtkWidget *close;
	GtkWidget *save;
	GtkWidget *reload;
	GtkWidget *show_paths[MAX_SHOW_PATHS];
	GtkWidget *find_in_files;
	GtkWidget *expand_all;
	GtkWidget *collapse_all;
}
doc_items;

static GtkTreeStore	*store_openfiles;
static GtkWidget *openfiles_popup_menu;


static GtkWidget *tag_window;	/* scrolled window that holds the symbol list GtkTreeView */

/* callback prototypes */
static void on_openfiles_document_action(GtkMenuItem *menuitem, gpointer user_data);
static gboolean sidebar_button_press_cb(GtkWidget *widget, GdkEventButton *event,
		gpointer user_data);
static gboolean sidebar_key_press_cb(GtkWidget *widget, GdkEventKey *event,
		gpointer user_data);
static void on_list_document_activate(GtkCheckMenuItem *item, gpointer user_data);
static void on_list_symbol_activate(GtkCheckMenuItem *item, gpointer user_data);
static void documents_menu_update(GtkTreeSelection *selection);
static void sidebar_tabs_show_hide(GtkNotebook *notebook, GtkWidget *child,
								   guint page_num, gpointer data);


/* the prepare_* functions are document-related, but I think they fit better here than in document.c */
static void prepare_taglist(GtkWidget *tree, GtkTreeStore *store)
{
	GtkCellRenderer *text_renderer, *icon_renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection *selection;

	text_renderer = gtk_cell_renderer_text_new();
	icon_renderer = gtk_cell_renderer_pixbuf_new();
	column = gtk_tree_view_column_new();

	gtk_tree_view_column_pack_start(column, icon_renderer, FALSE);
  	gtk_tree_view_column_set_attributes(column, icon_renderer, "pixbuf", SYMBOLS_COLUMN_ICON, NULL);
  	g_object_set(icon_renderer, "xalign", 0.0, NULL);

  	gtk_tree_view_column_pack_start(column, text_renderer, TRUE);
  	gtk_tree_view_column_set_attributes(column, text_renderer, "text", SYMBOLS_COLUMN_NAME, NULL);
  	g_object_set(text_renderer, "yalign", 0.5, NULL);
  	gtk_tree_view_column_set_title(column, _("Symbols"));

	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree), FALSE);

	ui_widget_modify_font_from_string(tree, interface_prefs.tagbar_font);

	gtk_tree_view_set_model(GTK_TREE_VIEW(tree), GTK_TREE_MODEL(store));
	g_object_unref(store);

	g_signal_connect(tree, "button-press-event",
		G_CALLBACK(sidebar_button_press_cb), NULL);
	g_signal_connect(tree, "key-press-event",
		G_CALLBACK(sidebar_key_press_cb), NULL);

	gtk_tree_view_set_show_expanders(GTK_TREE_VIEW(tree), interface_prefs.show_symbol_list_expanders);
	if (! interface_prefs.show_symbol_list_expanders)
		gtk_tree_view_set_level_indentation(GTK_TREE_VIEW(tree), 10);
	/* Tooltips */
	ui_tree_view_set_tooltip_text_column(GTK_TREE_VIEW(tree), SYMBOLS_COLUMN_TOOLTIP);

	/* selection handling */
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
	/* callback for changed selection not necessary, will be handled by button-press-event */
}


static gboolean
on_default_tag_tree_button_press_event(GtkWidget *widget, GdkEventButton *event,
		gpointer user_data)
{
	if (event->button == 3)
	{
		gtk_menu_popup(GTK_MENU(tv.popup_taglist), NULL, NULL, NULL, NULL,
			event->button, event->time);
		return TRUE;
	}
	return FALSE;
}


static void create_default_tag_tree(void)
{
	GtkScrolledWindow *scrolled_window = GTK_SCROLLED_WINDOW(tag_window);
	GtkWidget *label;

	/* default_tag_tree is a GtkViewPort with a GtkLabel inside it */
	tv.default_tag_tree = gtk_viewport_new(
		gtk_scrolled_window_get_hadjustment(scrolled_window),
		gtk_scrolled_window_get_vadjustment(scrolled_window));
	gtk_viewport_set_shadow_type(GTK_VIEWPORT(tv.default_tag_tree), GTK_SHADOW_NONE);
	label = gtk_label_new(_("No symbols found"));
	gtk_misc_set_alignment(GTK_MISC(label), 0.1f, 0.01f);
	gtk_container_add(GTK_CONTAINER(tv.default_tag_tree), label);
	gtk_widget_show_all(tv.default_tag_tree);
	g_signal_connect(tv.default_tag_tree, "button-press-event",
		G_CALLBACK(on_default_tag_tree_button_press_event), NULL);
	g_object_ref((gpointer)tv.default_tag_tree);	/* to hold it after removing */
}


/* update = rescan the tags for doc->filename */
void sidebar_update_tag_list(GeanyDocument *doc, gboolean update)
{
	GtkWidget *child = gtk_bin_get_child(GTK_BIN(tag_window));

	g_return_if_fail(doc == NULL || doc->is_valid);

	if (update)
		doc->priv->tag_tree_dirty = TRUE;

	if (gtk_notebook_get_current_page(GTK_NOTEBOOK(main_widgets.sidebar_notebook)) != TREEVIEW_SYMBOL)
		return; /* don't bother updating symbol tree if we don't see it */

	/* changes the tree view to the given one, trying not to do useless changes */
	#define CHANGE_TREE(new_child) \
		G_STMT_START { \
			/* only change the tag tree if it's actually not the same (to avoid flickering) and if
			 * it's the one of the current document (to avoid problems when e.g. reloading
			 * configuration files */ \
			if (child != new_child && doc == document_get_current()) \
			{ \
				if (child) \
					gtk_container_remove(GTK_CONTAINER(tag_window), child); \
				gtk_container_add(GTK_CONTAINER(tag_window), new_child); \
			} \
		} G_STMT_END

	if (tv.default_tag_tree == NULL)
		create_default_tag_tree();

	/* show default empty tag tree if there are no tags */
	if (doc == NULL || doc->file_type == NULL || ! filetype_has_tags(doc->file_type))
	{
		CHANGE_TREE(tv.default_tag_tree);
		return;
	}

	if (doc->priv->tag_tree_dirty)
	{	/* updating the tag list in the left tag window */
		if (doc->priv->tag_tree == NULL)
		{
			doc->priv->tag_store = gtk_tree_store_new(
				SYMBOLS_N_COLUMNS, GDK_TYPE_PIXBUF, G_TYPE_STRING, TM_TYPE_TAG, G_TYPE_STRING);
			doc->priv->tag_tree = gtk_tree_view_new();
			prepare_taglist(doc->priv->tag_tree, doc->priv->tag_store);
			gtk_widget_show(doc->priv->tag_tree);
			g_object_ref((gpointer)doc->priv->tag_tree);	/* to hold it after removing */
		}

		doc->has_tags = symbols_recreate_tag_list(doc, SYMBOLS_SORT_USE_PREVIOUS);
		doc->priv->tag_tree_dirty = FALSE;
	}

	if (doc->has_tags)
	{
		CHANGE_TREE(doc->priv->tag_tree);
	}
	else
	{
		CHANGE_TREE(tv.default_tag_tree);
	}

	#undef CHANGE_TREE
}


/* cleverly sorts documents by their short name */
static gint documents_sort_func(GtkTreeModel *model, GtkTreeIter *iter_a,
								GtkTreeIter *iter_b, gpointer data)
{
	gchar *name_a, *name_b;
	GeanyDocument *doc_a, *doc_b;
	gint cmp;

	gtk_tree_model_get(model, iter_a,
	                   DOCUMENTS_SHORTNAME, &name_a,
	                   DOCUMENTS_DOCUMENT, &doc_a, -1);
	gtk_tree_model_get(model, iter_b,
	                   DOCUMENTS_SHORTNAME, &name_b,
	                   DOCUMENTS_DOCUMENT, &doc_b, -1);
	/* sort dirs after files */
	if (!doc_a && doc_b)
		cmp = 1;
	else if (doc_a && !doc_b)
		cmp = -1;
	else
	{
		gchar *key_a = g_utf8_collate_key_for_filename(name_a, -1);
		gchar *key_b = g_utf8_collate_key_for_filename(name_b, -1);
		cmp = strcmp(key_a, key_b);
		g_free(key_b);
		g_free(key_a);
	}

	g_free(name_b);
	g_free(name_a);

	return cmp;
}


/* does some preparing things to the open files list widget */
static void prepare_openfiles(void)
{
	GtkCellRenderer *icon_renderer;
	GtkCellRenderer *text_renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection *selection;
	GtkTreeSortable *sortable;

	tv.tree_openfiles = ui_lookup_widget(main_widgets.window, "treeview6");

	/* store the icon and the short filename to show, and the index as reference,
	 * the colour (black/red/green) and the full name for the tooltip */
	store_openfiles = gtk_tree_store_new(5, G_TYPE_ICON, G_TYPE_STRING,
		G_TYPE_POINTER, GDK_TYPE_COLOR, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(tv.tree_openfiles), GTK_TREE_MODEL(store_openfiles));

	/* set policy settings for the scolledwindow around the treeview again, because glade
	 * doesn't keep the settings */
	gtk_scrolled_window_set_policy(
		GTK_SCROLLED_WINDOW(ui_lookup_widget(main_widgets.window, "scrolledwindow7")),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	icon_renderer = gtk_cell_renderer_pixbuf_new();
	g_object_set(icon_renderer, "stock-size", GTK_ICON_SIZE_MENU, NULL);
	text_renderer = gtk_cell_renderer_text_new();
	g_object_set(text_renderer, "ellipsize", PANGO_ELLIPSIZE_MIDDLE, NULL);
	column = gtk_tree_view_column_new();
	gtk_tree_view_column_pack_start(column, icon_renderer, FALSE);
	gtk_tree_view_column_set_attributes(column, icon_renderer, "gicon", DOCUMENTS_ICON, NULL);
	gtk_tree_view_column_pack_start(column, text_renderer, TRUE);
	gtk_tree_view_column_set_attributes(column, text_renderer, "text", DOCUMENTS_SHORTNAME,
		"foreground-gdk", DOCUMENTS_COLOR, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tv.tree_openfiles), column);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tv.tree_openfiles), FALSE);

	gtk_tree_view_set_search_column(GTK_TREE_VIEW(tv.tree_openfiles),
		DOCUMENTS_SHORTNAME);

	/* sort opened filenames in the store_openfiles treeview */
	sortable = GTK_TREE_SORTABLE(GTK_TREE_MODEL(store_openfiles));
	gtk_tree_sortable_set_sort_func(sortable, DOCUMENTS_SHORTNAME, documents_sort_func, NULL, NULL);
	gtk_tree_sortable_set_sort_column_id(sortable, DOCUMENTS_SHORTNAME, GTK_SORT_ASCENDING);

	ui_widget_modify_font_from_string(tv.tree_openfiles, interface_prefs.tagbar_font);

	/* tooltips */
	ui_tree_view_set_tooltip_text_column(GTK_TREE_VIEW(tv.tree_openfiles), DOCUMENTS_FILENAME);

	/* selection handling */
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tv.tree_openfiles));
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
	g_object_unref(store_openfiles);

	g_signal_connect(GTK_TREE_VIEW(tv.tree_openfiles), "button-press-event",
		G_CALLBACK(sidebar_button_press_cb), NULL);
	g_signal_connect(GTK_TREE_VIEW(tv.tree_openfiles), "key-press-event",
		G_CALLBACK(sidebar_key_press_cb), NULL);
}


static gboolean utils_filename_has_prefix(const gchar *str, const gchar *prefix)
{
	gchar *head = g_strndup(str, strlen(prefix));
	gboolean ret = utils_filenamecmp(head, prefix) == 0;

	g_free(head);
	return ret;
}


static gchar *get_project_folder(const gchar *path)
{
	gchar *project_base_path;
	gchar *dirname = NULL;
	const gchar *rest;

	/* replace the project base path with the project name */
	project_base_path = project_get_base_path();

	if (project_base_path != NULL)
	{
		gsize len = strlen(project_base_path);

		/* remove trailing separator so we can match base path exactly */
		if (project_base_path[len-1] == G_DIR_SEPARATOR)
			project_base_path[--len] = '\0';

		/* check whether the dir name matches or uses the project base path */
		if (utils_filename_has_prefix(path, project_base_path))
		{
			rest = path + len;
			if (*rest == G_DIR_SEPARATOR || *rest == '\0')
			{
				dirname = g_strdup_printf("%s%s", app->project->name, rest);
			}
		}
		g_free(project_base_path);
	}

	return dirname;
}


static gchar *get_doc_folder(const gchar *path)
{
	gchar *tmp_dirname = g_strdup(path);
	gchar *dirname = get_project_folder(path);
	const gchar *rest;

	if (dirname == NULL)
	{
		const gchar *home_dir = g_get_home_dir();

		dirname = tmp_dirname;
		/* If matches home dir, replace with tilde */
		if (!EMPTY(home_dir) && utils_filename_has_prefix(dirname, home_dir))
		{
			rest = dirname + strlen(home_dir);
			if (*rest == G_DIR_SEPARATOR || *rest == '\0')
			{
				dirname = g_strdup_printf("~%s", rest);
				g_free(tmp_dirname);
			}
		}
	}
	else
		g_free(tmp_dirname);

	return dirname;
}


static void unfold_iter(GtkTreeIter *iter)
{
	GtkTreePath *path;

	path = gtk_tree_model_get_path(GTK_TREE_MODEL(store_openfiles), iter);
	gtk_tree_view_expand_row(GTK_TREE_VIEW(tv.tree_openfiles), path, TRUE);
	gtk_tree_path_free(path);
}

static gchar *parent_dir_name(GtkTreeIter *parent, gchar *path)
{
	gsize parent_len = 0;
	gchar *dirname;
	gchar *pathname = NULL;

	if (parent)
	{
		gchar *parent_dir;
		GtkTreeModel *model = GTK_TREE_MODEL(store_openfiles);

		gtk_tree_model_get(model, parent, DOCUMENTS_FILENAME, &parent_dir, -1);
		if (parent_dir)
		{
			pathname = get_doc_folder(parent_dir);
			parent_len = strlen(pathname) + 1;
			g_free(parent_dir);
		}
	}

	dirname = get_doc_folder(path);
	if (parent_len)
	{
		gsize len;
		dirname = get_doc_folder(path);
		len = strlen(dirname);
		/* Maybe parent is /home but dirname is ~ (after substitution from /home/user) */
		if (pathname[0] == dirname[0])
			memmove(dirname, dirname + parent_len, len - parent_len + 1);
	}

	g_free(pathname);

	return dirname;
}

static void tree_copy_item(GtkTreeIter *parent, GtkTreeIter *parent_old, GtkTreeIter *parent_new)
{
	GIcon *icon;
	gchar *filename;
	gchar *shortname;
	GdkColor *color;
	GeanyDocument *doc;
	GtkTreeModel *model = GTK_TREE_MODEL(store_openfiles);

	gtk_tree_store_append(store_openfiles, parent, parent_new);
	gtk_tree_model_get(model,                parent_old,
	                   DOCUMENTS_ICON,      &icon,
	                   DOCUMENTS_SHORTNAME, &shortname,
	                   DOCUMENTS_DOCUMENT,  &doc,
	                   DOCUMENTS_COLOR,     &color,
	                   DOCUMENTS_FILENAME,  &filename,
	                   -1);
	/* Copy magick private member */
	if (doc)
		doc->priv->iter = *parent;
	else if (parent_new)
	{
		g_free(shortname);
		shortname = parent_dir_name(parent_new, filename);
	}
	gtk_tree_store_set(store_openfiles,     parent,
	                   DOCUMENTS_ICON,      icon,
	                   DOCUMENTS_SHORTNAME, shortname,
	                   DOCUMENTS_DOCUMENT,  doc,
	                   DOCUMENTS_COLOR,     color,
	                   DOCUMENTS_FILENAME,  filename,
	                   -1);
	g_free(filename);
	g_free(shortname);
	if (color)
		gdk_color_free(color);
}


/*
 * Recursively copy all nodes from old parent to new parent
 * */
static void tree_copy_recursive(GtkTreeIter *parent_old, GtkTreeIter *parent_new)
{
	gint i;
	GtkTreeIter child;
	GtkTreeIter parent;
	GtkTreePath *path;
	GtkTreeModel *model = GTK_TREE_MODEL(store_openfiles);

	path = gtk_tree_model_get_path(model, parent_old);
	gtk_tree_path_free(path);
	tree_copy_item(&parent, parent_old, parent_new);
	i = gtk_tree_model_iter_n_children(model, parent_old) - 1;
	while (i >= 0 && gtk_tree_model_iter_nth_child(model, &child, parent_old, i))
	{
		tree_copy_recursive(&child, &parent);
		i--;
	}
}


static void tree_add_new_dir(GtkTreeIter *child, GtkTreeIter *parent, gchar *file)
{
	static GIcon *dir_icon = NULL;
	gchar *dirname = parent_dir_name(parent, file);

	if (!dir_icon)
		dir_icon = ui_get_mime_icon("inode/directory");

	gtk_tree_store_append(store_openfiles, child, parent);
	gtk_tree_store_set(store_openfiles,     child,
	                   DOCUMENTS_ICON,      dir_icon,
	                   DOCUMENTS_FILENAME,  file,
	                   DOCUMENTS_SHORTNAME, dirname,
	                   -1);

	g_free(dirname);
}


/*
 * Returns the position of dir delimiter where paths don't match
 * */
static guint pathcmp(const gchar *s1, const gchar *s2)
{
	guint i = 0;
	gchar *a;
	gchar *b;

	g_return_val_if_fail(s1 != NULL, 0);
	g_return_val_if_fail(s2 != NULL, 0);

#ifdef G_OS_WIN32
	a = utf8_strdown(s1);
	if (NULL == a)
		return 0;
	b = utf8_strdown(s2);
	if (NULL == b)
	{
		g_free(a);
		return 0;
	}
#else
	a = (gchar*)s1;
	b = (gchar*)s2;
#endif

	while (a[i] && b[i] && a[i] == b[i])
		i++;
	if (a[i] == '\0' && b[i] == '\0')
		return i; /* strings are equal: a/b/c == a/b/c */
	if ((a[i] == '\0' && b[i] == G_DIR_SEPARATOR) ||
	    (b[i] == '\0' && a[i] == G_DIR_SEPARATOR))
		return i; /* subdir case: a/b/c == a/b */
	while (i > 0 && (a[i] != G_DIR_SEPARATOR || b[i] != G_DIR_SEPARATOR))
		i--; /* last slash: a/b/boo == a/b/bar */

#ifdef G_OS_WIN32
	g_free(a);
	g_free(b);
#endif

	return i;
}


typedef struct TreeForeachData {
	gchar *needle;
	gsize best_len;
	gsize needle_len;
	GtkTreeIter best_iter;
	enum {
		TREE_CASE_NONE,
		TREE_CASE_EQUALS,
		TREE_CASE_CHLID_OF,
		TREE_CASE_PARENT_OF,
		TREE_CASE_HAVE_SAME_PARENT
	} best_case;
} TreeForeachData;


static gboolean tree_foreach_callback(GtkTreeModel *model, GtkTreePath *path,
					GtkTreeIter *iter, gpointer data_in)
{
	gchar *name;
	gchar *dirname;
	guint diff;
	gsize name_len;
	GeanyDocument *doc;
	TreeForeachData *data = (TreeForeachData*)data_in;

	gtk_tree_model_get(model, iter,
			DOCUMENTS_FILENAME, &name,
			DOCUMENTS_DOCUMENT, &doc, -1);

	if (doc) /* skip documents */
		goto finally;

	dirname = get_doc_folder(name);
	if (dirname)
		SETPTR(name, dirname);

	diff = pathcmp(name, data->needle);
	name_len = strlen(name);

	if (diff == 0)
		goto finally;

	if (data->best_len < diff)
	{
		gint best_case;
		gboolean tree = documents_show_paths == SHOW_PATHS_TREE;

		/* there are four cases */
		/* first case: exact match. File is from already opened dir */
		if (name_len == diff && data->needle_len == name_len)
			best_case = TREE_CASE_EQUALS;
		/* second case: split current dir. File is from deeper level */
		else if (name_len == diff && tree)
			best_case = TREE_CASE_CHLID_OF;
		/* third case: split parent dir. File is from one of existing level */
		else if (data->needle_len == diff && tree)
			best_case = TREE_CASE_PARENT_OF;
		/* fourth case: both dirs have same parent */
		else if (tree)
			best_case = TREE_CASE_HAVE_SAME_PARENT;
		else
			goto finally;
		data->best_len = diff;
		data->best_case = best_case;
		data->best_iter = *iter;
	}
finally:
	g_free(name);
	return FALSE;
}

/* Returns TRUE if parent points to a newly added row,
 * caller might want to expand the relevant rows in the tree view */
static gboolean get_doc_parent(GeanyDocument *doc, GtkTreeIter *parent)
{
	gchar *path;
	GtkTreeIter iter;
	gint name_diff = 0;
	gboolean has_parent;
	GtkTreeModel *model = GTK_TREE_MODEL(store_openfiles);
	TreeForeachData data = {NULL, 0, 0, {0}, TREE_CASE_NONE};

	path = g_path_get_dirname(DOC_FILENAME(doc));

	/* find best opened dir */
	data.needle = get_doc_folder(path);
	data.needle_len = strlen(data.needle);
	name_diff = strlen(path) - data.needle_len;
	gtk_tree_model_foreach(model, tree_foreach_callback, (gpointer)&data);

	switch (data.best_case)
	{
		case TREE_CASE_EQUALS:
		{
			*parent = data.best_iter;
			break;
		}
		case TREE_CASE_CHLID_OF:
		{
			/* This dir is longer than existing so just add child */
			tree_add_new_dir(parent, &data.best_iter, path);
			break;
		}
		case TREE_CASE_PARENT_OF:
		{
			/* More complicated logic. This dir should be a parent
			 * of existing, so reparent existing dir.  */
			has_parent = gtk_tree_model_iter_parent(model, &iter, &data.best_iter);
			tree_add_new_dir(parent, has_parent ? &iter : NULL, path);
			tree_copy_recursive(&data.best_iter, parent);
			gtk_tree_store_remove(store_openfiles, &data.best_iter);
			break;
		}
		case TREE_CASE_HAVE_SAME_PARENT:
		{
			/* Even more complicated logic. Both dirs have same
			 * parent, so create new parent and reparent them */
			GtkTreeIter parent_buf;
			gchar *newpath = g_strndup(path, data.best_len + name_diff);

			has_parent = gtk_tree_model_iter_parent(model, &iter, &data.best_iter);
			tree_add_new_dir(&parent_buf, has_parent ? &iter : NULL, newpath);
			tree_copy_recursive(&data.best_iter, &parent_buf);
			unfold_iter(&parent_buf);
			gtk_tree_store_remove(store_openfiles, &data.best_iter);
			tree_add_new_dir(parent, &parent_buf, path);

			g_free(newpath);
			break;
		}
		default:
		{
			tree_add_new_dir(parent, NULL, path);
			break;
		}
	}
	if (data.needle != path)
		g_free(data.needle);
	g_free(path);

	return data.best_case != TREE_CASE_EQUALS;
}


/* Also sets doc->priv->iter.
 * This is called recursively in sidebar_openfiles_update_all(). */
void sidebar_openfiles_add(GeanyDocument *doc)
{
	GtkTreeIter *iter = &doc->priv->iter;
	gchar *basename;
	const GdkColor *color = document_get_status_color(doc);
	static GIcon *file_icon = NULL;

	if (documents_show_paths != SHOW_PATHS_NONE)
	{
		/* check if new parent */
		GtkTreeIter parent;
		if (get_doc_parent(doc, &parent))
			unfold_iter(&parent);
		gtk_tree_store_append(store_openfiles, iter, &parent);
	}
	else
	{
		gtk_tree_store_append(store_openfiles, iter, NULL);
	}


	if (!file_icon)
		file_icon = ui_get_mime_icon("text/plain");

	basename = g_path_get_basename(DOC_FILENAME(doc));
	gtk_tree_store_set(store_openfiles, iter,
		DOCUMENTS_ICON, (doc->file_type && doc->file_type->icon) ? doc->file_type->icon : file_icon,
		DOCUMENTS_SHORTNAME, basename, DOCUMENTS_DOCUMENT, doc, DOCUMENTS_COLOR, color,
		DOCUMENTS_FILENAME, DOC_FILENAME(doc), -1);
	g_free(basename);
}


static void openfiles_remove(GeanyDocument *doc)
{
	GtkTreeIter *iter = &doc->priv->iter;
	GtkTreeIter parent;
	GtkTreeModel *model = GTK_TREE_MODEL(store_openfiles);

	if (gtk_tree_model_iter_parent(model, &parent, iter) &&
	    gtk_tree_model_iter_n_children(model, &parent) == 1)
	{
		GtkTreeIter buf = parent;

		/* walk on top and close all orphaned parents */
		while (gtk_tree_model_iter_parent(model, &parent, &buf) &&
		       gtk_tree_model_iter_n_children(model, &parent) == 1)
		{
			buf = parent;
		}
		gtk_tree_store_remove(store_openfiles, &buf);
		return;
	}
	gtk_tree_store_remove(store_openfiles, iter);
}


void sidebar_openfiles_update(GeanyDocument *doc)
{
	GtkTreeIter *iter = &doc->priv->iter;
	gchar *fname;

	gtk_tree_model_get(GTK_TREE_MODEL(store_openfiles), iter, DOCUMENTS_FILENAME, &fname, -1);

	if (utils_str_equal(fname, DOC_FILENAME(doc)))
	{
		/* just update color and the icon */
		const GdkColor *color = document_get_status_color(doc);
		GIcon *icon = doc->file_type->icon;

		gtk_tree_store_set(store_openfiles, iter, DOCUMENTS_COLOR, color, -1);
		if (icon)
			gtk_tree_store_set(store_openfiles, iter, DOCUMENTS_ICON, icon, -1);
	}
	else
	{
		/* path has changed, so remove and re-add */
		GtkTreeSelection *treesel;
		gboolean sel;

		treesel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tv.tree_openfiles));
		sel = gtk_tree_selection_iter_is_selected(treesel, &doc->priv->iter);
		openfiles_remove(doc);

		sidebar_openfiles_add(doc);
		if (sel)
			gtk_tree_selection_select_iter(treesel, &doc->priv->iter);
	}
	g_free(fname);
}


void sidebar_openfiles_update_all(void)
{
	guint i;

	gtk_tree_store_clear(store_openfiles);
	foreach_document (i)
	{
		sidebar_openfiles_add(documents[i]);
	}
}


void sidebar_remove_document(GeanyDocument *doc)
{
	openfiles_remove(doc);

	if (GTK_IS_WIDGET(doc->priv->tag_tree))
	{
		gtk_widget_destroy(doc->priv->tag_tree); /* make GTK release its references, if any */
		/* Because it was ref'd in sidebar_update_tag_list, it needs unref'ing */
		g_object_unref(doc->priv->tag_tree);
		doc->priv->tag_tree = NULL;
	}
}


static void on_hide_sidebar(void)
{
	ui_prefs.sidebar_visible = FALSE;
	ui_sidebar_show_hide();
}


static gboolean on_sidebar_display_symbol_list_show(GtkWidget *item)
{
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item),
		interface_prefs.sidebar_symbol_visible);
	return FALSE;
}


static gboolean on_sidebar_display_open_files_show(GtkWidget *item)
{
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item),
		interface_prefs.sidebar_openfiles_visible);
	return FALSE;
}


void sidebar_add_common_menu_items(GtkMenu *menu)
{
	GtkWidget *item;

	item = gtk_separator_menu_item_new();
	gtk_widget_show(item);
	gtk_container_add(GTK_CONTAINER(menu), item);

	item = gtk_check_menu_item_new_with_mnemonic(_("Show S_ymbol List"));
	gtk_container_add(GTK_CONTAINER(menu), item);
#if GTK_CHECK_VERSION(3, 0, 0)
	g_signal_connect(item, "draw", G_CALLBACK(on_sidebar_display_symbol_list_show), NULL);
#else
	g_signal_connect(item, "expose-event",
			G_CALLBACK(on_sidebar_display_symbol_list_show), NULL);
#endif
	gtk_widget_show(item);
	g_signal_connect(item, "activate",
			G_CALLBACK(on_list_symbol_activate), NULL);

	item = gtk_check_menu_item_new_with_mnemonic(_("Show _Document List"));
	gtk_container_add(GTK_CONTAINER(menu), item);
#if GTK_CHECK_VERSION(3, 0, 0)
	g_signal_connect(item, "draw", G_CALLBACK(on_sidebar_display_open_files_show), NULL);
#else
	g_signal_connect(item, "expose-event",
			G_CALLBACK(on_sidebar_display_open_files_show), NULL);
#endif
	gtk_widget_show(item);
	g_signal_connect(item, "activate",
			G_CALLBACK(on_list_document_activate), NULL);

	item = gtk_image_menu_item_new_with_mnemonic(_("H_ide Sidebar"));
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item),
		gtk_image_new_from_stock(GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU));
	gtk_widget_show(item);
	gtk_container_add(GTK_CONTAINER(menu), item);
	g_signal_connect(item, "activate", G_CALLBACK(on_hide_sidebar), NULL);
}


static void on_openfiles_show_paths_activate(GtkCheckMenuItem *item, gpointer user_data)
{
	documents_show_paths = GPOINTER_TO_INT(user_data);
	sidebar_openfiles_update_all();
}


static void on_list_document_activate(GtkCheckMenuItem *item, gpointer user_data)
{
	interface_prefs.sidebar_openfiles_visible = gtk_check_menu_item_get_active(item);
	ui_sidebar_show_hide();
	sidebar_tabs_show_hide(GTK_NOTEBOOK(main_widgets.sidebar_notebook), NULL, 0, NULL);
}


static void on_list_symbol_activate(GtkCheckMenuItem *item, gpointer user_data)
{
	interface_prefs.sidebar_symbol_visible = gtk_check_menu_item_get_active(item);
	ui_sidebar_show_hide();
	sidebar_tabs_show_hide(GTK_NOTEBOOK(main_widgets.sidebar_notebook), NULL, 0, NULL);
}


static void on_find_in_files(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkTreeSelection *treesel;
	GtkTreeIter iter;
	GtkTreeModel *model;
	GeanyDocument *doc;
	gchar *dir;

	treesel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tv.tree_openfiles));
	if (!gtk_tree_selection_get_selected(treesel, &model, &iter))
		return;
	gtk_tree_model_get(model, &iter, DOCUMENTS_DOCUMENT, &doc, -1);

	if (!doc)
	{
		gtk_tree_model_get(model, &iter, DOCUMENTS_FILENAME, &dir, -1);
	}
	else
		dir = g_path_get_dirname(DOC_FILENAME(doc));

	search_show_find_in_files_dialog(dir);
	g_free(dir);
}


static void on_openfiles_expand_collapse(GtkMenuItem *menuitem, gpointer user_data)
{
	gboolean expand = GPOINTER_TO_INT(user_data);

	if (expand)
		gtk_tree_view_expand_all(GTK_TREE_VIEW(tv.tree_openfiles));
	else
		gtk_tree_view_collapse_all(GTK_TREE_VIEW(tv.tree_openfiles));
}


static void create_show_paths_popup_menu(void)
{
	GSList *group = NULL;
	const gchar *items[MAX_SHOW_PATHS] = {
		[SHOW_PATHS_NONE] = _("D_ocuments Only"),
		[SHOW_PATHS_LIST] = _("Show _Paths"),
		[SHOW_PATHS_TREE] = _("Show _Tree")
	};

	for (guint i = 0; i < G_N_ELEMENTS(items); i++)
	{
		GtkWidget *w = gtk_radio_menu_item_new_with_mnemonic(group, items[i]);
		group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(w));
		gtk_widget_show(w);
		gtk_container_add(GTK_CONTAINER(openfiles_popup_menu), w);
		g_signal_connect(w, "activate",
			G_CALLBACK(on_openfiles_show_paths_activate), GINT_TO_POINTER(i));
		doc_items.show_paths[i] = w;
	}
}


static void create_openfiles_popup_menu(void)
{
	GtkWidget *item;

	openfiles_popup_menu = gtk_menu_new();

	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, NULL);
	gtk_widget_show(item);
	gtk_container_add(GTK_CONTAINER(openfiles_popup_menu), item);
	g_signal_connect(item, "activate",
			G_CALLBACK(on_openfiles_document_action), GINT_TO_POINTER(OPENFILES_ACTION_REMOVE));
	doc_items.close = item;

	item = gtk_separator_menu_item_new();
	gtk_widget_show(item);
	gtk_container_add(GTK_CONTAINER(openfiles_popup_menu), item);

	item = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, NULL);
	gtk_widget_show(item);
	gtk_container_add(GTK_CONTAINER(openfiles_popup_menu), item);
	g_signal_connect(item, "activate",
			G_CALLBACK(on_openfiles_document_action), GINT_TO_POINTER(OPENFILES_ACTION_SAVE));
	doc_items.save = item;

	item = gtk_image_menu_item_new_with_mnemonic(_("_Reload"));
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item),
		gtk_image_new_from_stock(GTK_STOCK_REVERT_TO_SAVED, GTK_ICON_SIZE_MENU));
	gtk_widget_show(item);
	gtk_container_add(GTK_CONTAINER(openfiles_popup_menu), item);
	g_signal_connect(item, "activate",
			G_CALLBACK(on_openfiles_document_action), GINT_TO_POINTER(OPENFILES_ACTION_RELOAD));
	doc_items.reload = item;

	item = gtk_separator_menu_item_new();
	gtk_widget_show(item);
	gtk_container_add(GTK_CONTAINER(openfiles_popup_menu), item);

	item = ui_image_menu_item_new(GTK_STOCK_FIND, _("_Find in Files..."));
	gtk_widget_show(item);
	gtk_container_add(GTK_CONTAINER(openfiles_popup_menu), item);
	g_signal_connect(item, "activate", G_CALLBACK(on_find_in_files), NULL);
	doc_items.find_in_files = item;

	item = gtk_separator_menu_item_new();
	gtk_widget_show(item);
	gtk_container_add(GTK_CONTAINER(openfiles_popup_menu), item);

	create_show_paths_popup_menu();

	item = gtk_separator_menu_item_new();
	gtk_widget_show(item);
	gtk_container_add(GTK_CONTAINER(openfiles_popup_menu), item);

	doc_items.expand_all = ui_image_menu_item_new(GTK_STOCK_ADD, _("_Expand All"));
	gtk_widget_show(doc_items.expand_all);
	gtk_container_add(GTK_CONTAINER(openfiles_popup_menu), doc_items.expand_all);
	g_signal_connect(doc_items.expand_all, "activate",
					 G_CALLBACK(on_openfiles_expand_collapse), GINT_TO_POINTER(TRUE));

	doc_items.collapse_all = ui_image_menu_item_new(GTK_STOCK_REMOVE, _("_Collapse All"));
	gtk_widget_show(doc_items.collapse_all);
	gtk_container_add(GTK_CONTAINER(openfiles_popup_menu), doc_items.collapse_all);
	g_signal_connect(doc_items.collapse_all, "activate",
					 G_CALLBACK(on_openfiles_expand_collapse), GINT_TO_POINTER(FALSE));

	sidebar_add_common_menu_items(GTK_MENU(openfiles_popup_menu));
}


static void unfold_parent(GtkTreeIter *iter)
{
	GtkTreeIter parent;

	if (gtk_tree_model_iter_parent(GTK_TREE_MODEL(store_openfiles), &parent, iter))
		unfold_iter(&parent);
}


/* compares the given data with the doc pointer from the selected row of openfiles
 * treeview, in case of a match the row is selected and TRUE is returned
 * (called indirectly from gtk_tree_model_foreach()) */
static gboolean tree_model_find_node(GtkTreeModel *model, GtkTreePath *path,
		GtkTreeIter *iter, gpointer data)
{
	GeanyDocument *doc;

	gtk_tree_model_get(GTK_TREE_MODEL(store_openfiles), iter, DOCUMENTS_DOCUMENT, &doc, -1);

	if (doc == data)
	{
		/* unfolding also prevents a strange bug where the selection gets stuck on the parent
		 * when it is collapsed and then switching documents */
		unfold_parent(iter);
		gtk_tree_view_set_cursor(GTK_TREE_VIEW(tv.tree_openfiles), path, NULL, FALSE);
		return TRUE;
	}
	else return FALSE;
}


void sidebar_select_openfiles_item(GeanyDocument *doc)
{
	gtk_tree_model_foreach(GTK_TREE_MODEL(store_openfiles), tree_model_find_node, doc);
}


/* callbacks */

static void document_action(GeanyDocument *doc, gint action)
{
	if (! DOC_VALID(doc))
		return;

	switch (action)
	{
		case OPENFILES_ACTION_REMOVE:
		{
			document_close(doc);
			break;
		}
		case OPENFILES_ACTION_SAVE:
		{
			document_save_file(doc, FALSE);
			break;
		}
		case OPENFILES_ACTION_RELOAD:
		{
			document_reload_prompt(doc, NULL);
			break;
		}
	}
}


static void on_openfiles_document_action_apply(GtkTreeModel *model, GtkTreeIter iter, gint action)
{
	GeanyDocument *doc;
	gtk_tree_model_get(model, &iter, DOCUMENTS_DOCUMENT, &doc, -1);
	if (doc)
	{
		document_action(doc, action);
	}
	else
	{
		/* parent item selected */
		GtkTreeIter child;
		gint i = gtk_tree_model_iter_n_children(model, &iter) - 1;

		while (i >= 0 && gtk_tree_model_iter_nth_child(model, &child, &iter, i))
		{
			on_openfiles_document_action_apply(model, child, action);
			i--;
		}
	}
}


static void on_openfiles_document_action(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkTreeIter iter;
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tv.tree_openfiles));
	GtkTreeModel *model;
	gint action = GPOINTER_TO_INT(user_data);

	if (gtk_tree_selection_get_selected(selection, &model, &iter))
		on_openfiles_document_action_apply(model, iter, action);
}


static void change_focus_to_editor(GeanyDocument *doc, GtkWidget *source_widget)
{
	if (may_steal_focus)
		document_try_focus(doc, source_widget);
	may_steal_focus = FALSE;
}


static gboolean openfiles_go_to_selection(GtkTreeSelection *selection, guint keyval)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	GeanyDocument *doc = NULL;

	/* use switch_notebook_page to ignore changing the notebook page because it is already done */
	if (gtk_tree_selection_get_selected(selection, &model, &iter) && ! ignore_callback)
	{
		gtk_tree_model_get(model, &iter, DOCUMENTS_DOCUMENT, &doc, -1);
		if (! doc)
			return FALSE;	/* parent */

		/* switch to the doc and grab the focus */
		document_show_tab(doc);
		if (keyval != GDK_space)
			change_focus_to_editor(doc, tv.tree_openfiles);
	}
	return FALSE;
}


static gboolean taglist_go_to_selection(GtkTreeSelection *selection, guint keyval, guint state)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gint line = 0;
	gboolean handled = TRUE;

	if (gtk_tree_selection_get_selected(selection, &model, &iter))
	{
		TMTag *tag;

		gtk_tree_model_get(model, &iter, SYMBOLS_COLUMN_TAG, &tag, -1);
		if (! tag)
			return FALSE;

		line = tag->line;
		if (line > 0)
		{
			GeanyDocument *doc = document_get_current();

			if (doc != NULL)
			{
				navqueue_goto_line(doc, doc, line);
				state = keybindings_get_modifiers(state);
				if (keyval != GDK_space && ! (state & GEANY_PRIMARY_MOD_MASK))
					change_focus_to_editor(doc, NULL);
				else
					handled = FALSE;
			}
		}
		tm_tag_unref(tag);
	}
	return handled;
}


static gboolean sidebar_key_press_cb(GtkWidget *widget, GdkEventKey *event,
											 gpointer user_data)
{
	may_steal_focus = FALSE;
	if (ui_is_keyval_enter_or_return(event->keyval) || event->keyval == GDK_space)
	{
		GtkWidgetClass *widget_class = GTK_WIDGET_GET_CLASS(widget);
		GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
		may_steal_focus = TRUE;

		/* force the TreeView handler to run before us for it to do its job (selection & stuff).
		 * doing so will prevent further handlers to be run in most cases, but the only one is our
		 * own, so guess it's fine. */
		if (widget_class->key_press_event)
			widget_class->key_press_event(widget, event);

		if (widget == tv.tree_openfiles) /* tag and doc list have separate handlers */
			openfiles_go_to_selection(selection, event->keyval);
		else
			taglist_go_to_selection(selection, event->keyval, event->state);

		return TRUE;
	}
	return FALSE;
}


static gboolean sidebar_button_press_cb(GtkWidget *widget, GdkEventButton *event,
		G_GNUC_UNUSED gpointer user_data)
{
	GtkTreeSelection *selection;
	GtkWidgetClass *widget_class = GTK_WIDGET_GET_CLASS(widget);
	gboolean handled = FALSE;

	/* force the TreeView handler to run before us for it to do its job (selection & stuff).
	 * doing so will prevent further handlers to be run in most cases, but the only one is our own,
	 * so guess it's fine. */
	if (widget_class->button_press_event)
		handled = widget_class->button_press_event(widget, event);

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
	may_steal_focus = TRUE;

	if (event->type == GDK_2BUTTON_PRESS)
	{	/* double click on parent node(section) expands/collapses it */
		GtkTreeModel *model;
		GtkTreeIter iter;

		if (gtk_tree_selection_get_selected(selection, &model, &iter))
		{
			if (gtk_tree_model_iter_has_child(model, &iter))
			{
				GtkTreePath *path = gtk_tree_model_get_path(model, &iter);

				if (gtk_tree_view_row_expanded(GTK_TREE_VIEW(widget), path))
					gtk_tree_view_collapse_row(GTK_TREE_VIEW(widget), path);
				else
					gtk_tree_view_expand_row(GTK_TREE_VIEW(widget), path, FALSE);

				gtk_tree_path_free(path);
				return TRUE;
			}
		}
	}
	else if (event->button == 1)
	{	/* allow reclicking of taglist treeview item */
		if (widget == tv.tree_openfiles)
		{
			openfiles_go_to_selection(selection, 0);
			handled = TRUE;
		}
		else
			handled = taglist_go_to_selection(selection, 0, event->state);
	}
	else if (event->button == 2)
	{
		if (widget == tv.tree_openfiles)
			on_openfiles_document_action(NULL, GINT_TO_POINTER(OPENFILES_ACTION_REMOVE));
	}
	else if (event->button == 3)
	{
		if (widget == tv.tree_openfiles)
		{
			if (!openfiles_popup_menu)
				create_openfiles_popup_menu();

			/* update menu item sensitivity */
			documents_menu_update(selection);
			gtk_menu_popup(GTK_MENU(openfiles_popup_menu), NULL, NULL, NULL, NULL,
					event->button, event->time);
		}
		else
		{
			gtk_menu_popup(GTK_MENU(tv.popup_taglist), NULL, NULL, NULL, NULL,
					event->button, event->time);
		}
		handled = TRUE;
	}
	return handled;
}


static void documents_menu_update(GtkTreeSelection *selection)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	gboolean sel, path;
	gchar *shortname = NULL;
	GeanyDocument *doc = NULL;

	/* maybe no selection e.g. if ctrl-click deselected */
	sel = gtk_tree_selection_get_selected(selection, &model, &iter);
	if (sel)
	{
		gtk_tree_model_get(model, &iter, DOCUMENTS_DOCUMENT, &doc,
			DOCUMENTS_SHORTNAME, &shortname, -1);
	}
	path = !EMPTY(shortname) &&
		(g_path_is_absolute(shortname) ||
		(app->project && g_str_has_prefix(shortname, app->project->name)));

	/* can close all, save all (except shortname), but only reload individually ATM */
	gtk_widget_set_sensitive(doc_items.close, sel);
	gtk_widget_set_sensitive(doc_items.save, (doc && doc->real_path) || path);
	gtk_widget_set_sensitive(doc_items.reload, doc && doc->real_path);
	gtk_widget_set_sensitive(doc_items.find_in_files, sel);
	g_free(shortname);

	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(doc_items.show_paths[documents_show_paths]), TRUE);
	gtk_widget_set_sensitive(doc_items.expand_all, documents_show_paths);
	gtk_widget_set_sensitive(doc_items.collapse_all, documents_show_paths);
}


static StashGroup *stash_group = NULL;

static void on_load_settings(void)
{
	tag_window = ui_lookup_widget(main_widgets.window, "scrolledwindow2");

	prepare_openfiles();
	/* note: ui_prefs.sidebar_page is reapplied after plugins are loaded */
	stash_group_display(stash_group, NULL);
	sidebar_tabs_show_hide(GTK_NOTEBOOK(main_widgets.sidebar_notebook), NULL, 0, NULL);
}


static void on_save_settings(void)
{
	stash_group_update(stash_group, NULL);
	sidebar_tabs_show_hide(GTK_NOTEBOOK(main_widgets.sidebar_notebook), NULL, 0, NULL);
}


static void on_sidebar_switch_page(GtkNotebook *notebook,
	gpointer page, guint page_num, gpointer user_data)
{
	if (page_num == TREEVIEW_SYMBOL)
		sidebar_update_tag_list(document_get_current(), FALSE);
}


void sidebar_init(void)
{
	StashGroup *group;

	group = stash_group_new(PACKAGE);
	stash_group_add_integer(group, (gint*)&documents_show_paths, "documents_show_paths", SHOW_PATHS_LIST);
	stash_group_add_widget_property(group, &ui_prefs.sidebar_page, "sidebar_page", GINT_TO_POINTER(0),
		main_widgets.sidebar_notebook, "page", 0);
	configuration_add_pref_group(group, FALSE);
	stash_group = group;

	/* delay building documents treeview until sidebar font has been read */
	g_signal_connect(geany_object, "load-settings", on_load_settings, NULL);
	g_signal_connect(geany_object, "save-settings", on_save_settings, NULL);

	g_signal_connect(main_widgets.sidebar_notebook, "page-added",
		G_CALLBACK(sidebar_tabs_show_hide), NULL);
	g_signal_connect(main_widgets.sidebar_notebook, "page-removed",
		G_CALLBACK(sidebar_tabs_show_hide), NULL);
	/* tabs may have changed when sidebar is reshown */
	g_signal_connect(main_widgets.sidebar_notebook, "show",
		G_CALLBACK(sidebar_tabs_show_hide), NULL);
	g_signal_connect_after(main_widgets.sidebar_notebook, "switch-page",
		G_CALLBACK(on_sidebar_switch_page), NULL);

	sidebar_tabs_show_hide(GTK_NOTEBOOK(main_widgets.sidebar_notebook), NULL, 0, NULL);
}

#define WIDGET(w) w && GTK_IS_WIDGET(w)

void sidebar_finalize(void)
{
	if (WIDGET(tv.default_tag_tree))
	{
		gtk_widget_destroy(tv.default_tag_tree); /* make GTK release its references, if any... */
		g_object_unref(tv.default_tag_tree); /* ...and release our own */
	}
	if (WIDGET(tv.popup_taglist))
		gtk_widget_destroy(tv.popup_taglist);
	if (WIDGET(openfiles_popup_menu))
		gtk_widget_destroy(openfiles_popup_menu);
}


void sidebar_focus_openfiles_tab(void)
{
	if (ui_prefs.sidebar_visible && interface_prefs.sidebar_openfiles_visible)
	{
		GtkNotebook *notebook = GTK_NOTEBOOK(main_widgets.sidebar_notebook);

		gtk_notebook_set_current_page(notebook, TREEVIEW_OPENFILES);
		gtk_widget_grab_focus(tv.tree_openfiles);
	}
}


void sidebar_focus_symbols_tab(void)
{
	if (ui_prefs.sidebar_visible && interface_prefs.sidebar_symbol_visible)
	{
		GtkNotebook *notebook = GTK_NOTEBOOK(main_widgets.sidebar_notebook);
		GtkWidget *symbol_list_scrollwin = gtk_notebook_get_nth_page(notebook, TREEVIEW_SYMBOL);

		gtk_notebook_set_current_page(notebook, TREEVIEW_SYMBOL);
		gtk_widget_grab_focus(gtk_bin_get_child(GTK_BIN(symbol_list_scrollwin)));
	}
}


static void sidebar_tabs_show_hide(GtkNotebook *notebook, GtkWidget *child,
								   guint page_num, gpointer data)
{
	gint tabs = gtk_notebook_get_n_pages(notebook);

	if (interface_prefs.sidebar_symbol_visible == FALSE)
		tabs--;
	if (interface_prefs.sidebar_openfiles_visible == FALSE)
		tabs--;

	gtk_notebook_set_show_tabs(notebook, (tabs > 1));
}
