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


typedef struct GeanyPluginPrivate
{
	GeanyPlugin2	*object;
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
static Plugin *geany_plugin2_set_priv(GeanyPlugin2 *self, Plugin *priv)
{
	g_object_set_data(G_OBJECT(self), PRIV_FIELD, priv);
}

static Plugin *geany_plugin2_get_priv(GeanyPlugin2 *self)
{
	return g_object_get_data(G_OBJECT(self), PRIV_FIELD);
}

void plugin_watch_object(Plugin *plugin, gpointer object);

extern GHashTable *active_plugins;

/* quite an ugly macro, it is this way because you can declare different types in a for-loops
 * initialization expression */
#define foreach_active_plugin(obj, info)                                                       \
	for (GeanyPlugin2 *obj = NULL, **_d0 = NULL; ! _d0; _d0 = &obj)                            \
		for (const GList *node = peas_engine_get_plugin_list(peas_engine_get_default()), **_d1 = NULL; ! _d1; _d1 = &node)  \
			for (PeasPluginInfo *info = NULL;                                                  \
				node && (info = node->data);                                                   \
				node = g_list_next(node))                                                      \
		if (NULL == ((obj) = g_hash_table_lookup(active_plugins, info)))                       \
			continue;                                                                          \
		else



G_END_DECLS

#endif /* GEANY_PLUGIN_PRIVATE_H */
