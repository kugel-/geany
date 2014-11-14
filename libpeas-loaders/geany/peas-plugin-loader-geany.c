/*
 * peas-plugin-loader-geanyc
 * This file is part of libpeas
 *
 * Copyright (C) 2014 - Thomas Martitz
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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include <glib.h>

#include <libpeas/peas-plugin-loader.h>
#include "peas-plugin-loader-geany.h"

#include <geanyplugin2.h>

#if 0

#define PEAS_TYPE_GEANY_OBJECT             (peas_geany_object_get_type ())
#define PEAS_GEANY_OBJECT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), PEAS_TYPE_GEANY, PeasGeanyObject))
#define PEAS_GEANY_OBJECT_CLASS(obj)       (G_TYPE_CHECK_CLASS_CAST ((obj), PEAS_TYPE_GEANY, PeasGeanyInterface))
#define PEAS_IS_GEANY_OBJECT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PEAS_TYPE_GEANY))
#define PEAS_IS_GEANY_OBJECT_CLASS(klass)  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PEAS_TYPE_GEANY))
#define PEAS_GEANY_GET_CLASS(obj)          (G_TYPE_INSTANCE_GET_INTERFACE ((obj), PEAS_TYPE_GEANY, PeasGeanyInterface))

typedef PeasGeanyData GeanyData;

/* This class is the most basic PeasGeany interface implementation.
 *
 * It acts as a proxy for the plugin provided plugin functions */

typedef struct _PeasGeanyObject {
  GObject parent;
  /* Called to verify plugin compatibility (before init()). */
  gint		(*version_check)(gint abi_ver);
  /* Called when the plugin is enabled */
  void		(*init) (GeanyData *data);
  /* plugins configure dialog, optional */
  GtkWidget*	(*configure) (GtkDialog *dialog);
  /* Called when the plugin should show some help, optional */
  void		(*help) (void);
  /* Called when the plugin is disabled or when Geany exits */
  void		(*cleanup) (void);
} PeasGeanyObject;

typedef struct _PeasGeanyObjectClass {
  GObjectClass parent;
} PeasGeanyObjectClass;

/* pgo - PeasGeanyObject */
gint pgo_version_check(PeasGeany *obj, gint abi_ver)
{
  return PEAS_GEANY_OBJECT(obj)->version_check(abi_ver);
}

void pgo_init(PeasGeany *obj, GeanyData *data)
{
  PEAS_GEANY_OBJECT(obj)->init(data);
}

GtkWidget *
pgo_configure(PeasGeany *obj, GtkDialog *dialog)
{
  PeasGeanyObject *pgo = PEAS_GEANY_OBJECT(obj);
  if (pgo->configure)
    return pgo->configure(dialog);
  else
    return NULL;
}

void
pgo_help(PeasGeany *obj)
{
  PeasGeanyObject *pgo = PEAS_GEANY_OBJECT(obj);
  if (pgo->help)
    pgo->help();
}

void
pgo_cleanup(PeasGeany *obj)
{
  PeasGeanyObject *pgo = PEAS_GEANY_OBJECT(obj);
  if (pgo->cleanup);
    pgo->cleanup();
}

int
pgo_get_methods(PeasGeany *obj)
{
  PeasGeanyObject *pgo = PEAS_GEANY_OBJECT(obj);

  gint methods = 0;

  if (pgo->configure)
    methods |= GEANY_METHODS_CONFIGURE;
  if (pgo->help)
    methods |= GEANY_METHODS_HELP
  if (pgo->cleanup)
    methods |= GEANY_METHODS_CLEANUP;

}

void
peas_geany_object_iface_init(PeasGeanyInterface *iface)
{
#define SET(x) iface->x = pgo_ ## x
  SET(version_check);
  SET(init);
  SET(provides_method);
  SET(configure);
  SET(help);
  SET(cleanup);
#undef SET
}

enum {
  PROP_0,

  PROP_METHODS,
}

static void
peas_geany_object_init(PeasGeanyObject *obj)
{
}


static void
pgo_get_property (PeasGeany    *object,
                  guint         prop_id,
                  GValue       *value,
                  GParamSpec   *pspec)
{
  PeasGeanyObject *pgo = PEAS_GEANY_OBJECT(obj);

  switch ()
}

static void
peas_geany_object_class_init(PeasGeanyObjectClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->set_property = maman_baz_set_property;
  gobject_class->get_property = maman_baz_get_property;

  g_object_class_override_property (gobject_class, PROP_NAME, "name");
}

