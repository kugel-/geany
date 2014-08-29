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

static gboolean want_plugins = FALSE;

/* global engine instance */
static PeasEngine *peas;
/* list of all available, loadable plugins, only valid as long as the plugin manager dialog is
 * opened, afterwards it will be destroyed */
static gchar **active_plugins_pref = NULL; 	/* list of plugin filenames to load at startup */
/* Using a tree to sort automatically, useful for creating the plugin pref dialog */
static GTree *active_plugins;

static GtkWidget *menu_separator = NULL;

static gchar *get_plugin_path(void);
static void pm_show_dialog(GtkMenuItem *menuitem, gpointer user_data);

static PluginFuncs plugin_funcs = {
	&plugin_add_toolbar_item,
	&plugin_module_make_resident,
	&plugin_signal_connect,
	&plugin_set_key_group,
	&plugin_show_configure,
	&plugin_timeout_add,
	&plugin_timeout_add_seconds,
	&plugin_idle_add,
	&plugin_builder_connect_signals
};

static DocumentFuncs doc_funcs = {
	&document_new_file,
	&document_get_current,
	&document_get_from_page,
	&document_find_by_filename,
	&document_find_by_real_path,
	&document_save_file,
	&document_open_file,
	&document_open_files,
	&document_remove_page,
	&document_reload_force,
	&document_set_encoding,
	&document_set_text_changed,
	&document_set_filetype,
	&document_close,
	&document_index,
	&document_save_file_as,
	&document_rename_file,
	&document_get_status_color,
	&document_get_basename_for_display,
	&document_get_notebook_page,
	&document_compare_by_display_name,
	&document_compare_by_tab_order,
	&document_compare_by_tab_order_reverse,
	&document_find_by_id
};

static EditorFuncs editor_funcs = {
	&editor_get_indent_prefs,
	&editor_create_widget,
	&editor_indicator_set_on_range,
	&editor_indicator_set_on_line,
	&editor_indicator_clear,
	&editor_set_indent_type,
	&editor_get_word_at_pos,
	&editor_get_eol_char_name,
	&editor_get_eol_char_len,
	&editor_get_eol_char,
	&editor_insert_text_block,
	&editor_get_eol_char_mode,
	&editor_goto_pos,
	&editor_find_snippet,
	&editor_insert_snippet
};

static ScintillaFuncs scintilla_funcs = {
	&scintilla_send_message,
	&scintilla_new
};

/* Macro to prevent a duplicate macro being generated in geanyfunctions.h */
#define _scintilla_send_message_macro	scintilla_send_message

static SciFuncs sci_funcs = {
	&_scintilla_send_message_macro,
	&sci_send_command,
	&sci_start_undo_action,
	&sci_end_undo_action,
	&sci_set_text,
	&sci_insert_text,
	&sci_get_text,
	&sci_get_length,
	&sci_get_current_position,
	&sci_set_current_position,
	&sci_get_col_from_position,
	&sci_get_line_from_position,
	&sci_get_position_from_line,
	&sci_replace_sel,
	&sci_get_selected_text,
	&sci_get_selected_text_length,
	&sci_get_selection_start,
	&sci_get_selection_end,
	&sci_get_selection_mode,
	&sci_set_selection_mode,
	&sci_set_selection_start,
	&sci_set_selection_end,
	&sci_get_text_range,
	&sci_get_line,
	&sci_get_line_length,
	&sci_get_line_count,
	&sci_get_line_is_visible,
	&sci_ensure_line_is_visible,
	&sci_scroll_caret,
	&sci_find_matching_brace,
	&sci_get_style_at,
	&sci_get_char_at,
	&sci_get_current_line,
	&sci_has_selection,
	&sci_get_tab_width,
	&sci_indicator_clear,
	&sci_indicator_set,
	&sci_get_contents,
	&sci_get_contents_range,
	&sci_get_selection_contents,
	&sci_set_font,
	&sci_get_line_end_position,
	&sci_set_target_start,
	&sci_set_target_end,
	&sci_replace_target,
	&sci_set_marker_at_line,
	&sci_delete_marker_at_line,
	&sci_is_marker_set_at_line,
	&sci_goto_line,
	&sci_find_text,
	&sci_set_line_indentation,
	&sci_get_line_indentation,
	&sci_get_lexer
};

static TemplateFuncs template_funcs = {
	&templates_get_template_fileheader
};

