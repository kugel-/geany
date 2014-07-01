# Python plugin loader

loaderdir = $(libdir)/geany/libpeas-1.0/loaders

AM_CPPFLAGS = \
	-I$(top_srcdir)			\
	@GLIB_CFLAGS@			\
	@GIR_CFLAGS@			\
	$(WARN_CFLAGS)			\
	$(DISABLE_DEPRECATED)		\
	$(PYGOBJECT_CFLAGS)		\
	$(PYTHON2_CFLAGS)		\
	-DPEAS_PYEXECDIR=\""$(PYTHON2_PYEXECDIR)"\" \
	-DPEAS_LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"

loader_LTLIBRARIES = libpythonloader.la

libpythonloader_la_SOURCES = \
	peas-plugin-loader-python.c 	\
	peas-plugin-loader-python.h

libpythonloader_la_LDFLAGS = \
	-Wl,-rpath,$(libdir)/geany/libpeas \
	$(LOADER_LIBTOOL_FLAGS)		\
	$(PYTHON2_LDFLAGS)

libpythonloader_la_LIBADD = \
	@GLIB_CFLAGS@			\
	$(PYGOBJECT_LIBS)		\
	$(PYTHON2_LIBS)
