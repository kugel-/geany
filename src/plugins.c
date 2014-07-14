/*
 *      plugins.c - this file is part of Geany, a fast and lightweight IDE
 *
 *      Copyright 2007-2012 Enrico Tröger <enrico(dot)troeger(at)uvena(dot)de>
 *      Copyright 2007-2012 Nick Treleaven <nick(dot)treleaven(at)btinternet(dot)com>
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

/* Code to manage, load and unload plugins. */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_PLUGINS

#include "plugins.h"

#include "app.h"
#include "dialogs.h"
#include "encodings.h"
#include "geanyobject.h"
#include "geanywraplabel.h"
#include "geanyplugin2.h"
#include "highlighting.h"
#include "keybindingsprivate.h"
#include "keyfile.h"
#include "main.h"
#include "msgwindow.h"
#include "navqueue.h"
#include "plugindata.h"
#include "pluginprivate.h"
#include "pluginutils.h"
#include "prefs.h"
#include "sciwrappers.h"
#include "stash.h"
#include "support.h"
#include "symbols.h"
#include "templates.h"
#include "toolbar.h"
#include "ui_utils.h"
#include "utils.h"
#include "win32.h"

#include "gtkcompat.h"

#include <string.h>
#include <libpeas/peas.h>
#include <libpeas/peas-plugin-info-priv.h>

static gboolean want_plugins = FALSE;

/* global engine instance */
static PeasEngine *peas;
/* list of all available, loadable plugins, only valid as long as the plugin manager dialog is
 * opened, afterwards it will be destroyed */
static gchar **active_plugins_pref = NULL; 	/* list of plugin filenames to load at startup */
/* non-static because needed by pluginutils */
GHashTable *active_plugins;

static GtkWidget *menu_separator = NULL;

static gchar *get_plugin_path(void);
static void pm_show_dialog(GtkMenuItem *menuitem, gpointer user_data);

/* legacy vtable defined in pluginfuncs.c */
extern GeanyFunctions geany_functions;

GEANY_EXPORT GeanyData *geany_data;

static void
geany_data_init(void)
{
	static GeanyData gd;
	gd = (GeanyData) {
		app,
		&main_widgets,
		documents_array,
		filetypes_array,
		&prefs,
		&interface_prefs,
		&toolbar_prefs,
		&editor_prefs,
		&file_prefs,
		&search_prefs,
		&tool_prefs,
		&template_prefs,
		&build_info,
		filetypes_by_title
	};

	geany_data = &gd;
}

static int get_methods(GeanyPlugin2 *plugin)
{
	int ret;
	g_object_get(G_OBJECT(plugin), "methods", &ret, NULL);
	return ret;
}

static gboolean
plugin_check_version(GeanyPlugin2 *plugin, PeasPluginInfo *info)
{
	gint result = geany_plugin2_version_check(plugin, GEANY_ABI_VERSION);

	if (result < 0)
	{
		msgwin_status_add(_("The plugin \"%s\" is not binary compatible with this "
			"release of Geany - please recompile it."), peas_plugin_info_get_name(info));
		geany_debug("Plugin \"%s\" is not binary compatible with this "
			"release of Geany - recompile it.", peas_plugin_info_get_name(info));
		return FALSE;
	}
	if (result > GEANY_API_VERSION)
	{
		geany_debug("Plugin \"%s\" requires a newer version of Geany (API >= v%d).",
			peas_plugin_info_get_name(info), result);
		return FALSE;
	}
	return TRUE;
}


static void add_callbacks(Plugin *plugin, PluginCallback *callbacks)
{
	PluginCallback *cb;
	guint i, len = 0;

	while (TRUE)
	{
		cb = &callbacks[len];
		if (!cb->signal_name || !cb->callback)
			break;
		len++;
	}
	if (len == 0)
		return;

	for (i = 0; i < len; i++)
	{
		cb = &callbacks[i];

		plugin_signal_connect(&plugin->public, NULL, cb->signal_name, cb->after,
			cb->callback, cb->user_data);
	}
}