static UtilsFuncs utils_funcs = {
	&utils_str_equal,
	&utils_string_replace_all,
	&utils_get_file_list,
	&utils_write_file,
	&utils_get_locale_from_utf8,
	&utils_get_utf8_from_locale,
	&utils_remove_ext_from_filename,
	&utils_mkdir,
	&utils_get_setting_boolean,
	&utils_get_setting_integer,
	&utils_get_setting_string,
	&utils_spawn_sync,
	&utils_spawn_async,
	&utils_str_casecmp,
	&utils_get_date_time,
	&utils_open_browser,
	&utils_string_replace_first,
	&utils_str_middle_truncate,
	&utils_str_remove_chars,
	&utils_get_file_list_full,
	&utils_copy_environment,
	&utils_find_open_xml_tag,
	&utils_find_open_xml_tag_pos
};

static UIUtilsFuncs uiutils_funcs = {
	&ui_dialog_vbox_new,
	&ui_frame_new_with_alignment,
	&ui_set_statusbar,
	&ui_table_add_row,
	&ui_path_box_new,
	&ui_button_new_with_image,
	&ui_add_document_sensitive,
	&ui_widget_set_tooltip_text,
	&ui_image_menu_item_new,
	&ui_lookup_widget,
	&ui_progress_bar_start,
	&ui_progress_bar_stop,
	&ui_entry_add_clear_icon,
	&ui_menu_add_document_items,
	&ui_widget_modify_font_from_string,
	&ui_is_keyval_enter_or_return,
	&ui_get_gtk_settings_integer,
	&ui_combo_box_add_to_history,
	&ui_menu_add_document_items_sorted,
	&ui_lookup_stock_label
};

static DialogFuncs dialog_funcs = {
	&dialogs_show_question,
	&dialogs_show_msgbox,
	&dialogs_show_save_as,
	&dialogs_show_input_numeric,
	&dialogs_show_input
};

/* Macro to prevent confusing macro being generated in geanyfunctions.h */
#define _lookup_widget_macro	ui_lookup_widget

/* deprecated */
static SupportFuncs support_funcs = {
	&_lookup_widget_macro
};

static MsgWinFuncs msgwin_funcs = {
	&msgwin_status_add,
	&msgwin_compiler_add,
	&msgwin_msg_add,
	&msgwin_clear_tab,
	&msgwin_switch_tab,
	&msgwin_set_messages_dir
};

static EncodingFuncs encoding_funcs = {
	&encodings_convert_to_utf8,
	&encodings_convert_to_utf8_from_charset,
	&encodings_get_charset_from_index
};

static KeybindingFuncs keybindings_funcs = {
	&keybindings_send_command,
	&keybindings_set_item,
	&keybindings_get_item
};

static TagManagerFuncs tagmanager_funcs = {
	&tm_get_real_path,
	&tm_source_file_new,
	&tm_source_file_free,
	&tm_workspace_add_source_file,
	&tm_workspace_remove_source_file,
	&tm_workspace_add_source_files,
	&tm_workspace_remove_source_files
};

static SearchFuncs search_funcs = {
	&search_show_find_in_files_dialog
};

static HighlightingFuncs highlighting_funcs = {
	&highlighting_get_style,
	&highlighting_set_styles,
	&highlighting_is_string_style,
	&highlighting_is_comment_style,
	&highlighting_is_code_style
};

static FiletypeFuncs filetype_funcs = {
	&filetypes_detect_from_file,
	&filetypes_lookup_by_name,
	&filetypes_index,
	&filetypes_get_display_name,
	&filetypes_get_sorted_by_name
};

static NavQueueFuncs navqueue_funcs = {
	&navqueue_goto_line
};

static MainFuncs main_funcs = {
	&main_reload_configuration,
	&main_locale_init,
	&main_is_realized
};

static StashFuncs stash_funcs = {
	&stash_group_new,
	&stash_group_add_boolean,
	&stash_group_add_integer,
	&stash_group_add_string,
	&stash_group_add_string_vector,
	&stash_group_load_from_key_file,
	&stash_group_save_to_key_file,
	&stash_group_free,
	&stash_group_load_from_file,
	&stash_group_save_to_file,
	&stash_group_add_toggle_button,
	&stash_group_add_radio_buttons,
	&stash_group_add_spin_button_integer,
	&stash_group_add_combo_box,
	&stash_group_add_combo_box_entry,
	&stash_group_add_entry,
	&stash_group_add_widget_property,
	&stash_group_display,
	&stash_group_update,
	&stash_group_free_settings
};

static SymbolsFuncs symbols_funcs = {
	&symbols_get_context_separator
};

