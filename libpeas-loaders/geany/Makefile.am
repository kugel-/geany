# Seed plugin loader

loaderdir = $(libdir)/geany/libpeas-1.0/loaders

AM_CPPFLAGS = \
	-I$(top_srcdir)			\
	@GLIB_CFLAGS@			\
	@GIR_CFLAGS@			\
	$(WARN_CFLAGS)			\
	$(DISABLE_DEPRECATED)	\
	-Wno-missing-prototypes

loader_LTLIBRARIES = libgeanyloader.la

libgeanyloader_la_SOURCES = \
	peas-plugin-loader-geany.c 	\
	peas-plugin-loader-geany.h

libgeanyloader_la_LDFLAGS = $(LOADER_LIBTOOL_FLAGS) -Wl,-rpath,$(libdir)/geany/libpeas
libgeanyloader_la_LIBADD = $(PEAS_LIBS)
