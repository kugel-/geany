/*
 *      demoplugin.c - this file is part of Geany, a fast and lightweight IDE
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

/**
 * Demo plugin - example of a basic plugin for Geany. Adds a menu item to the
 * Tools menu.
 *
 * Note: This is not installed by default, but (on *nix) you can build it as follows:
 * cd plugins
 * make demoplugin.so
 *
 * Then copy or symlink the plugins/demoplugin.so file to ~/.config/geany/plugins
 * - it will be loaded at next startup.
 */


#include "geanyplugin.h"	/* plugin API, always comes first */
#include "Scintilla.h"	/* for the SCNotification struct */


// A simple global variable used to hold all the plugin's state
static struct DemoPluginData
{
	GtkWidget   *menu_item;    // menu item in the Tools menu
	gchar       *welcome_text; // text shown when the menu item is clicked
	GtkEntry    *entry;        // text box in the plugin preferences dialog
	GeanyPlugin *plugin;       // data about our plugin known to Geany
}
demo_plugin_data;


static gboolean on_editor_notify(GObject *object, GeanyEditor *editor,
								 SCNotification *nt, gpointer data)
{
	/* Geany passes the plugin's GeanyPlugin pointer as user_data if it did not set
	 * it explicitely. This is specific to PluginCallbacks. */
	GeanyPlugin *plugin = data;
	GeanyData *geany_data = plugin->geany_data;

	/* For detailed documentation about the SCNotification struct, please see
	 * http://www.scintilla.org/ScintillaDoc.html#Notifications. */
	switch (nt->nmhdr.code)
	{
		case SCN_UPDATEUI:
			/* This notification is sent very often, you should not do time-consuming tasks here */
			break;
		case SCN_CHARADDED:
			/* For demonstrating purposes simply print the typed character in the status bar */
			ui_set_statusbar(FALSE, _("Typed character: %c"), nt->ch);
			break;
		case SCN_URIDROPPED:
		{
			/* Show a message dialog with the dropped URI list when files (i.e. a list of
			 * filenames) is dropped to the editor widget) */
			if (nt->text != NULL)
			{
				GtkWidget *dialog;

				dialog = gtk_message_dialog_new(
					GTK_WINDOW(geany_data->main_widgets->window),
					GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_MESSAGE_INFO,
					GTK_BUTTONS_OK,
					_("The following files were dropped:"));
				gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog),
					"%s", nt->text);

				gtk_dialog_run(GTK_DIALOG(dialog));
				gtk_widget_destroy(dialog);
			}
			/* we return TRUE here which prevents Geany from processing the SCN_URIDROPPED
			 * notification, i.e. Geany won't open the passed files */
			return TRUE;
		}
	}

	return FALSE;
}


static PluginCallback demo_callbacks[] =
{
	/* Set 'after' (third field) to TRUE to run the callback @a after the default handler.
	 * If 'after' is FALSE, the callback is run @a before the default handler, so the plugin
	 * can prevent Geany from processing the notification. Use this with care. */
	{ "editor-notify", (GCallback) &on_editor_notify, FALSE, NULL },
	{ NULL, NULL, FALSE, NULL }
};


/* Callback when the menu item is clicked. */
static void
item_activate(GtkMenuItem *menuitem, struct DemoPluginData *data)
{
	dialogs_show_msgbox(GTK_MESSAGE_INFO, _("%s (From the %s plugin)"),
		data->welcome_text, data->plugin->info->name);
}


/* Called by Geany to initialize the plugin */
static gboolean demo_init(GeanyPlugin *plugin, gpointer plugin_data)
{
	struct DemoPluginData *data = plugin_data;
	GeanyData *geany_data = plugin->geany_data;

	/* Add an item to the Tools menu */
	data->menu_item = gtk_menu_item_new_with_mnemonic(_("_Demo Plugin"));
	gtk_widget_show(data->menu_item);
	gtk_container_add(GTK_CONTAINER(geany_data->main_widgets->tools_menu), data->menu_item);
	g_signal_connect(data->menu_item, "activate", G_CALLBACK(item_activate), data);

	/* make the menu item sensitive only when documents are open */
	ui_add_document_sensitive(data->menu_item);

	/* set the default welcome text, can be changed by user */
	data->welcome_text = g_strdup(_("Hello World!"));

	return TRUE;
}


