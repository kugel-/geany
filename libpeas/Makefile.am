libpeasdir = $(libdir)/geany/libpeas-1.0
libpeas_LTLIBRARIES = libpeas-1.0.la

AM_CPPFLAGS =				\
	-I$(top_srcdir)			\
	-I$(srcdir)			\
	$(PEAS_CFLAGS)			\
	$(IGE_MAC_CFLAGS)		\
	$(WARN_CFLAGS)			\
	$(DISABLE_DEPRECATED)		\
	-DDATADIR=\""$(datadir)"\"	\
	-DLIBDIR=\""$(libdir)/geany"\" \
	@GLIB_CFLAGS@ \
	@GIR_CFLAGS@

libpeas_1_0_la_LDFLAGS = \
	-export-dynamic -no-undefined -export-symbols-regex "^[^_].*"

libpeas_1_0_la_LIBADD = $(PEAS_LIBS) $(IGE_MAC_LIBS)

INST_H_FILES =			\
	peas-plugin-info.h	\
	peas-object-module.h	\
	peas-extension-base.h	\
	peas-extension.h	\
	peas-extension-set.h	\
	peas-activatable.h	\
	peas-engine.h		\
	peas.h

NOINST_H_FILES =			\
	peas-debug.h			\
	peas-dirs.h			\
	peas-engine-priv.h		\
	peas-extension-wrapper.h	\
	peas-extension-subclasses.h	\
	peas-helpers.h			\
	peas-i18n.h			\
	peas-introspection.h		\
	peas-plugin-info-priv.h		\
	peas-plugin-loader.h		\
	peas-plugin-loader-c.h

C_FILES =				\
	peas-activatable.c		\
	peas-debug.c			\
	peas-dirs.c			\
	peas-engine.c			\
	peas-extension.c		\
	peas-extension-base.c		\
	peas-extension-set.c		\
	peas-extension-wrapper.c	\
	peas-extension-subclasses.c	\
	peas-helpers.c			\
	peas-i18n.c			\
	peas-introspection.c		\
	peas-object-module.c		\
	peas-plugin-info.c		\
	peas-plugin-loader.c		\
	peas-plugin-loader-c.c

BUILT_SOURCES = \
	peas-marshal.c			\
	peas-marshal.h

libpeas_1_0_la_SOURCES = 	\
	$(INST_H_FILES)		\
	$(NOINST_H_FILES)	\
	$(BUILT_SOURCES)	\
	$(C_FILES)

headerdir = $(includedir)/geany/libpeas-1.0
header_DATA = $(INST_H_FILES)

peas-marshal.c: peas-marshal.list $(GLIB_GENMARSHAL)
	$(AM_V_GEN) (echo "#include \"peas-marshal.h\"" && $(GLIB_GENMARSHAL) $< --body --prefix=peas_cclosure_marshal) > $@

peas-marshal.h: peas-marshal.list $(GLIB_GENMARSHAL)
	$(AM_V_GEN) $(GLIB_GENMARSHAL) $< --header --prefix=peas_cclosure_marshal > $@

EXTRA_DIST = peas-marshal.list

CLEANFILES = $(BUILT_SOURCES)

if ENABLE_VAPIGEN
  -include $(VAPIGEN_MAKEFILE)
  libpeas-1.0.vapi: Peas-1.0.gir Peas-1.0.metadata libpeas-1.0.deps

  VAPIGEN_VAPIS = libpeas-1.0.vapi

  libpeas_1_0_vapi_DEPS = gio-2.0 gobject-introspection-1.0
  libpeas_1_0_vapi_GIRDIRS = $(srcdir)
  libpeas_1_0_vapi_METADATADIRS = $(srcdir)
  libpeas_1_0_vapi_FILES = Peas-1.0.gir

  vapidir = $(datadir)/vala/vapi
  vapi_DATA = $(VAPIGEN_VAPIS) $(VAPIGEN_VAPIS:.vapi=.deps)

  EXTRA_DIST += Peas-1.0.metadata libpeas-1.0.deps
  CLEANFILES += $(VAPIGEN_VAPIS)
endif

dist-hook:
	cd $(distdir); rm -f $(BUILT_SOURCES)

if HAVE_INTROSPECTION
  -include $(INTROSPECTION_MAKEFILE)
  INTROSPECTION_GIRS = Peas-1.0.gir
  INTROSPECTION_SCANNER_ARGS = \
	-I$(top_srcdir) 		\
	--c-include=libpeas/peas.h	\
	--warn-all

  introspection_sources = $(INST_H_FILES) $(C_FILES)

  Peas-1.0.gir: libpeas-1.0.la
  Peas_1_0_gir_INCLUDES = GObject-2.0 GModule-2.0 Gio-2.0 GIRepository-2.0
  Peas_1_0_gir_CFLAGS = $(PEAS_CFLAGS)
  Peas_1_0_gir_LIBS = libpeas-1.0.la
  Peas_1_0_gir_FILES = $(addprefix $(srcdir)/,$(introspection_sources))
  Peas_1_0_gir_EXPORT_PACKAGES = libpeas-1.0

  girdir = $(libpeasdir)
  gir_DATA = $(INTROSPECTION_GIRS)

  typelibdir = $(libpeasdir)
  typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

  CLEANFILES += \
	$(gir_DATA)		\
	$(typelib_DATA)
endif