static void read_key_group(Plugin *plugin)
{
	GeanyKeyGroupInfo *p_key_info;
	GeanyKeyGroup **p_key_group;

	g_module_symbol(plugin->module, "plugin_key_group_info", (void *) &p_key_info);
	g_module_symbol(plugin->module, "plugin_key_group", (void *) &p_key_group);
	if (p_key_info && p_key_group)
	{
		GeanyKeyGroupInfo *key_info = p_key_info;

		if (*p_key_group)
			geany_debug("Ignoring plugin_key_group symbol for plugin '%s' - "
				"use plugin_set_key_group() instead to allocate keybindings dynamically.",
				plugin->info.name);
		else
		{
			if (key_info->count)
			{
				GeanyKeyGroup *key_group =
					plugin_set_key_group(&plugin->public, key_info->name, key_info->count, NULL);
				if (key_group)
					*p_key_group = key_group;
			}
			else
				geany_debug("Ignoring plugin_key_group_info symbol for plugin '%s' - "
					"count field is zero. Maybe use plugin_set_key_group() instead?",
					plugin->info.name);
		}
	}
	else if (p_key_info || p_key_group)
		geany_debug("Ignoring only one of plugin_key_group[_info] symbols defined for plugin '%s'. "
			"Maybe use plugin_set_key_group() instead?",
			plugin->info.name);
}


static gint cmp_plugin_names(gconstpointer a, gconstpointer b)
{
	const Plugin *pa = a;
	const Plugin *pb = b;

	return strcmp(pa->info.name, pb->info.name);
}


/* Load and optionally init a plugin.
 * init_plugin decides whether the plugin's plugin_init() function should be called or not. If it is
 * called, the plugin will be started, if not the plugin will be read only (for the list of
 * available plugins in the plugin manager).
 * When add_to_list is set, the plugin will be added to the plugin manager's plugin_list. */
static void
plugin_load(GeanyPlugin2 *plugin, PeasPluginInfo *info)
{
	Plugin *plugin_priv;
	gchar *fname;
	GModule *module;
	PluginCallback *callbacks;
	gint methods, configure_mask;
	gboolean is_legacy;

	is_legacy = g_str_equal(peas_plugin_info_get_loader_name(info), "geany");

	/* The module is already loaded by libpeas, so this will just grab another reference.
	 * This cannot fail too */
	fname = g_build_path(G_DIR_SEPARATOR_S, peas_plugin_info_get_module_dir(info),
						peas_plugin_info_get_module_name(info), NULL);
	module = is_legacy ? g_module_open(fname, G_MODULE_BIND_LOCAL) : NULL;

	plugin_priv              = g_new0(Plugin, 1);
	plugin_priv->peas_info   = info;
	plugin_priv->object      = plugin;
	plugin_priv->module      = module;
	plugin_priv->public.info = &plugin_priv->info;
	plugin_priv->public.priv = plugin_priv;

	geany_plugin2_set_priv(plugin, plugin_priv);

	if (is_legacy)
	{
		GeanyPlugin **p_geany_plugin;
		PluginInfo **p_info;
		PluginFields **plugin_fields;
		GeanyData **p_geany_data;
		GeanyFunctions **p_geany_funcs;

		/* set these symbols before plugin_init() is called
		 * this should only be done for legacy .so file plugins, not for the new-style ones.
		 * this might not even be C plugins */
		g_module_symbol(module, "geany_plugin", (void *) &p_geany_plugin);
		if (p_geany_plugin)
			*p_geany_plugin = &plugin_priv->public;
		g_module_symbol(module, "plugin_info", (void *) &p_info);
		if (p_info)
			*p_info = &plugin_priv->info;
		g_module_symbol(module, "plugin_fields", (void *) &plugin_fields);
		if (plugin_fields)
			*plugin_fields = &plugin_priv->fields;

		g_module_symbol(module, "geany_data", (void *) &p_geany_data);
		if (p_geany_data)
			*p_geany_data = geany_data;
		g_module_symbol(module, "geany_functions", (void *) &p_geany_funcs);
		if (p_geany_funcs)
			*p_geany_funcs = &geany_functions;

		read_key_group(plugin_priv);
	}

	/* start the plugin */
	geany_plugin2_initialize(plugin);

	methods = get_methods(plugin);
	configure_mask = GEANY_METHODS_CLEANUP|GEANY_METHODS_CONFIGURE_SINGLE;

	if (app->debug_mode && ((methods & configure_mask) == configure_mask))
		g_warning("Plugin '%s' implements plugin_configure_single() unnecessarily - "
			"only plugin_configure() will be used!",
			peas_plugin_info_get_name(info));

	if ((methods & GEANY_METHODS_CLEANUP) == 0 && app->debug_mode)
	{
		g_warning("Plugin '%s' has no plugin_cleanup() function - there may be memory leaks!",
			peas_plugin_info_get_name(info));
	}

	/* now read any plugin-owned data that might have been set in plugin_init() */

	if (plugin_priv->fields.flags & PLUGIN_IS_DOCUMENT_SENSITIVE)
	{
		ui_add_document_sensitive(plugin_priv->fields.menu_item);
	}

	if (is_legacy)
	{
		g_module_symbol(module, "plugin_callbacks", (void *) &callbacks);
		if (callbacks)
			add_callbacks(plugin_priv, callbacks);
	}

	geany_debug("Loaded:   %s (%s)",
		peas_plugin_info_get_module_name(info), peas_plugin_info_get_name(info));
}