static BuildFuncs build_funcs = {
	&build_activate_menu_item,
	&build_get_current_menu_item,
	&build_remove_menu_item,
	&build_set_menu_item,
	&build_get_group_count
};

static GeanyFunctions geany_functions = {
	&doc_funcs,
	&sci_funcs,
	&template_funcs,
	&utils_funcs,
	&uiutils_funcs,
	&support_funcs,
	&dialog_funcs,
	&msgwin_funcs,
	&encoding_funcs,
	&keybindings_funcs,
	&tagmanager_funcs,
	&search_funcs,
	&highlighting_funcs,
	&filetype_funcs,
	&navqueue_funcs,
	&editor_funcs,
	&main_funcs,
	&plugin_funcs,
	&scintilla_funcs,
	&msgwin_funcs,
	&stash_funcs,
	&symbols_funcs,
	&build_funcs
};

GeanyFunctions *geany_get_funcs(void)
{
	return &geany_functions;
}

GeanyData *geany_get_data(void)
{
	static gsize init_val;
	static GeanyData gd;
	
	if (g_once_init_enter(&init_val))
	{
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
		g_once_init_leave(&init_val, 1);
	}

	return &gd;
}


static gboolean plugin_check_version(PeasPluginInfo *info)
{
	const gchar *str;
	gint abi_ver, api_ver;

	str = peas_plugin_info_get_external_data(info, "X-Geany-ABI");
	if (!str)
		return FALSE;
	abi_ver = g_ascii_strtoll(str, NULL, 10);

	if (abi_ver != GEANY_ABI_VERSION)
	{
		msgwin_status_add(_("The plugin \"%s\" is not binary compatible with this "
			"release of Geany - please recompile it."), peas_plugin_info_get_name(info));
		geany_debug("Plugin \"%s\" is not binary compatible with this "
			"release of Geany - recompile it.", peas_plugin_info_get_name(info));
		return FALSE;
	}

	str = peas_plugin_info_get_external_data(info, "X-Geany-API");
	if (!str)
		return FALSE;
	api_ver = g_ascii_strtoll(str, NULL, 10);

	if (api_ver > GEANY_API_VERSION)
	{
		geany_debug("Plugin \"%s\" requires a newer version of Geany (API >= v%d).",
			peas_plugin_info_get_name(info), api_ver);
		return FALSE;
	}
	return TRUE;
}

GeanyPlugin2Configure* new_configure_instance(Plugin *plugin)
{
	PeasPluginInfo *info = peas_extension_base_get_plugin_info(PEAS_EXTENSION_BASE(plugin));
	return (GeanyPlugin2Configure* )peas_engine_create_extension(peas, info, GEANY_TYPE_PLUGIN2_CONFIGURE, NULL);
}


GeanyPlugin2Help* new_help_instance(Plugin *plugin)
{
	PeasPluginInfo *info = peas_extension_base_get_plugin_info(PEAS_EXTENSION_BASE(plugin));
	return (GeanyPlugin2Help *)peas_engine_create_extension(peas, info, GEANY_TYPE_PLUGIN2_HELP, NULL);
}


