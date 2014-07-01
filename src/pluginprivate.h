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

#include "plugindata.h"
#include "ui_utils.h"	/* GeanyAutoSeparator */
#include "keybindings.h"	/* GeanyKeyGroup */

#include "peas-geany.h"
#include "peas-plugin-info.h"

#include "gtkcompat.h"


G_BEGIN_DECLS

typedef struct SignalConnection
{
	GObject	*object;
	gulong	handler_id;
}
SignalConnection;


typedef struct GeanyPluginPrivate
{
	PeasGeany		*object;
	PeasPluginInfo	*peas_info;
	PluginInfo		info;				/* plugin name, description, etc */
	GeanyPlugin		public;				/* fields the plugin can read */
	GModule 		*module;			/* GModule loaded for the plugin */

	/* extra stuff */
	PluginFields	fields;
	GeanyKeyGroup	*key_group;
	GeanyAutoSeparator	toolbar_separator;
	GArray			*signal_ids;			/* SignalConnection's to disconnect when unloading */
	GList			*sources;				/* GSources to destroy when unloading */
}
GeanyPluginPrivate;

typedef GeanyPluginPrivate Plugin;	/* shorter alias */

#define PRIV_FIELD "__priv"
/* store this in a private (TODO: _set_qdata() ?) */
static Plugin *peas_geany_set_priv(PeasGeany *this, Plugin *priv)
{
	g_object_set_data(G_OBJECT(this), PRIV_FIELD, priv);
}

static Plugin *peas_geany_get_priv(PeasGeany *this)
{
	return g_object_get_data(G_OBJECT(this), PRIV_FIELD);
}

void plugin_watch_object(Plugin *plugin, gpointer object);

extern GHashTable *active_plugins;

#define foreach_active_plugin(obj, info)                                                       \
	for (PeasGeany *obj = NULL; 0;)                                                            \
		for (const GList *node = peas_engine_get_plugin_list(peas_engine_get_default()); 0;)   \
			for (PeasPluginInfo *info = NULL;                                                  \
				node && (info = node->data);                                                   \
				node = g_list_next(node))                                                      \
		if (NULL == ((obj) = g_hash_table_lookup(active_plugins, info)))                       \
			continue;                                                                          \
		else



G_END_DECLS

#endif /* GEANY_PLUGIN_PRIVATE_H */