static void on_object_weak_notify(gpointer data, GObject *old_ptr)
{
	Plugin *plugin = data;
	guint i = 0;

	g_return_if_fail(plugin && plugin->signal_ids);

	for (i = 0; i < plugin->signal_ids->len; i++)
	{
		SignalConnection *sc = &g_array_index(plugin->signal_ids, SignalConnection, i);

		if (sc->object == old_ptr)
		{
			g_array_remove_index_fast(plugin->signal_ids, i);
			/* we can break the loop right after finding the first match,
			 * because we will get one notification per connected signal */
			break;
		}
	}
}


/* add an object to watch for destruction, and release pointers to it when destroyed.
 * this should only be used by plugin_signal_connect() to add a watch on
 * the object lifetime and nuke out references to it in plugin->signal_ids */
void plugin_watch_object(Plugin *plugin, gpointer object)
{
	g_object_weak_ref(object, on_object_weak_notify, plugin);
}


static void remove_callbacks(Plugin *plugin)
{
	GArray *signal_ids = plugin->signal_ids;
	SignalConnection *sc;

	if (signal_ids == NULL)
		return;

	foreach_array(SignalConnection, sc, signal_ids)
	{
		g_signal_handler_disconnect(sc->object, sc->handler_id);
		g_object_weak_unref(sc->object, on_object_weak_notify, plugin);
	}

	g_array_free(signal_ids, TRUE);
}


static void remove_sources(Plugin *plugin)
{
	GList *item;

	item = plugin->sources;
	while (item != NULL)
	{
		GList *next = item->next; /* cache the next pointer because current item will be freed */

		g_source_destroy(item->data);
		item = next;
	}
	/* don't free the list here, it is allocated inside each source's data */
}

/* Clean up anything used by an active plugin  */
static void
plugin_cleanup(GeanyPlugin2 *plugin, PeasPluginInfo *info)
{
	GtkWidget *widget;
	Plugin *priv = geany_plugin2_get_priv(plugin);

	geany_plugin2_cleanup(plugin);

	remove_callbacks(priv);
	remove_sources(priv);

	if (priv->key_group)
		keybindings_free_group(priv->key_group);

	widget = priv->toolbar_separator.widget;
	if (widget)
		gtk_widget_destroy(widget);

	g_free(priv);

	geany_debug("Unloaded: %s", peas_plugin_info_get_module_name(info));
}


static gchar *get_custom_plugin_path(const gchar *plugin_path_config,
									 const gchar *plugin_path_system)
{
	gchar *plugin_path_custom;

	if (EMPTY(prefs.custom_plugin_path))
		return NULL;

	plugin_path_custom = utils_get_locale_from_utf8(prefs.custom_plugin_path);
	utils_tidy_path(plugin_path_custom);

	/* check whether the custom plugin path is one of the system or user plugin paths
	 * and abort if so */
	if (utils_str_equal(plugin_path_custom, plugin_path_config) ||
		utils_str_equal(plugin_path_custom, plugin_path_system))
	{
		g_free(plugin_path_custom);
		return NULL;
	}
	return plugin_path_custom;
}


/* all 3 paths Geany looks for plugins in can change (even system path on Windows)
 * so we need to check active plugins are in the right place before loading */
static void add_plugin_paths(PeasEngine *engine)
{
	gchar *user, *system, *custom;
	/* add, in reverse order, the following paths for plugin lookup:
	 * System, e.g. $PREFIX/lib/geany/
	 * User, e.g. $HOME/.config/geany/plugins
	 * Prefs, whatever the user has set as custom path */
	system = get_plugin_path();
	peas_engine_prepend_search_path(engine, system, app->configdir);

	user = g_build_filename(app->configdir, "plugins", NULL);
	peas_engine_prepend_search_path(engine, user, app->configdir);

	custom = get_custom_plugin_path(user, system);
	if (custom) /* Might consider a different data dir for the custom path ones? */
		peas_engine_prepend_search_path(engine, custom, app->configdir);

	g_free(user);
	g_free(system);
	g_free(custom);
}


static gchar *get_plugin_path(void)
{
#ifdef G_OS_WIN32
	gchar *path;
	gchar *install_dir = win32_get_installation_dir();

	path = g_build_filename(install_dir, "lib", NULL);
	g_free(install_dir);

	return path;
#else
	return g_build_filename(GEANY_LIBDIR, "geany", NULL);
#endif
}

