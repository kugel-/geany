/*
 * peas-activatable.c
 * This file is part of libpeas
 *
 * Copyright (C) 2010 Steve Frécinaux
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

#include "peas-geany.h"

/**
 * SECTION:peas-geany
 * @short_description: Interface for geany plugins.
 * @see_also: #PeasExtensionSet
 *
 * #PeasGeany is an interface which should be implemented by plugins
 * that should be activated on an object of a certain type (depending on the
 * application). For instance, in a typical windowed application,
 * #PeasGeany plugin instances could be bound to individual toplevel
 * windows.
 *
 * It is typical to use #PeasGeany along with #PeasExtensionSet in order
 * to activate and deactivate extensions automatically when plugins are loaded
 * or unloaded.
 *
 * You can also use the code of this interface as a base for your own
 * extension types, as illustrated by gedit's %GeditWindowActivatable and
 * %GeditDocumentActivatable interfaces.
 **/

G_DEFINE_INTERFACE(PeasGeany, peas_geany, G_TYPE_OBJECT)

void
peas_geany_default_init (PeasGeanyInterface *iface)
{
}

#define GET_IFACE(x, fn)                  \
  ({g_return_if_fail (PEAS_IS_GEANY (x));   \
  iface = PEAS_GEANY_GET_IFACE (x);       \
  g_return_if_fail (iface->fn != NULL); iface;})

gint
peas_geany_version_check (PeasGeany *obj, gint abi_ver)
{
  PeasGeanyInterface *iface;

  iface = GET_IFACE(obj, version_check);

  return iface->version_check (obj, abi_ver);
}

void
peas_geany_init (PeasGeany *obj, PeasGeanyData *data)
{
  PeasGeanyInterface *iface;

  iface = GET_IFACE(obj, init);

  iface->init(obj, data);
}

gboolean
peas_geany_provides_method (PeasGeany *obj, int which)
{
  PeasGeanyInterface *iface;

  iface = GET_IFACE(obj, init);

  return iface->provides_method(obj, which);
}

GtkWidget *
peas_geany_configure (PeasGeany *obj, GtkDialog *dialog)
{
  PeasGeanyInterface *iface;

  iface = GET_IFACE(obj, configure);

  iface->configure(obj, dialog);
}

void
peas_geany_configure_single (PeasGeany *obj, GtkWidget *parent)
{
  PeasGeanyInterface *iface;

  iface = GET_IFACE(obj, configure_single);

  iface->configure_single(obj, parent);
}

void
peas_geany_help (PeasGeany *obj)
{
  PeasGeanyInterface *iface;

  iface = GET_IFACE(obj, help);

  iface->help(obj);
}

void
peas_geany_cleanup (PeasGeany *obj)
{
  PeasGeanyInterface *iface;

  iface = GET_IFACE(obj, cleanup);

  iface->cleanup(obj);
}
