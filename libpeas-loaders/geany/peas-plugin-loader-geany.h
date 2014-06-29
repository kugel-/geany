/*
 * peas-plugin-loader-c.h
 * This file is part of libpeas
 *
 * Copyright (C) 2008 - Jesse van den Kieboom
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Library General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __PEAS_PLUGIN_LOADER_GEANY_H__
#define __PEAS_PLUGIN_LOADER_GEANY_H__

#include <libpeas/peas-plugin-loader.h>
#include <libpeas/peas-plugin-loader-c.h>

G_BEGIN_DECLS

#define PEAS_TYPE_PLUGIN_LOADER_GEANY            (peas_plugin_loader_geany_get_type ())
#define PEAS_PLUGIN_LOADER_GEANY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), PEAS_TYPE_PLUGIN_LOADER_GEANY, PeasPluginLoaderGeany))
#define PEAS_PLUGIN_LOADER_GEANY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), PEAS_TYPE_PLUGIN_LOADER_GEANY, PeasPluginLoaderGeanyClass))
#define PEAS_IS_PLUGIN_LOADER_GEANY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PEAS_TYPE_PLUGIN_LOADER_GEANY))
#define PEAS_IS_PLUGIN_LOADER_GEANY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PEAS_TYPE_PLUGIN_LOADER_GEANY))
#define PEAS_PLUGIN_LOADER_GEANY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), PEAS_TYPE_PLUGIN_LOADER_GEANY, PeasPluginLoaderGeanyClass))

typedef struct _PeasPluginLoaderGeany         PeasPluginLoaderGeany;
typedef struct _PeasPluginLoaderGeanyClass    PeasPluginLoaderGeanyClass;
typedef struct _PeasPluginLoaderGeanyPrivate  PeasPluginLoaderGeanyPrivate;

struct _PeasPluginLoaderGeany {
  PeasPluginLoader parent;
};

struct _PeasPluginLoaderGeanyClass {
  PeasPluginLoaderClass parent_class;
};

GType             peas_plugin_loader_geany_get_type    (void) G_GNUC_CONST;
PeasPluginLoader *peas_plugin_loader_geany_new         (void);

/* All the loaders must implement this function */
G_MODULE_EXPORT void     peas_register_types                 (PeasObjectModule *module);

G_END_DECLS

#endif /* __PEAS_PLUGIN_LOADER_GEANY_H__ */