static void on_tools_menu_show(GtkWidget *menu_item, G_GNUC_UNUSED gpointer user_data)
{
	GList *item, *list = gtk_container_get_children(GTK_CONTAINER(menu_item));
	guint i = 0;
	gboolean have_plugin_menu_items = FALSE;

	for (item = list; item != NULL; item = g_list_next(item))
	{
		if (item->data == menu_separator)
		{
			if (i < g_list_length(list) - 1)
			{
				have_plugin_menu_items = TRUE;
				break;
			}
		}
		i++;
	}
	g_list_free(list);

	ui_widget_show_hide(menu_separator, have_plugin_menu_items);
}


/* Calling this starts up plugin support */
void plugins_load_active(void)
{
	GtkWidget *widget;

	want_plugins = TRUE;

	geany_data_init();

	widget = gtk_separator_menu_item_new();
	gtk_widget_show(widget);
	gtk_container_add(GTK_CONTAINER(main_widgets.tools_menu), widget);

	widget = gtk_menu_item_new_with_mnemonic(_("_Plugin Manager"));
	gtk_container_add(GTK_CONTAINER(main_widgets.tools_menu), widget);
	gtk_widget_show(widget);
	g_signal_connect(widget, "activate", G_CALLBACK(pm_show_dialog), NULL);

	menu_separator = gtk_separator_menu_item_new();
	gtk_container_add(GTK_CONTAINER(main_widgets.tools_menu), menu_separator);
	g_signal_connect(main_widgets.tools_menu, "show", G_CALLBACK(on_tools_menu_show), NULL);

	if (active_plugins_pref == NULL)
		return;

	peas_engine_set_loaded_plugins(peas, (const gchar **)active_plugins_pref);
}


/* Update the global active plugins list so it's up-to-date when configuration
 * is saved. Called in response to GeanyObject's "save-settings" signal. */
static void update_active_plugins_pref(void)
{
	g_return_if_fail(peas != NULL);
	if (active_plugins_pref)
		g_strfreev(active_plugins_pref);
	active_plugins_pref = peas_engine_get_loaded_plugins(peas);
}


static void on_plugin_loaded(PeasEngine *engine, PeasPluginInfo *info, gpointer user_data)
{
	const gchar *name = peas_plugin_info_get_name(info);
	if (peas_engine_provides_extension(engine, info, GEANY_TYPE_PLUGIN2))
	{
		GeanyPlugin2 *obj = GEANY_PLUGIN2(peas_engine_create_extension(engine, info, GEANY_TYPE_PLUGIN2, NULL));
		gboolean success = plugin_check_version(obj, info);
		if (success)
		{
			plugin_load(obj, info);
			/* remember which plugins are active. */
			g_hash_table_insert(active_plugins, info, obj);
		}
		else
		{/* Keep the plugin loaded, so that we can save it to the prefs and attempt to reload
			 * in the future */
			geany_debug("Plugin %s out of date and must be recompiled\n", name);
			g_object_unref(obj);
		}
	}
	else
	{
		geany_debug("Plugin %s does not implement %s! Ignoring.", name, g_type_name(GEANY_TYPE_PLUGIN2));
		peas_engine_unload_plugin(engine, info);
	}
}


static void on_plugin_unload(PeasEngine *engine, PeasPluginInfo *info, gpointer user_data)
{
	GeanyPlugin2 *obj = g_hash_table_lookup(active_plugins, info);
	if (obj)
	{
		plugin_cleanup(obj, info);
		g_hash_table_remove(active_plugins, info);
		g_object_unref(obj);
	}
}


static gboolean
plugins_provide_info(PeasPluginInfo *info,
                     const char *filename,
                     const gchar *module_dir,
                     const gchar *data_dir,
                     gpointer user_data);

/* called even if plugin support is disabled */
void plugins_init(void)
{
	StashGroup *group;
	gchar *path;

	path = get_plugin_path();
	geany_debug("System plugin path: %s", path);

	peas = peas_engine_get_default();
	peas_engine_enable_loader(peas, "geany");
	peas_engine_enable_loader(peas, "python");

	g_signal_connect_after(peas,   "load-plugin", G_CALLBACK(on_plugin_loaded), NULL);
	g_signal_connect(peas,       "unload-plugin", G_CALLBACK(on_plugin_unload), NULL);
	g_signal_connect_after(peas, "unload-plugin", G_CALLBACK(peas_engine_garbage_collect), NULL);

	add_plugin_paths(peas);
	peas_engine_install_provider(peas, "so", plugins_provide_info, NULL, NULL);
	peas_engine_rescan_plugins(peas);

	active_plugins = g_hash_table_new(g_direct_hash, g_direct_equal);

	g_free(path);

	group = stash_group_new("plugins");
	configuration_add_pref_group(group, TRUE);

	stash_group_add_toggle_button(group, &prefs.load_plugins,
		"load_plugins", TRUE, "check_plugins");
	stash_group_add_entry(group, &prefs.custom_plugin_path,
		"custom_plugin_path", "", "extra_plugin_path_entry");

	g_signal_connect(geany_object, "save-settings", G_CALLBACK(update_active_plugins_pref), NULL);
	stash_group_add_string_vector(group, &active_plugins_pref, "active_plugins", NULL);
}