static void on_object_weak_notify(gpointer data, GObject *old_ptr)
{
	PluginPriv *plugin = GEANY_PLUGIN2_BASE_GET_PRIVATE(data);
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
	GArray *signal_ids = GEANY_PLUGIN2_BASE_GET_PRIVATE(plugin)->signal_ids;
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

	item = GEANY_PLUGIN2_BASE_GET_PRIVATE(plugin)->sources;
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
plugin_cleanup(Plugin *plugin, PeasPluginInfo *info)
{
	GtkWidget *widget;
	PluginPriv *priv = GEANY_PLUGIN2_BASE_GET_PRIVATE(plugin);

	geany_plugin2_cleanup(GEANY_PLUGIN2(plugin));

	remove_callbacks(plugin);
	remove_sources(plugin);

	if (priv->key_group)
		keybindings_free_group(priv->key_group);

	widget = priv->toolbar_separator.widget;
	if (widget)
		gtk_widget_destroy(widget);
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
	Plugin *plugin = NULL;

	if (peas_engine_provides_extension(engine, info, GEANY_TYPE_PLUGIN2))
	{
		/* If the version check fails keep the plugin loaded anyway, so that we can save it to
		 * the prefs and attempt to reload in the future */
		if (plugin_check_version(info))
		{
			plugin = GEANY_PLUGIN2_BASE(peas_engine_create_extension(engine, info, GEANY_TYPE_PLUGIN2, NULL));
			if (plugin)
			{
				PluginPriv *priv = GEANY_PLUGIN2_BASE_GET_PRIVATE(plugin);
				/* Set up configure factory. Either the plugin provides them as a
				 * separate extension, or it is implemented in its GeanyPlugin2Base type */
				if (peas_engine_provides_extension(peas, info, GEANY_TYPE_PLUGIN2_CONFIGURE))
					priv->new_configure = new_configure_instance;
				else if (GEANY_IS_PLUGIN2_CONFIGURE(plugin))
					priv->new_configure = (gpointer) g_object_ref;
				/* ditto for help */
				if (peas_engine_provides_extension(peas, info, GEANY_TYPE_PLUGIN2_HELP))
					priv->new_help = new_help_instance;
				else if (GEANY_IS_PLUGIN2_HELP(plugin))
					priv->new_help = (gpointer) g_object_ref;
				/* now read any plugin-owned data that might have been set in plugin_init() */
				if (priv->fields.flags & PLUGIN_IS_DOCUMENT_SENSITIVE)
					ui_add_document_sensitive(priv->fields.menu_item);
				/* remember which plugins are active. do not ref, g_tree_remove()
				 * will unref automatically */
				g_tree_insert(active_plugins, info, plugin);
				if (G_UNLIKELY(priv->new_configure && priv->configure_single))
				{
					if (app->debug_mode)
						g_warning("Plugin '%s' implements plugin_configure_single() unnecessarily - "
								"only plugin_configure() will be used!", peas_plugin_info_get_name(info));
					priv->configure_single = NULL;
				}
				geany_debug("Loaded:   %s (%s)",
						peas_plugin_info_get_module_name(info), peas_plugin_info_get_name(info));
			}
		}
	}

	if (!plugin)
	{
		geany_debug("Plugin %s does not implement %s! Ignoring.", name, g_type_name(GEANY_TYPE_PLUGIN2));
		peas_engine_unload_plugin(engine, info);
	}
}


static void on_plugin_unload(PeasEngine *engine, PeasPluginInfo *info, gpointer user_data)
{
	Plugin *plugin = g_tree_lookup(active_plugins, info);
	if (plugin)
	{
		plugin_cleanup(plugin, info);
		/* will also unref and destroy the plugin instance */
		g_tree_remove(active_plugins, info);
		geany_debug("Unloaded: %s", peas_plugin_info_get_name(info));
	}
}


static gint cmp_plugin_names(gconstpointer a, gconstpointer b, gpointer data)
{
	PeasPluginInfo *ia = (PeasPluginInfo *) a;
	PeasPluginInfo *ib = (PeasPluginInfo *) b;
	return utils_str_casecmp(peas_plugin_info_get_name(ia), peas_plugin_info_get_name(ib));
}


/* called even if plugin support is disabled */
void plugins_init(void)
{
	StashGroup *group;
	gchar *path;

	path = get_plugin_path();
	geany_debug("System plugin path: %s", path);

	peas = peas_engine_get_default();
	peas_engine_enable_loader(peas, "c");
	peas_engine_enable_loader(peas, "python");

	g_signal_connect_after(peas,   "load-plugin", G_CALLBACK(on_plugin_loaded), NULL);
	g_signal_connect(peas,       "unload-plugin", G_CALLBACK(on_plugin_unload), NULL);
	g_signal_connect_after(peas, "unload-plugin", G_CALLBACK(peas_engine_garbage_collect), NULL);

	add_plugin_paths(peas);
	peas_engine_rescan_plugins(peas);

	active_plugins = g_tree_new_full(cmp_plugin_names, NULL, NULL, g_object_unref);

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
	g_tree_destroy(active_plugins);
	g_strfreev(active_plugins_pref);
}


/* Plugin configuration */

static gboolean
plugin_is_configurable(PeasPluginInfo *info, Plugin *plugin, gboolean *result)
{
	gboolean ret;
	PluginPriv *priv = GEANY_PLUGIN2_BASE_GET_PRIVATE(plugin);

	ret = priv->new_configure || priv->configure_single;
	if (result)
		*result = ret;
	return ret;
}


/* Check whether there are any plugins loaded which provide a configure symbol */
gboolean plugins_have_preferences(void)
{
	gboolean ret = FALSE;

	g_tree_foreach(active_plugins, (GTraverseFunc) plugin_is_configurable, &ret);

	return ret;
}


struct CreatePrefPageData
{
	GtkDialog *dialog;  /* in */
	Plugin    *current; /* in */
	GtkNotebook *nb;    /* in */
	gint       current_page;  /* out */
};


static gboolean create_pref_page(PeasPluginInfo *info, Plugin *plugin, struct CreatePrefPageData *data)
{
	GtkWidget *label, *page = NULL;	/* some plugins don't have prefs */
	PluginPriv *priv = GEANY_PLUGIN2_BASE_GET_PRIVATE(plugin);
	gint n;

	if (priv->configure_single)
	{
		GtkWidget *align = gtk_alignment_new(0.5, 0.5, 0, 0);
		GtkWidget *btn;

		gtk_alignment_set_padding(GTK_ALIGNMENT(align), 6, 6, 6, 6);

		btn = gtk_button_new_from_stock(GTK_STOCK_PREFERENCES);
		g_signal_connect_object(btn, "clicked", G_CALLBACK(priv->configure_single),
								main_widgets.window, G_CONNECT_SWAPPED);
		gtk_container_add(GTK_CONTAINER(align), btn);
		page = align;
	}
	else if (priv->new_configure)
	{
		GeanyPlugin2Configure *cfg = priv->new_configure(plugin);

		page = geany_plugin2_configure_get_config_widget(cfg, GTK_DIALOG(data->dialog));

		g_object_unref(cfg);
		if (! GTK_IS_WIDGET(page))
		{
			geany_debug("Invalid widget returned from plugin_configure() in plugin \"%s\"!",
				peas_plugin_info_get_name(info));
			return FALSE;
		}
		else
		{
			GtkWidget *align = gtk_alignment_new(0.5, 0.5, 1, 1);

			gtk_alignment_set_padding(GTK_ALIGNMENT(align), 6, 6, 6, 6);
			gtk_container_add(GTK_CONTAINER(align), page);
			page = gtk_vbox_new(FALSE, 0);
			gtk_box_pack_start(GTK_BOX(page), align, TRUE, TRUE, 0);
		}
	}

	if (page)
	{
		label = gtk_label_new(peas_plugin_info_get_name(info));
		n = gtk_notebook_append_page(GTK_NOTEBOOK(data->nb), page, label);

		/* output the designated current tab to be presented to the user */
		if (plugin == data->current)
			data->current_page = n;
	}
	return FALSE;
}


/* multiple plugin configure dialog
 * current_plugin can be NULL */
static void configure_plugins(Plugin *current, PeasPluginInfo *current_info)
{
	GtkWidget *dialog, *vbox, *nb;
	struct CreatePrefPageData data;

	dialog = gtk_dialog_new_with_buttons(_("Configure Plugins"),
		GTK_WINDOW(main_widgets.window), GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_STOCK_APPLY, GTK_RESPONSE_APPLY,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
	gtk_widget_set_name(dialog, "GeanyDialog");

	vbox = ui_dialog_vbox_new(GTK_DIALOG(dialog));
	nb = gtk_notebook_new();
	gtk_notebook_set_scrollable(GTK_NOTEBOOK(nb), TRUE);
	gtk_box_pack_start(GTK_BOX(vbox), nb, TRUE, TRUE, 0);

	data.dialog = (GtkDialog *) dialog;
	data.current = current;
	data.nb = (GtkNotebook *) nb;
	data.current_page = -1;
	g_tree_foreach(active_plugins, (GTraverseFunc) create_pref_page, &data);
	if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(nb)))
	{
		gtk_widget_show_all(vbox);
		if (data.current_page >= 0)
			gtk_notebook_set_current_page(GTK_NOTEBOOK(nb), data.current_page);

		/* run the dialog */
		while (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_APPLY);
	}
	else
		utils_beep();

	gtk_widget_destroy(dialog);
}


