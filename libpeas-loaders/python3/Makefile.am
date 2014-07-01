# Python 3 plugin loader

loaderdir = $(libdir)/libpeas-1.0/loaders

AM_CPPFLAGS = \
	-I$(top_srcdir)			\
	@GLIB_CFLAGS@			\
	@GIR_CFLAGS@			\
	$(WARN_CFLAGS)			\
	$(DISABLE_DEPRECATED)		\
	$(PYGOBJECT_CFLAGS)		\
	$(PYTHON3_CFLAGS)		\
	-DPEAS_PYEXECDIR=\""$(PYTHON3_PYEXECDIR)"\" \
	-DPEAS_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

loader_LTLIBRARIES = libpython3loader.la

libpython3loader_la_SOURCES = \
	$(top_srcdir)/libpeas-loaders/python/peas-plugin-loader-python.c	\
	$(top_srcdir)/libpeas-loaders/python/peas-plugin-loader-python.h

libpython3loader_la_LDFLAGS = \
	$(LOADER_LIBTOOL_FLAGS)		\
	$(PYTHON3_LDFLAGS)

libpython3loader_la_LIBADD = \
	@GLIB_CFLAGS@			\
	$(PYGOBJECT_LIBS)		\
	$(PYTHON3_LIBS)