/* called even if plugin support is disabled */
void plugins_finalize(void)
{
	/* This instructs libpeas to unload all plugins, unloading is done in the signal handler */
	peas_engine_set_loaded_plugins(peas, NULL);
	g_hash_table_destroy(active_plugins);
	g_strfreev(active_plugins_pref);
}


/* Check whether there are any plugins loaded which provide a configure symbol */
gboolean plugins_have_preferences(void)
{
	const GList *item;

	foreach_list(item, peas_engine_get_plugin_list(peas))
	{
		PeasPluginInfo *info = item->data;
		if (peas_plugin_info_is_loaded(info))
		{
			GeanyPlugin2 *obj = g_hash_table_lookup(active_plugins, info);
			gint methods      = get_methods(obj);
			return methods & (GEANY_METHODS_CONFIGURE|GEANY_METHODS_CONFIGURE_SINGLE);
		}
	}

	return FALSE;
}

static gboolean
plugins_provide_info(PeasPluginInfo *info,
                     const char *filename,
                     const gchar *module_dir,
                     const gchar *data_dir,
                     gpointer user_data)
{
	GModule *module;
	const gchar *module_name;
	GeanyFunctions **p_geany_funcs;
	GeanyData **p_geany_data;
	PluginInfo _info;
	void (*set_info)(PluginInfo *info);

	/* BIND_LAZY here since all we care about is plugin_set_info() because
	* this function is really just about filling PeasPluginInfo.
	* The rest of plugin is loaded by the legacy loader at a later point, and
	* then BIND_LOCAL is more appropriate */
	module = g_module_open(filename, G_MODULE_BIND_LAZY);

	if (module == NULL)
	{
		g_warning ("Bad plugin file: '%s'", filename);
		goto error;
	}

	/* Check early if this is actually a legacy geany plugin by looking for geany_plugins.
	 * Do not consider it if not, there mught be unrelated .so files and it wouldn't be useful
	 * anyway */
	g_module_symbol(module, "geany_functions", (void *) &p_geany_funcs);
	if (p_geany_funcs == NULL)
		goto error;
	/* plugins expect to be able to access these two in plugin_set_info already, so set them */
	*p_geany_funcs = &geany_functions;
	g_module_symbol(module, "geany_data", (void *) &p_geany_data);
	if (p_geany_data)
		*p_geany_data = geany_data;

	g_module_symbol(module, "plugin_set_info", (gpointer *)&set_info);
	if (set_info == NULL)
	{
		g_warning ("Plugin %s does not provide plugin_set_info(), ignoring.", filename);
		goto error;
    }
	set_info(&_info);

	if (_info.name == NULL)
	{
		g_warning ("Plugin does not provide 'Name': '%s'", filename);
		goto error;
	}

	module_name = g_utf8_strrchr(filename, -1, '/');
	if (!module_name)
		module_name = filename;
	else
		module_name += 1;

	info->module_name = g_strdup(module_name);
	info->name = g_strdup(_info.name);

	/* The remaining fields are optional */
	if (_info.version)
		info->version = g_strdup (_info.version);

	if (_info.description)
		info->desc = g_strdup (_info.description);

	info->authors = g_new0 (gchar *, 1);
	if (_info.author)
		info->authors[0] = g_strdup (_info.author);

	info->dependencies = g_new0 (gchar *, 1);

	info->loader = g_strdup("geany");

	/* We're done for now */
	g_module_close(module);

	return TRUE;

error:
	if (module != NULL)
		g_module_close(module);

	return FALSE;
}


/* Plugin Manager */

enum
{
	PLUGIN_COLUMN_CHECK = 0,
	PLUGIN_COLUMN_PLUGIN,
	PLUGIN_N_COLUMNS,
	PM_BUTTON_KEYBINDINGS,
	PM_BUTTON_CONFIGURE,
	PM_BUTTON_HELP
};

