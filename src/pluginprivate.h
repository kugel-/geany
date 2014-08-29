/*
 *      pluginprivate.h - this file is part of Geany, a fast and lightweight IDE
 *
 *      Copyright 2009-2012 Nick Treleaven <nick(dot)treleaven(at)btinternet(dot)com>
 *      Copyright 2009-2012 Enrico Tröger <enrico(dot)troeger(at)uvena(dot)de>
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


#ifndef GEANY_PLUGIN_PRIVATE_H
#define GEANY_PLUGIN_PRIVATE_H 1

#include "geanyplugin2.h"
#include "plugindata.h"
#include "ui_utils.h"	/* GeanyAutoSeparator */
#include "keybindings.h"	/* GeanyKeyGroup */

#include "gtkcompat.h"
#include <libpeas/peas.h>


G_BEGIN_DECLS

typedef struct SignalConnection
{
	GObject	*object;
	gulong	handler_id;
}
SignalConnection;

#define GEANY_PLUGIN2_BASE_GET_PRIVATE(obj)  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GEANY_TYPE_PLUGIN2_BASE, GeanyPlugin2Base)->priv)
#define PLUGIN_GET_PRIVATE(obj)              (GEANY_PLUGIN2_BASE_GET_PRIVATE(obj))

#define GEANYPLUGIN_TO_PRIV(p)               ((GeanyPlugin2BasePrivate *)(p))

/* shortcuts */
typedef struct _GeanyPlugin2Base        Plugin;
typedef struct _GeanyPlugin2BasePrivate PluginPriv;

struct _GeanyPlugin2BasePrivate
{
	/* This still contains public fields, but we should deprecate it soon in favor of
	 * renaming GeanyPlugin2 interace to GeanyPlugin. This is also first so that
	 * pluginutils.c can cast from it to PluginPriv directly */
	GeanyPlugin          public;
	GModule             *module;			/* GModule loaded for the plugin */
	/* extra stuff */
	PluginInfo           info;
	PluginFields         fields;
	GeanyKeyGroup       *key_group;
	GeanyAutoSeparator   toolbar_separator;
	GArray              *signal_ids;			/* SignalConnection's to disconnect when unloading */
	GList               *sources;				/* GSources to destroy when unloading */

	GeanyPlugin2Configure *(*new_configure)(GeanyPlugin2Base *plugin);
	GeanyPlugin2Help      *(*new_help)(GeanyPlugin2Base *plugin);

	/* compat hooks from the old interface */
	void (*configure_single)(GtkWidget *parent);
};

void plugin_watch_object(Plugin *plugin, gpointer object);

void plugins_configure(Plugin *current);

GeanyData *geany_get_data(void);
GeanyFunctions *geany_get_funcs(void);

G_END_DECLS

#endif /* GEANY_PLUGIN_PRIVATE_H */
