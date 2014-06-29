# Seed plugin loader

loaderdir = $(libdir)/geany/loaders

AM_CPPFLAGS = \
	-I$(top_srcdir)			\
	@GLIB_CFLAGS@			\
	@GIR_CFLAGS@			\
	$(GCOV_CFLAGS)			\
	$(WARN_CFLAGS)			\
	$(DISABLE_DEPRECATED)		\
	$(SEED_CFLAGS)

loader_LTLIBRARIES = libseedloader.la

libseedloader_la_SOURCES = \
	peas-extension-seed.c		\
	peas-extension-seed.h		\
	peas-plugin-loader-seed.c 	\
	peas-plugin-loader-seed.h

libseedloader_la_LDFLAGS = $(LOADER_LIBTOOL_FLAGS)
libseedloader_la_LIBADD = @GLIB_CFLAGS@ $(SEED_LIBS)
