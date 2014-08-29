#include "config.h"

#include "geanyplugin.h"
#include <geanyplugin2.h>

#include <glib.h>
#include <glib-object.h>
#include <libpeas/peas.h>

static const gchar *gtype_prefix;

enum
{
	PROP_0,

	PROP_METHODS,
	NUM_PROPERTIES,
};

static GParamSpec *props[NUM_PROPERTIES];

typedef struct _PluginBase	PluginBase;
typedef struct _PluginBaseClass	PluginBaseClass;

static GObjectClass *plugin_base_parent_class;

struct _PluginBase
{
	GeanyPlugin2Base parent_instance;
};

struct _PluginBaseClass
{
	GeanyPlugin2BaseClass parent_class;
};

static GType plugin_base_get_type (void) G_GNUC_CONST;

/* Enforce this symbol to be exported even if the original plugin code doesn't. Needed in
 * our constructed hook */
G_MODULE_EXPORT
GeanyData *geany_data;

static gint version_check_wrapper(GeanyPlugin2 *self, gint abi_ver)
{
	return plugin_version_check(abi_ver);
}

static GtkWidget *(*p_plugin_configure)(GtkDialog *dialog);
static GtkWidget *configure_wrapper(GeanyPlugin2Configure *self, GtkDialog *dialog)
{
	return p_plugin_configure(dialog);
}

static void (*p_plugin_help)(void);
static void help_wrapper(GeanyPlugin2Help *self)
{
	p_plugin_help();
}

static void (*p_plugin_cleanup)(void);
static void cleanup_wrapper(GeanyPlugin2 *self)
{
	if (p_plugin_cleanup)
		p_plugin_cleanup();
}

/* Must use constructed hook, because Geany sets up our pointers in its constructor. */
static void
plugin_base_constructed (GObject *object)
{
	/* geany_data is set by Geany. */
	plugin_init(geany_data);
	/* Must be done after plugin_init(). */
	G_OBJECT_CLASS(plugin_base_parent_class)->constructed(object);
}


static void
plugin_base_finalize(GObject *object)
{
	G_OBJECT_CLASS(plugin_base_parent_class)->finalize(object);
}


static void
plugin_base_class_init(PluginBaseClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	plugin_base_parent_class = (GObjectClass *) g_type_class_peek_parent((gpointer) object_class);

	object_class->constructed = plugin_base_constructed;
	object_class->finalize = plugin_base_finalize;
}


static void
plugin_base_init (PluginBase *self) { }


static void
plugin_base_geany_plugin2_iface_init(GeanyPlugin2Interface *iface)
{
	iface->cleanup = cleanup_wrapper;
}

static const GInterfaceInfo geany_plugin2_info =
{
	(GInterfaceInitFunc) plugin_base_geany_plugin2_iface_init,
	(GInterfaceFinalizeFunc) NULL, NULL
};

static void
plugin_base_geany_plugin2_configure_iface_init(GeanyPlugin2ConfigureInterface *iface)
{
	iface->get_config_widget = configure_wrapper;
}

static const GInterfaceInfo geany_plugin2_configure_info =
{
	(GInterfaceInitFunc) plugin_base_geany_plugin2_configure_iface_init,
	(GInterfaceFinalizeFunc) NULL, NULL
};


static void
plugin_base_geany_plugin2_help_iface_init(GeanyPlugin2HelpInterface *iface)
{
	iface->provide_help = help_wrapper;
}


static const GInterfaceInfo geany_plugin2_help_info =
{
	(GInterfaceInitFunc) plugin_base_geany_plugin2_help_iface_init,
	(GInterfaceFinalizeFunc) NULL, NULL
};

static GType plugin_base_get_type (void)
{
	static volatile gsize plugin_base_type_id__volatile = 0;

	if (g_once_init_enter (&plugin_base_type_id__volatile))
	{
		GType plugin_base_type_id = g_type_register_static_simple(GEANY_TYPE_PLUGIN2_BASE,
		                                                          gtype_prefix,
		                                                          sizeof(PluginBaseClass),
		                                                          (GClassInitFunc) plugin_base_class_init,
		                                                          sizeof(PluginBase),
		                                                          (GInstanceInitFunc) plugin_base_init, 0);
		g_type_add_interface_static (plugin_base_type_id, GEANY_TYPE_PLUGIN2, &geany_plugin2_info);
		g_once_init_leave (&plugin_base_type_id__volatile, plugin_base_type_id);
	}

	return plugin_base_type_id__volatile;
}

G_MODULE_EXPORT void
peas_register_types (PeasObjectModule* module)
{
	GModule *mod;
	gboolean has_plugin_init, has_plugin_version_check;
	GType plugin_base_type;
	const gchar *modname = peas_object_module_get_module_name(module);
	gchar *lib;
	static volatile gsize plugin_base_type_id__volatile = 0;

	if (g_once_init_enter (&plugin_base_type_id__volatile))
	{
		/* gtype_prefix must be set first. this is never freed as it should be static to GLib */
		gtype_prefix = g_strdup_printf("%sPluginBase", modname);
		plugin_base_type = plugin_base_get_type();
		/* fetch optional functions for the additional interfaces */
		lib = g_module_build_path(peas_object_module_get_path(module),
								  peas_object_module_get_module_name(module));
		mod = g_module_open(lib, G_MODULE_BIND_LAZY | G_MODULE_BIND_LOCAL);

		if (g_module_symbol(mod, "plugin_configure", (void **)&p_plugin_configure))
			g_type_add_interface_static(plugin_base_type, GEANY_TYPE_PLUGIN2_CONFIGURE, &geany_plugin2_configure_info);

		if (g_module_symbol(mod, "plugin_help", (void **)&p_plugin_help))
			g_type_add_interface_static(plugin_base_type, GEANY_TYPE_PLUGIN2_HELP, &geany_plugin2_help_info);

		if (!g_module_symbol(mod, "plugin_cleanup", (void **)&p_plugin_cleanup))
			g_warning("Plugin '%s' has no plugin_cleanup() function - there may be memory leaks!", modname);

		g_module_close(mod);
		g_free(lib);

		g_once_init_leave(&plugin_base_type_id__volatile, plugin_base_type);
	}
	peas_object_module_register_extension_type(module, GEANY_TYPE_PLUGIN2, plugin_base_type_id__volatile);
}