G_DEFINE_TYPE_WITH_CODE (PeasGeanyObject, peas_geany_object, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (PEAS_TYPE_GEANY,
                                                peas_geany_object_iface_init));

#else

#include "geanypluginlegacy.h"

#endif

static void
peas_plugin_loader_geany_init (PeasPluginLoaderGeany *loader)
{
}

G_DEFINE_TYPE (PeasPluginLoaderGeany, peas_plugin_loader_geany,
               PEAS_TYPE_PLUGIN_LOADER);

static GModule *open_module_from_info(PeasPluginInfo *info)
{
  GModule *module;
  const gchar *dir = peas_plugin_info_get_module_dir(info);
  const gchar *mod = peas_plugin_info_get_module_name(info);
  const gchar *fname = g_build_filename(dir, mod, NULL);

  module = g_module_open(fname, G_MODULE_BIND_LOCAL);
  if (module == NULL)
    g_warning("Failed to load module: %s\n", g_module_error());

  return module;
}

static gboolean
peas_plugin_loader_geany_load (PeasPluginLoader *loader,
                               PeasPluginInfo   *info)
{

  /* "leak" reference here to keep the GModule open. This
   * way we avoid having to store the module pointer. _unload()
   * will balance the reference count again */
  return open_module_from_info(info) != NULL;
}

static void
peas_plugin_loader_geany_unload (PeasPluginLoader *loader,
                                 PeasPluginInfo   *info)
{
  GModule *module = open_module_from_info(info);

  /* close twice, once for the above call and once for _load() */
  g_module_close(module);
  g_module_close(module);
}


static gboolean
check_geany (PeasPluginLoader *loader, PeasPluginInfo *info)
{
  GModule          *module;
  void             *dummy;
  gboolean          ret;

  module = open_module_from_info(info);

  ret = g_module_symbol (module, "plugin_init", &dummy)
         && g_module_symbol(module, "plugin_version_check", &dummy);

  g_module_close(module);

  return ret;
}

static gboolean
peas_plugin_loader_geany_provides_extension  (PeasPluginLoader *loader,
                                              PeasPluginInfo   *info,
                                              GType             exten_type)
{
  if (exten_type == GEANY_TYPE_PLUGIN2)
    return check_geany(loader, info);

  return FALSE;
}

static PeasExtension *create_geany(PeasPluginLoader *loader,
                                   PeasPluginInfo   *info)
{
  GeanyPluginLegacy *obj;
  GModule           *module;

  module = open_module_from_info(info);

  obj = g_object_new(GEANY_TYPE_PLUGIN_LEGACY, NULL);

  g_module_symbol(module, "plugin_version_check", (void *) &obj->_version_check);
  g_module_symbol(module, "plugin_init", (void *) &obj->_init);
  g_module_symbol(module, "plugin_configure", (void *) &obj->_configure);
  g_module_symbol(module, "plugin_help", (void *) &obj->_help);
  g_module_symbol(module, "plugin_cleanup", (void *) &obj->_cleanup);
  /* transfer the module reference (don't close the module here) */
  obj->module = module;

  return PEAS_EXTENSION(obj);
}

static PeasExtension *
peas_plugin_loader_geany_create_extension (PeasPluginLoader *loader,
                                       PeasPluginInfo   *info,
                                       GType             exten_type,
                                       guint             n_parameters,
                                       GParameter       *parameters)
{
  if (exten_type == GEANY_TYPE_PLUGIN2)
    return create_geany(loader, info);

  return NULL;
}

static void
peas_plugin_loader_geany_finalize (GObject *loader)
{
  G_OBJECT_CLASS(
            peas_plugin_loader_geany_parent_class)->finalize(loader);
}

static void
peas_plugin_loader_geany_class_init (PeasPluginLoaderGeanyClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  PeasPluginLoaderClass *loader_class = PEAS_PLUGIN_LOADER_CLASS (klass);

  object_class->finalize = peas_plugin_loader_geany_finalize;

  loader_class->load = peas_plugin_loader_geany_load;
  loader_class->unload = peas_plugin_loader_geany_unload;
  loader_class->provides_extension = peas_plugin_loader_geany_provides_extension;
  loader_class->create_extension = peas_plugin_loader_geany_create_extension;
}

G_MODULE_EXPORT void
peas_register_types (PeasObjectModule *module)
{
  peas_object_module_register_extension_type (module,
                                              PEAS_TYPE_PLUGIN_LOADER,
                                              PEAS_TYPE_PLUGIN_LOADER_GEANY);
}
