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
#include <libpeas/peas-plugin-loader-c.h>

#include <libpeas/peas-object-module.h>
#include <libpeas/peas-geany.h>

#include "peas-plugin-loader-geany.h"

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
  /* plugin configure dialog, optional */
  void		(*configure_single) (GtkWidget *parent);
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

gboolean
pgo_provides_method(PeasGeany *obj, int which)
{
  PeasGeanyObject *pgo = PEAS_GEANY_OBJECT(obj);
  switch (which)
  {
    case PEAS_GEANY_CONFIGURE:
      return pgo->configure != NULL;
    case PEAS_GEANY_CONFIGURE_SINGLE:
      return pgo->configure_single != NULL;
    case PEAS_GEANY_HELP:
      return pgo->help != NULL;
    case PEAS_GEANY_CLEANUP:
      return pgo->cleanup != NULL;
    default:
      return TRUE;
  }
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
pgo_configure_single(PeasGeany *obj, GtkWidget *parent)
{
  PeasGeanyObject *pgo = PEAS_GEANY_OBJECT(obj);
  if (pgo->configure_single)
    pgo->configure_single(parent);
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

void
peas_geany_object_iface_init(PeasGeanyInterface *iface)
{
#define SET(x) iface->x = pgo_ ## x
  SET(version_check);
  SET(init);
  SET(provides_method);
  SET(configure);
  SET(configure_single);
  SET(help);
  SET(cleanup);
#undef SET
}

static void
peas_geany_object_init(PeasGeanyObject *obj)
{
}

static void
peas_geany_object_class_init(PeasGeanyObjectClass *klass)
{
}

G_DEFINE_TYPE_WITH_CODE (PeasGeanyObject, peas_geany_object, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (PEAS_TYPE_GEANY,
                                                peas_geany_object_iface_init));


static GModule *open_module_from_info(PeasPluginInfo *info)
{
  const gchar *dir = peas_plugin_info_get_module_dir(info);
  const gchar *mod = peas_plugin_info_get_module_name(info);
  const gchar *fname = g_build_filename(dir, mod, NULL);

  return g_module_open(fname, G_MODULE_BIND_LOCAL);
}

static void
peas_plugin_loader_geany_init (PeasPluginLoaderGeany *loader)
{
}

G_DEFINE_TYPE (PeasPluginLoaderGeany, peas_plugin_loader_geany,
               PEAS_TYPE_PLUGIN_LOADER);

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
  if (exten_type == PEAS_TYPE_GEANY)
    return check_geany(loader, info);

  return FALSE;
}

static PeasExtension *create_geany(PeasPluginLoader *loader,
                                   PeasPluginInfo   *info)
{
  PeasGeanyObject  *obj;
  GModule          *module;

  module = open_module_from_info(info);
  
  obj = g_object_new(PEAS_TYPE_GEANY_OBJECT, NULL);

  g_module_symbol(module, "plugin_version_check", (void *) &obj->version_check);
  g_module_symbol(module, "plugin_init", (void *) &obj->init);
  g_module_symbol(module, "plugin_configure", (void *) &obj->configure);
  g_module_symbol(module, "plugin_configure_single", (void *) &obj->configure_single);
  g_module_symbol(module, "plugin_help", (void *) &obj->help);
  g_module_symbol(module, "plugin_cleanup", (void *) &obj->cleanup);

  g_module_close(module);

  return PEAS_EXTENSION(obj);
}

static PeasExtension *
peas_plugin_loader_geany_create_extension (PeasPluginLoader *loader,
                                       PeasPluginInfo   *info,
                                       GType             exten_type,
                                       guint             n_parameters,
                                       GParameter       *parameters)
{
  if (exten_type == PEAS_TYPE_GEANY)
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