typedef struct
{
	GtkWidget *dialog;
	GtkWidget *tree;
	GtkListStore *store;
	GtkWidget *configure_button;
	GtkWidget *keybindings_button;
	GtkWidget *help_button;
}
PluginManagerWidgets;

static PluginManagerWidgets pm_widgets;


static void pm_update_buttons(PeasPluginInfo *info)
{
	GeanyPlugin2 *obj = g_hash_table_lookup(active_plugins, info);
	if (obj)
	{
		gint methods = get_methods(obj);
		gboolean has_configure_any = methods & (GEANY_METHODS_CONFIGURE|GEANY_METHODS_CONFIGURE_SINGLE);
		gboolean has_help = methods & GEANY_METHODS_HELP;
		gtk_widget_set_sensitive(pm_widgets.configure_button, has_configure_any);
		gtk_widget_set_sensitive(pm_widgets.help_button, has_help);
		/* FIXME */
		gtk_widget_set_sensitive(pm_widgets.keybindings_button, FALSE);
	}
	else
	{
		gtk_widget_set_sensitive(pm_widgets.configure_button,FALSE);
		gtk_widget_set_sensitive(pm_widgets.help_button, FALSE);
		gtk_widget_set_sensitive(pm_widgets.keybindings_button, FALSE);
	}
}


static void pm_selection_changed(GtkTreeSelection *selection, gpointer user_data)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *p;

	if (gtk_tree_selection_get_selected(selection, &model, &iter))
	{
		gtk_tree_model_get(model, &iter, PLUGIN_COLUMN_PLUGIN, &p, -1);

		if (p != NULL)
		{
			PeasPluginInfo *info = peas_engine_get_plugin_info(peas, p);
			pm_update_buttons(info);
		}
	}
}


static void pm_plugin_toggled(GtkCellRendererToggle *cell, gchar *pth, gpointer data)
{
	gboolean old_state, state;
	gchar *file_name;
	GtkTreeIter iter;
	GtkTreePath *path = gtk_tree_path_new_from_string(pth);
	gchar *p;
	PeasPluginInfo *info;

	gtk_tree_model_get_iter(GTK_TREE_MODEL(pm_widgets.store), &iter, path);
	gtk_tree_path_free(path);

	gtk_tree_model_get(GTK_TREE_MODEL(pm_widgets.store), &iter,
		PLUGIN_COLUMN_CHECK, &old_state, PLUGIN_COLUMN_PLUGIN, &p, -1);

	/* no plugins item */
	if (p == NULL)
		return;

	info = peas_engine_get_plugin_info(peas, p);

	state = ! old_state; /* toggle the state */

	/* unload plugin module */
	if (!state)
	{
		/* save shortcuts (only need this group, but it doesn't take long) */
		keybindings_write_to_file();
		if (peas_plugin_info_is_loaded(info))
			peas_engine_unload_plugin(peas, info);
	}
	else
	{
		/* loading might succeed but the plugin might be incompatible so check if it's really active */
		state = peas_engine_load_plugin(peas, info) && g_hash_table_contains(active_plugins, info);
		if (state)
			keybindings_load_keyfile();		/* load shortcuts */
	}

	/* update model */
	gtk_list_store_set(pm_widgets.store, &iter,
		PLUGIN_COLUMN_CHECK, state, -1);

	/* set again the sensitiveness of the configure and help buttons */
	pm_update_buttons(info);

	g_free(file_name);
}


static gboolean pm_treeview_query_tooltip(GtkWidget *widget, gint x, gint y,
		gboolean keyboard_mode, GtkTooltip *tooltip, gpointer user_data)
{
	GtkTreeModel *model;
	GtkTreeIter iter;
	GtkTreePath *path;
	gchar          *p;
	PeasPluginInfo *info = NULL;

	if (! gtk_tree_view_get_tooltip_context(GTK_TREE_VIEW(widget), &x, &y, keyboard_mode,
			&model, &path, &iter))
		return FALSE;

	gtk_tree_model_get(model, &iter, PLUGIN_COLUMN_PLUGIN, &p, -1);
	if (p != NULL)
		info = peas_engine_get_plugin_info(peas, p);

	if (info != NULL)
	{
		gchar *markup;
		gchar *details;
		gchar *authors;

		authors = g_strjoinv(", ", (gchar **)peas_plugin_info_get_authors(info));
		details = g_strdup_printf(_("Version:\t%s\nAuthor(s):\t%s\nModule:\t%s"),
			peas_plugin_info_get_version(info), authors,
			peas_plugin_info_get_module_name(info));
		markup = g_markup_printf_escaped("<b>%s</b>\n%s\n<small><i>\n%s</i></small>",
			peas_plugin_info_get_name(info),
			peas_plugin_info_get_description(info), details);

		gtk_tooltip_set_markup(tooltip, markup);
		gtk_tree_view_set_tooltip_row(GTK_TREE_VIEW(widget), tooltip, path);

		g_free(details);
		g_free(markup);
	}
	gtk_tree_path_free(path);

	return p != NULL;
}


