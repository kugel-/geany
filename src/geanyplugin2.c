/*
 *      geanyplugin2.c - this file is part of Geany, a fast and lightweight IDE
 *
 *      Copyright 2006-2012 Enrico Tröger <enrico(dot)troeger(at)uvena(dot)de>
 *      Copyright 2006-2012 Nick Treleaven <nick(dot)treleaven(at)btinternet(dot)com>
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
 * GeanyPlugin2Base abstract base class, GeanyPlugin2Configure and GeanyPlugin2Help implementations
 */ 

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "geanyplugin2.h"
#include "plugindata.h"
#include "pluginprivate.h"
#include "pluginutils.h"


#include "gtkcompat.h"
#include <glib.h>
#include <glib-object.h>


void
read_key_group(PluginPriv *priv, PeasPluginInfo *info)
{
	GeanyKeyGroupInfo *p_key_info;
	GeanyKeyGroup **p_key_group;
	const gchar *name = peas_plugin_info_get_name(info);

	g_module_symbol(priv->module, "plugin_key_group_info", (void *) &p_key_info);
	g_module_symbol(priv->module, "plugin_key_group", (void *) &p_key_group);
	if (p_key_info && p_key_group)
	{
		GeanyKeyGroupInfo *key_info = p_key_info;

		if (*p_key_group)
			geany_debug("Ignoring plugin_key_group symbol for plugin '%s' - "
				"use plugin_set_key_group() instead to allocate keybindings dynamically.", name);
		else
		{
			if (key_info->count)
			{
				GeanyKeyGroup *key_group =
					plugin_set_key_group(&priv->public, key_info->name, key_info->count, NULL);
				if (key_group)
					*p_key_group = key_group;
			}
			else
				geany_debug("Ignoring plugin_key_group_info symbol for plugin '%s' - "
					"count field is zero. Maybe use plugin_set_key_group() instead?", name);
		}
	}
	else if (p_key_info || p_key_group)
		geany_debug("Ignoring only one of plugin_key_group[_info] symbols defined for plugin '%s'. "
			"Maybe use plugin_set_key_group() instead?", name);
}


static void
add_callbacks(PluginPriv *priv, PluginCallback *callbacks)
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

		plugin_signal_connect(&priv->public, NULL, cb->signal_name, cb->after,
			cb->callback, cb->user_data);
	}
}


G_DEFINE_TYPE (GeanyPlugin2Base, geany_plugin2_base, PEAS_TYPE_EXTENSION_BASE)

static void
geany_plugin2_base_constructed(GObject *object)
{
	GeanyPlugin2BasePrivate *priv = GEANY_PLUGIN2_BASE_GET_PRIVATE(object);
	PluginCallback *callbacks;

	G_OBJECT_CLASS(geany_plugin2_base_parent_class)->constructed(object);

	g_module_symbol(priv->module, "plugin_callbacks", (void *) &callbacks);
	if (callbacks)
		add_callbacks(priv, callbacks);
}


static GObject *
geany_plugin2_base_constructor(GType                  gtype,
							   guint                  n_properties,
							   GObjectConstructParam *properties)
{
	GObject *obj;
	GeanyPlugin2Base *self;
	GeanyPlugin2BasePrivate *priv;
	PeasPluginInfo *info;
	gchar *fname;
	GModule *module;
	GeanyPlugin **p_geany_plugin;
	PluginInfo **p_info;
	PluginFields **plugin_fields;
	GeanyData **p_geany_data;
	GeanyFunctions **p_geany_funcs;
	gint methods, configure_mask;
	void (*p_plugin_configure_single)(GtkWidget*);

	obj = G_OBJECT_CLASS(geany_plugin2_base_parent_class)->constructor(gtype, n_properties, properties);
	self = GEANY_PLUGIN2_BASE(obj);
	info = peas_extension_base_get_plugin_info(PEAS_EXTENSION_BASE(self));

	/* The module is already loaded by libpeas, so this will just grab another reference.
	 * This cannot fail too (until we support non-C plugins) */
	fname = g_module_build_path(peas_plugin_info_get_module_dir(info),
	                            peas_plugin_info_get_module_name(info));
	module = g_module_open(fname, G_MODULE_BIND_LOCAL);

	/* Initialize Geany-private fields */
	priv = GEANY_PLUGIN2_BASE_GET_PRIVATE(self);
	priv->module = module;

	/* set these symbols before plugin_init() is called
	 * we don't set geany_functions and geany_data since they are set directly by plugin_new() */
	g_module_symbol(module, "geany_plugin", (void *) &p_geany_plugin);
	if (p_geany_plugin)
		*p_geany_plugin = &priv->public;

	g_module_symbol(module, "plugin_info", (void *) &p_info);
	if (p_info)
		*p_info = &priv->info;

	g_module_symbol(module, "plugin_fields", (void *) &plugin_fields);
	if (plugin_fields)
		*plugin_fields = &priv->fields;

	g_module_symbol(module, "geany_data", (void *) &p_geany_data);
	if (p_geany_data)
		*p_geany_data = geany_get_data();

	g_module_symbol(module, "geany_functions", (void *) &p_geany_funcs);
	if (p_geany_funcs)
		*p_geany_funcs = geany_get_funcs();

	/* Read deprecated stuff.
	 * plugin_configure_single should be phased out soon */
	g_module_symbol(module, "plugin_configure_single", (void *) &p_plugin_configure_single);
	if (p_plugin_configure_single)
		priv->configure_single = p_plugin_configure_single;


	/* We do not execute plugin_set_info() anymore. Instead we exclusively rely on the metadata
	 * provided by the plugin's .plugin file. The fields are set anyway for compatibility */
	priv->info.name = peas_plugin_info_get_name(info);
	priv->info.description = peas_plugin_info_get_description(info);
	priv->info.version = peas_plugin_info_get_version(info);
	priv->info.author = g_strjoinv(", ", (gchar **)peas_plugin_info_get_authors(info));

	read_key_group(priv, info);

	return obj;
}