/* Callback connected in demo_configure(). */
static void
on_configure_response(GtkDialog *dialog, gint response, struct DemoPluginData *data)
{
	/* catch OK or Apply clicked */
	if (response == GTK_RESPONSE_OK || response == GTK_RESPONSE_APPLY)
	{
		g_free(data->welcome_text);
		data->welcome_text = g_strdup(gtk_entry_get_text(data->entry));
		/* maybe the plugin should write here the settings into a file
		 * (e.g. using GLib's GKeyFile API)
		 * all plugin specific files should be created in:
		 * geany->app->configdir G_DIR_SEPARATOR_S plugins G_DIR_SEPARATOR_S pluginname G_DIR_SEPARATOR_S
		 * e.g. this could be: ~/.config/geany/plugins/Demo/, please use geany->app->configdir */
	}
}


/* Called by Geany to show the plugin's configure dialog. This function is always called after
 * demo_init() was called.
 * You can omit this function if the plugin doesn't need to be configured.
 * Note: parent is the parent window which can be used as the transient window for the created
 *       dialog. */
static GtkWidget *demo_configure(GeanyPlugin *plugin, GtkDialog *dialog, gpointer plugin_data)
{
	struct DemoPluginData *data = plugin_data;
	GtkWidget *label, *entry, *vbox;

	/* example configuration dialog */
	vbox = gtk_vbox_new(FALSE, 6);

	/* add a label and a text entry to the dialog */
	label = gtk_label_new(_("Welcome text to show:"));
	gtk_misc_set_alignment(GTK_MISC(label), 0, 0.5);
	entry = gtk_entry_new();
	if (data->welcome_text != NULL)
		gtk_entry_set_text(GTK_ENTRY(entry), data->welcome_text);

	gtk_container_add(GTK_CONTAINER(vbox), label);
	gtk_container_add(GTK_CONTAINER(vbox), entry);

	gtk_widget_show_all(vbox);

	/* keep a pointer to the entry so we can access it from the 'response' callback */
	data->entry = GTK_ENTRY(entry);

	/* Connect a callback for when the user clicks a dialog button */
	g_signal_connect(dialog, "response", G_CALLBACK(on_configure_response), data);

	return vbox;
}


/* Called by Geany before unloading the plugin.
 * Here any UI changes should be removed, memory freed and any other finalization done.
 * Be sure to leave Geany as it was before demo_init(). */
static void demo_cleanup(GeanyPlugin *plugin, gpointer data)
{
	// We don't need this but it is required for some reason
}


/* Geany will call this function, passing our data, when the plugin is
 * unloaded. This is done by way of geany_plugin_set_data(). */
static void demo_data_free(gpointer plugin_data)
{
	struct DemoPluginData *data = plugin_data;
	gtk_widget_destroy(data->menu_item);
	g_free(data->welcome_text);
}


/* Geany calls this function when it loads our plugin's shared library module.
 * Here you tell Geany about your plugin's meta info and entry points. The
 * plugin argument points to the structure Geany uses to keep track of our
 * plugin. The module and geany_api_version are not used. */
void geany_load_module(GeanyPlugin *plugin, GModule *module, gint geany_api_version)
{
	/* main_locale_init() must be called for your package before any localization can be done */
	main_locale_init(LOCALEDIR, GETTEXT_PACKAGE);
	plugin->info->name = _("Demo");
	plugin->info->description = _("Example plugin.");
	plugin->info->version = "0.3";
	plugin->info->author =  _("The Geany developer team");

	plugin->funcs->init = demo_init;
	plugin->funcs->configure = demo_configure;
	plugin->funcs->help = NULL; /* This demo has no help but it is an option */
	plugin->funcs->cleanup = demo_cleanup; /* Geany requires this, though we don't use it */
	plugin->funcs->callbacks = demo_callbacks;

	GEANY_PLUGIN_REGISTER(plugin, 224);

	/* tell Geany about our data so it will call our free function on it
	 * whenever the plugin is unloaded and/or Geany is closed */
	demo_plugin_data.plugin = plugin;
	geany_plugin_set_data(plugin, &demo_plugin_data, demo_data_free);
}
