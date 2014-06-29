/*
 * peas-geany.h
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

#ifndef __PEAS_GEANY_H__
#define __PEAS_GEANY_H__

#include <glib-object.h>

G_BEGIN_DECLS

/*
 * Type checking and casting macros
 */
#define PEAS_TYPE_GEANY                    (peas_geany_get_type ())
#define PEAS_GEANY(obj)                    (G_TYPE_CHECK_INSTANCE_CAST ((obj), PEAS_TYPE_GEANY, PeasGeany))
#define PEAS_GEANY_IFACE(obj)              (G_TYPE_CHECK_CLASS_CAST ((obj), PEAS_TYPE_GEANY, PeasGeanyInterface))
#define PEAS_IS_GEANY(obj)                 (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PEAS_TYPE_GEANY))
#define PEAS_GEANY_GET_IFACE(obj)          (G_TYPE_INSTANCE_GET_INTERFACE ((obj), PEAS_TYPE_GEANY, PeasGeanyInterface))

/**
 * PeasGeany:
 *
 * Interface for legacy geany plugins.
 */
typedef struct _PeasGeany           PeasGeany; /* dummy typedef */
typedef struct _PeasGeanyInterface  PeasGeanyInterface;


typedef struct _PeasGeanyData       PeasGeanyData;
typedef struct _GtkWidget           GtkWidget;
typedef struct _GtkDialog           GtkDialog;

/**
 * PeasGeanyInterface:
 * @g_iface: The parent interface.
 * @set_info: plugin_set_info.
 * @version_check: plugin_version_check.
 * @init: plugin_init.
 * @configure: plugin_configure.
 * @configure_single: plugin_configure_single.
 * @help: plugin_help.
 * @cleanup: plugin_cleanup.
 *
 * Provides an interface for activatable plugins.
 */
struct _PeasGeanyInterface {
  GTypeInterface g_iface;

  gint		(*version_check)        (PeasGeany *pg, gint abi_ver);
  void		(*init)                 (PeasGeany *pg, PeasGeanyData *data);
  GtkWidget*	(*configure)        (PeasGeany *pg, GtkDialog *dialog);
  void		(*configure_single)     (PeasGeany *pg, GtkWidget *parent);
  void		(*help)                 (PeasGeany *pg);
  void		(*cleanup)              (PeasGeany *pg);
};

/*
 * Public methods
 */
GType       peas_geany_get_type             (void)  G_GNUC_CONST;

gint        peas_geany_version_check        (PeasGeany *pg, gint abi_ver);
void        peas_geany_init                 (PeasGeany *pg, PeasGeanyData *data);
/**
 * peas_geany_configure:
 *
 * Return value: (transfer floating): New widget
 */
GtkWidget*  peas_geany_configure            (PeasGeany *pg, GtkDialog *dialog);
void        peas_geany_configure_single     (PeasGeany *pg, GtkWidget *parent);
void        peas_geany_help                 (PeasGeany *pg);
void        peas_geany_cleanup              (PeasGeany *pg);



G_END_DECLS

#endif /* __PEAS_GEANY_H__ */