/* Shows the plugin's configure dialog.
 * The plugin must implement one of the plugin_configure() or plugin_configure_single() symbols.
 * NULL to show the configure dialog for all plugins
 */
void plugins_configure(Plugin *current)
{
	if (!current) /* current == NULL should just show the plugins pref dialog */
	{
		configure_plugins(NULL, NULL);
	}
	else if (GEANY_PLUGIN2_BASE_GET_PRIVATE(current)->new_configure)
	{
		PeasPluginInfo *info = peas_extension_base_get_plugin_info(PEAS_EXTENSION_BASE(current));
		configure_plugins(current, info);
	}
	else
	{
		PluginPriv *priv = GEANY_PLUGIN2_BASE_GET_PRIVATE(current);
		g_return_if_fail(priv->configure_single != NULL);
		priv->configure_single(main_widgets.window);
	}
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
	GeanyPlugin2Base *plugin = g_tree_lookup(active_plugins, info);
	if (plugin)
	{
		PluginPriv *priv = GEANY_PLUGIN2_BASE_GET_PRIVATE(plugin);
		gtk_widget_set_sensitive(pm_widgets.configure_button,
				plugin_is_configurable(info, plugin, NULL));
		gtk_widget_set_sensitive(pm_widgets.help_button, priv->new_help != NULL);
		gtk_widget_set_sensitive(pm_widgets.keybindings_button,
				priv->key_group && priv->key_group->plugin_key_count > 0);
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
	PeasPluginInfo *info;

	if (gtk_tree_selection_get_selected(selection, &model, &iter))
	{
		gtk_tree_model_get(model, &iter, PLUGIN_COLUMN_PLUGIN, &info, -1);
		pm_update_buttons(info);
	}
}


static void pm_plugin_toggled(GtkCellRendererToggle *cell, gchar *pth, gpointer data)
{
	gboolean old_state, state;
	gchar *file_name;
	GtkTreeIter iter;
	GtkTreePath *path = gtk_tree_path_new_from_string(pth);
	PeasPluginInfo *info;

	gtk_tree_model_get_iter(GTK_TREE_MODEL(pm_widgets.store), &iter, path);
	gtk_tree_path_free(path);

	gtk_tree_model_get(GTK_TREE_MODEL(pm_widgets.store), &iter,
		PLUGIN_COLUMN_CHECK, &old_state, PLUGIN_COLUMN_PLUGIN, &info, -1);

	/* no plugins item */
	if (info == NULL)
		return;

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
		state = peas_engine_load_plugin(peas, info) && g_tree_lookup(active_plugins, info) != NULL;
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
	PeasPluginInfo *info;

	if (! gtk_tree_view_get_tooltip_context(GTK_TREE_VIEW(widget), &x, &y, keyboard_mode,
			&model, &path, &iter))
		return FALSE;

	gtk_tree_model_get(model, &iter, PLUGIN_COLUMN_PLUGIN, &info, -1);
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
		g_free(authors);
	}
	gtk_tree_path_free(path);

	return info != NULL;
}


