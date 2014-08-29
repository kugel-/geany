#ifndef __GEANY_PLUGIN2_H__
#define __GEANY_PLUGIN2_H__

#include "gtkcompat.h"
#include <glib-object.h>
#include <libpeas/peas.h>

G_BEGIN_DECLS

#define GEANY_TYPE_PLUGIN2_BASE			(geany_plugin2_base_get_type ())
#define GEANY_PLUGIN2_BASE(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GEANY_TYPE_PLUGIN2_BASE, GeanyPlugin2Base))
#define GEANY_PLUGIN2_BASE_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GEANY_TYPE_PLUGIN2_BASE, GeanyPlugin2Base const))
#define GEANY_PLUGIN2_BASE_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GEANY_TYPE_PLUGIN2_BASE, GeanyPlugin2BaseClass))
#define GEANY_IS_PLUGIN2_BASE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GEANY_TYPE_PLUGIN2_BASE))
#define GEANY_IS_PLUGIN2_BASE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GEANY_TYPE_PLUGIN2_BASE))
#define GEANY_PLUGIN2_BASE_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GEANY_TYPE_PLUGIN2_BASE, GeanyPlugin2BaseClass))

typedef struct _GeanyPlugin2Base            GeanyPlugin2Base;
typedef struct _GeanyPlugin2BaseClass       GeanyPlugin2BaseClass;
typedef struct _GeanyPlugin2BasePrivate     GeanyPlugin2BasePrivate;

struct _GeanyPlugin2Base
{
	PeasExtensionBase        parent;

	GeanyPlugin2BasePrivate *priv;
};

struct _GeanyPlugin2BaseClass
{
	PeasExtensionBaseClass parent_class;
};

GType geany_plugin2_base_get_type (void) G_GNUC_CONST;
GeanyPlugin2Base *geany_plugin2_base_new (void);

#define GEANY_TYPE_PLUGIN2			(geany_plugin2_get_type ())
#define GEANY_PLUGIN2(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GEANY_TYPE_PLUGIN2, GeanyPlugin2))
#define GEANY_IS_PLUGIN2(obj)			(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GEANY_TYPE_PLUGIN2))
#define GEANY_PLUGIN2_GET_INTERFACE(obj)	(G_TYPE_INSTANCE_GET_INTERFACE ((obj), GEANY_TYPE_PLUGIN2, GeanyPlugin2Interface))

typedef struct _GeanyPlugin2		GeanyPlugin2;
typedef struct _GeanyPlugin2Interface	GeanyPlugin2Interface;

struct _GeanyPlugin2Interface
{
	GTypeInterface parent;

	void (*cleanup)(GeanyPlugin2 *self);
};

GType geany_plugin2_get_type (void) G_GNUC_CONST;

void geany_plugin2_cleanup (GeanyPlugin2 *self);

#define GEANY_TYPE_PLUGIN2_CONFIGURE			(geany_plugin2_configure_get_type ())
#define GEANY_PLUGIN2_CONFIGURE(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GEANY_TYPE_PLUGIN2_CONFIGURE, GeanyPlugin2Configure))
#define GEANY_IS_PLUGIN2_CONFIGURE(obj)			(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GEANY_TYPE_PLUGIN2_CONFIGURE))
#define GEANY_PLUGIN2_CONFIGURE_GET_INTERFACE(obj)	(G_TYPE_INSTANCE_GET_INTERFACE ((obj), GEANY_TYPE_PLUGIN2_CONFIGURE, GeanyPlugin2ConfigureInterface))

typedef struct _GeanyPlugin2Configure		GeanyPlugin2Configure;
typedef struct _GeanyPlugin2ConfigureInterface	GeanyPlugin2ConfigureInterface;

struct _GeanyPlugin2ConfigureInterface
{
	GTypeInterface parent;

	GtkWidget* (*get_config_widget)(GeanyPlugin2Configure *self, GtkDialog *parent);
};

GType geany_plugin2_configure_get_type (void) G_GNUC_CONST;

GtkWidget *geany_plugin2_configure_get_config_widget(GeanyPlugin2Configure *self, GtkDialog *parent);

#define GEANY_TYPE_PLUGIN2_HELP			(geany_plugin2_help_get_type ())
#define GEANY_PLUGIN2_HELP(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GEANY_TYPE_PLUGIN2_HELP, GeanyPlugin2Help))
#define GEANY_IS_PLUGIN2_HELP(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GEANY_TYPE_PLUGIN2_HELP))
#define GEANY_PLUGIN2_HELP_GET_INTERFACE(obj)	(G_TYPE_INSTANCE_GET_INTERFACE ((obj), GEANY_TYPE_PLUGIN2_HELP, GeanyPlugin2HelpInterface))

typedef struct _GeanyPlugin2Help	GeanyPlugin2Help;
typedef struct _GeanyPlugin2HelpInterface	GeanyPlugin2HelpInterface;

struct _GeanyPlugin2HelpInterface
{
	GTypeInterface parent;

	void (*provide_help) (GeanyPlugin2Help *self);
};

GType geany_plugin2_help_get_type (void) G_GNUC_CONST;

void geany_plugin2_help_provide_help (GeanyPlugin2Help *self);

G_END_DECLS

#endif /* __GEANY_PLUGIN2_H__ */