static void pm_treeview_text_cell_data_func(GtkTreeViewColumn *column, GtkCellRenderer *cell,
		GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
	gchar *p;

	gtk_tree_model_get(model, iter, PLUGIN_COLUMN_PLUGIN, &p, -1);

	if (p == NULL)
		g_object_set(cell, "text", _("No plugins available."), NULL);
	else
	{
		PeasPluginInfo *info = peas_engine_get_plugin_info(peas, p);
		gchar *markup;

		g_return_if_fail(info != NULL);

		markup = g_markup_printf_escaped("<b>%s</b>\n%s",
				peas_plugin_info_get_name(info),
				peas_plugin_info_get_description(info));

		g_object_set(cell, "markup", markup, NULL);
		g_free(markup);
	}
}


static gint pm_tree_sort_func(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b,
		gpointer user_data)
{
	gchar *pa, *pb;

	gtk_tree_model_get(model, a, PLUGIN_COLUMN_PLUGIN, &pa, -1);
	gtk_tree_model_get(model, b, PLUGIN_COLUMN_PLUGIN, &pb, -1);

	if (pa && pb)
		return strcmp(pa, pb);
	else
		return pa - pb;
}


static void pm_prepare_treeview(GtkWidget *tree, GtkListStore *store)
{
	GtkCellRenderer *text_renderer, *checkbox_renderer;
	GtkTreeViewColumn *column;
	GtkTreeIter iter;
	GList *list;
	GtkTreeSelection *sel;

	g_signal_connect(tree, "query-tooltip", G_CALLBACK(pm_treeview_query_tooltip), NULL);
	gtk_widget_set_has_tooltip(tree, TRUE);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree), FALSE);

	checkbox_renderer = gtk_cell_renderer_toggle_new();
	column = gtk_tree_view_column_new_with_attributes(
		_("Active"), checkbox_renderer, "active", PLUGIN_COLUMN_CHECK, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
	g_signal_connect(checkbox_renderer, "toggled", G_CALLBACK(pm_plugin_toggled), NULL);

	text_renderer = gtk_cell_renderer_text_new();
	g_object_set(text_renderer, "ellipsize", PANGO_ELLIPSIZE_END, NULL);
	column = gtk_tree_view_column_new_with_attributes(_("Plugin"), text_renderer, NULL);
	gtk_tree_view_column_set_cell_data_func(column, text_renderer,
		pm_treeview_text_cell_data_func, NULL, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

	gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(tree), TRUE);
	gtk_tree_view_set_enable_search(GTK_TREE_VIEW(tree), FALSE);
	gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(store), PLUGIN_COLUMN_PLUGIN,
		pm_tree_sort_func, NULL, NULL);
	gtk_tree_sortable_set_sort_column_id(
		GTK_TREE_SORTABLE(store), PLUGIN_COLUMN_PLUGIN, GTK_SORT_ASCENDING);

	/* selection handling */
	sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
	gtk_tree_selection_set_mode(sel, GTK_SELECTION_SINGLE);
	g_signal_connect(sel, "changed", G_CALLBACK(pm_selection_changed), NULL);

	list = (GList *) peas_engine_get_plugin_list(peas);
	if (g_list_length(list) == 0)
	{
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, PLUGIN_COLUMN_CHECK, FALSE,
				PLUGIN_COLUMN_PLUGIN, NULL, -1);
	}
	else
	{
		for (; list != NULL; list = list->next)
		{
			PeasPluginInfo *info = list->data;
			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store, &iter,
				PLUGIN_COLUMN_CHECK,  g_hash_table_contains(active_plugins, info),
				PLUGIN_COLUMN_PLUGIN, peas_plugin_info_get_module_name(info),
				-1);
		}
	}
	gtk_tree_view_set_model(GTK_TREE_VIEW(tree), GTK_TREE_MODEL(store));
	g_object_unref(store);
}