static void pm_treeview_text_cell_data_func(GtkTreeViewColumn *column, GtkCellRenderer *cell,
		GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
	PeasPluginInfo *info;

	gtk_tree_model_get(model, iter, PLUGIN_COLUMN_PLUGIN, &info, -1);

	if (info == NULL)
		g_object_set(cell, "text", _("No plugins available."), NULL);
	else
	{
		gchar *markup;

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
	PeasPluginInfo *pa, *pb;

	gtk_tree_model_get(model, a, PLUGIN_COLUMN_PLUGIN, &pa, -1);
	gtk_tree_model_get(model, b, PLUGIN_COLUMN_PLUGIN, &pb, -1);

	return cmp_plugin_names(pa, pb, NULL);
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
				PLUGIN_COLUMN_CHECK,  g_tree_lookup(active_plugins, info) != NULL,
				PLUGIN_COLUMN_PLUGIN, info,
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
	PeasPluginInfo *info;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(pm_widgets.tree));
	if (gtk_tree_selection_get_selected(selection, &model, &iter))
	{
		Plugin *plugin;
		PluginPriv *priv;

		gtk_tree_model_get(model, &iter, PLUGIN_COLUMN_PLUGIN, &info, -1);
		if (info)
		{
			plugin = g_tree_lookup(active_plugins, info);
			priv = PLUGIN_GET_PRIVATE(plugin);
			switch (GPOINTER_TO_INT(user_data))
			{
				case PM_BUTTON_CONFIGURE:
				{
					plugins_configure(plugin);
					break;
				}
				case PM_BUTTON_HELP:
				{
					GeanyPlugin2Help *obj = priv->new_help(plugin);
					geany_plugin2_help_provide_help(obj);
					g_object_unref(obj);
					break;
				}
				case PM_BUTTON_KEYBINDINGS:
				{
					if (priv->key_group && priv->key_group->plugin_key_count > 0)
					{
						PeasPluginInfo *info = peas_extension_base_get_plugin_info(PEAS_EXTENSION_BASE(plugin));
						keybindings_dialog_show_prefs_scroll(peas_plugin_info_get_name(info));
					}
					break;
				}
			}
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