static void
geany_plugin2_base_finalize(GObject *object)
{
	GeanyPlugin2BasePrivate *priv = GEANY_PLUGIN2_BASE_GET_PRIVATE(object);
	PeasPluginInfo *info = peas_extension_base_get_plugin_info(PEAS_EXTENSION_BASE(object));

	G_OBJECT_CLASS(geany_plugin2_base_parent_class)->finalize(object);

	g_module_close(priv->module);
	g_free((gpointer) priv->info.author);
	g_free(priv);
}


static void
geany_plugin2_base_init(GeanyPlugin2Base *self)
{
	self->priv = g_new0(GeanyPlugin2BasePrivate, 1);

	self->priv->public.info = &self->priv->info;
	self->priv->public.priv = self;
}


static void
geany_plugin2_base_class_init(GeanyPlugin2BaseClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

	object_class->constructor = geany_plugin2_base_constructor;
	object_class->finalize = geany_plugin2_base_finalize;
	object_class->constructed = geany_plugin2_base_constructed;
}


GeanyPlugin2Base *
geany_plugin2_base_new(void)
{
	return g_object_new(GEANY_TYPE_PLUGIN2_BASE, NULL);
}


static void geany_plugin2_default_init(GeanyPlugin2Interface *iface) { }

G_DEFINE_INTERFACE(GeanyPlugin2, geany_plugin2, GEANY_TYPE_PLUGIN2_BASE)


/* Default implementation */
void
geany_plugin2_cleanup_default(GeanyPlugin2 *self)
{
}

static void
geany_plugin2_init(GeanyPlugin2Interface *iface)
{
	static gboolean initialized = FALSE;

	iface->cleanup = geany_plugin2_cleanup_default;

	if (!initialized)
	{
		initialized = TRUE;
	}
}


void
geany_plugin2_cleanup(GeanyPlugin2 *self)
{
	g_return_if_fail(GEANY_PLUGIN2(self));
	return GEANY_PLUGIN2_GET_INTERFACE(self)->cleanup(self);
}


static void geany_plugin2_configure_default_init(GeanyPlugin2ConfigureInterface *iface) { }

G_DEFINE_INTERFACE(GeanyPlugin2Configure, geany_plugin2_configure, 0)

/* Default implementation */
static GtkWidget *
geany_plugin2_configure_get_config_widget_default(GeanyPlugin2Configure *self, GtkDialog *parent)
{
	g_warn_if_reached();
	return NULL;
}

static void
geany_plugin2_configure_init(GeanyPlugin2ConfigureInterface *iface)
{
	static gboolean initialized = FALSE;

	iface->get_config_widget = geany_plugin2_configure_get_config_widget_default;

	if (!initialized)
	{
		initialized = TRUE;
	}
}


GtkWidget *
geany_plugin2_configure_get_config_widget(GeanyPlugin2Configure *self, GtkDialog *dialog)
{
	g_return_val_if_fail (GEANY_PLUGIN2_CONFIGURE (self), NULL);
	return GEANY_PLUGIN2_CONFIGURE_GET_INTERFACE (self)->get_config_widget (self, dialog);
}

static void geany_plugin2_help_default_init(GeanyPlugin2HelpInterface *iface) { }

G_DEFINE_INTERFACE(GeanyPlugin2Help, geany_plugin2_help, 0)

/* Default implementation */
void
geany_plugin2_help_provide_help_default(GeanyPlugin2Help *self)
{
	g_warn_if_reached ();
}

static void
geany_plugin2_help_init(GeanyPlugin2HelpInterface *iface)
{
	static gboolean initialized = FALSE;

	iface->provide_help = geany_plugin2_help_provide_help_default;

	if (!initialized)
	{
		initialized = TRUE;
	}
}

void
geany_plugin2_help_provide_help(GeanyPlugin2Help *self)
{
	g_return_if_fail(GEANY_PLUGIN2_HELP(self));
	GEANY_PLUGIN2_HELP_GET_INTERFACE(self)->provide_help(self);
}