static void pm_on_plugin_button_clicked(GtkButton *button, gpointer user_data)
{
	GtkTreeModel *model;
	GtkTreeSelection *selection;
	GtkTreeIter iter;
	gchar *p;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(pm_widgets.tree));
	if (gtk_tree_selection_get_selected(selection, &model, &iter))
	{
		gtk_tree_model_get(model, &iter, PLUGIN_COLUMN_PLUGIN, &p, -1);

		if (p != NULL)
		{
			PeasPluginInfo *info = peas_engine_get_plugin_info(peas, p);
			GeanyPlugin2   *obj = g_hash_table_lookup(active_plugins, info);
			Plugin         *priv = geany_plugin2_get_priv(obj);
			if (GPOINTER_TO_INT(user_data) == PM_BUTTON_CONFIGURE)
				plugin_show_configure(&priv->public);
			else if (GPOINTER_TO_INT(user_data) == PM_BUTTON_HELP &&
					(get_methods(obj) & GEANY_METHODS_HELP))
				geany_plugin2_help(obj);
			else if (GPOINTER_TO_INT(user_data) == PM_BUTTON_KEYBINDINGS && priv->key_group && priv->key_group->plugin_key_count > 0)
				keybindings_dialog_show_prefs_scroll(priv->info.name);
		}
	}
}


/* Callback when plugin manager dialog closes, only ever has response of
 * GTK_RESPONSE_OK or GTK_RESPONSE_DELETE_EVENT and both are treated the same. */
static void pm_dialog_response(GtkDialog *dialog, gint response, gpointer user_data)
{
	/* TODO: free non-active plugins (?) */
	gtk_widget_destroy(GTK_WIDGET(dialog));

	configuration_save();
}


static void pm_show_dialog(GtkMenuItem *menuitem, gpointer user_data)
{
	GtkWidget *vbox, *vbox2, *hbox, *swin, *label;

	pm_widgets.dialog = gtk_dialog_new_with_buttons(_("Plugins"), GTK_WINDOW(main_widgets.window),
						GTK_DIALOG_DESTROY_WITH_PARENT,
						GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
	vbox = ui_dialog_vbox_new(GTK_DIALOG(pm_widgets.dialog));
	gtk_widget_set_name(pm_widgets.dialog, "GeanyDialog");
	gtk_box_set_spacing(GTK_BOX(vbox), 6);

	gtk_window_set_default_size(GTK_WINDOW(pm_widgets.dialog), 500, 450);

	pm_widgets.tree = gtk_tree_view_new();
	pm_widgets.store = gtk_list_store_new(
		PLUGIN_N_COLUMNS, G_TYPE_BOOLEAN, G_TYPE_POINTER);
	pm_prepare_treeview(pm_widgets.tree, pm_widgets.store);

	swin = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(swin),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(swin), GTK_SHADOW_IN);
	gtk_container_add(GTK_CONTAINER(swin), pm_widgets.tree);

	label = geany_wrap_label_new(_("Choose which plugins should be loaded at startup:"));

	pm_widgets.keybindings_button = gtk_button_new_with_label(_("Keybindings"));
	gtk_widget_set_sensitive(pm_widgets.keybindings_button, FALSE);
	g_signal_connect(pm_widgets.keybindings_button, "clicked",
		G_CALLBACK(pm_on_plugin_button_clicked), GINT_TO_POINTER(PM_BUTTON_KEYBINDINGS));

	pm_widgets.configure_button = gtk_button_new_from_stock(GTK_STOCK_PREFERENCES);
	gtk_widget_set_sensitive(pm_widgets.configure_button, FALSE);
	g_signal_connect(pm_widgets.configure_button, "clicked",
		G_CALLBACK(pm_on_plugin_button_clicked), GINT_TO_POINTER(PM_BUTTON_CONFIGURE));

	pm_widgets.help_button = gtk_button_new_from_stock(GTK_STOCK_HELP);
	gtk_widget_set_sensitive(pm_widgets.help_button, FALSE);
	g_signal_connect(pm_widgets.help_button, "clicked",
		G_CALLBACK(pm_on_plugin_button_clicked), GINT_TO_POINTER(PM_BUTTON_HELP));

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_set_spacing(GTK_BOX(hbox), 6);
	gtk_box_pack_end(GTK_BOX(hbox), pm_widgets.keybindings_button, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(hbox), pm_widgets.configure_button, FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(hbox), pm_widgets.help_button, FALSE, FALSE, 0);

	vbox2 = gtk_vbox_new(FALSE, 3);
	gtk_box_pack_start(GTK_BOX(vbox2), label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(vbox2), swin, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox2), hbox, FALSE, TRUE, 0);

	g_signal_connect(pm_widgets.dialog, "response", G_CALLBACK(pm_dialog_response), NULL);

	gtk_box_pack_start(GTK_BOX(vbox), vbox2, TRUE, TRUE, 0);
	gtk_widget_show_all(pm_widgets.dialog);
}


#endif
