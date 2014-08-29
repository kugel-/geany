# Adapted from Pidgin's plugins/Makefile.am, thanks

EXTRA_DIST = \
	makefile.win32 \
	genapi.py

plugindir = $(libdir)/geany

# peas-support.c is not a header, but downstream plugins should be able
# to #include it and compile it with only GEANY_CFLAGS
plugins_includedir = $(includedir)/geany
plugins_include_HEADERS = \
	geany-peas-support.c \
	geanyplugin.h \
	geanyfunctions.h

# systems without python should continue to build OK
geanyfunctions.h: genapi.py ../src/plugins.c
	python genapi.py || true

all: geanyfunctions.h


libdemoplugin_la_LDFLAGS    = -module -avoid-version
libclassbuilder_la_LDFLAGS  = -module -avoid-version
libhtmlchars_la_LDFLAGS     = -module -avoid-version
libexport_la_LDFLAGS        = -module -avoid-version
libsaveactions_la_LDFLAGS   = -module -avoid-version
libfilebrowser_la_LDFLAGS   = -module -avoid-version
libsplitwindow_la_LDFLAGS   = -module -avoid-version

AM_CPPFLAGS = \
	-DDATADIR=\"$(datadir)\" \
	-DGTK \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/tagmanager/src \
	-I$(top_srcdir)/scintilla/include \
	$(GTK_CFLAGS) \
	$(PEAS_CFLAGS) \
	$(PLUGIN_CFLAGS) \
	-fvisibility=protected

if PLUGINS

if MINGW
# build Geany for Windows on non-Windows systems (cross-compile)
# (this is a little hack'ish and surely can be improved)
DLL_LD_FLAGS = -module -avoid-version
MINGW_CFLAGS = \
			-DGEANY_DATADIR=\"data\" \
			-DHAVE_CONFIG_H \
			-DGTK \
			-I$(top_srcdir) \
			-I$(top_srcdir)/src \
			-I$(top_srcdir)/tagmanager/src \
			-I$(top_srcdir)/scintilla/include \
			$(GTK_CFLAGS) \
			$(PLUGIN_CFLAGS)

.PHONY: all clean

all-local: \
	classbuilder.dll \
	htmlchars.dll \
	export.dll \
	saveactions.dll \
	splitwindow.dll \
	filebrowser.dll

.c.dll:
	$(CC) $(MINGW_CFLAGS) -o $@.o -c $<
	$(CC) -shared $@.o $(GTK_LIBS) $(DLL_LD_FLAGS) -o $@

clean:
	rm -f *.o *.dll

else

# Plugins to be installed
plugin_LTLIBRARIES = \
	libclassbuilder.la \
	libhtmlchars.la \
	libexport.la \
	libsaveactions.la \
	libfilebrowser.la \
	libsplitwindow.la

# Plugins not to be installed
noinst_LTLIBRARIES = \
	libdemoplugin.la

libdemoplugin_la_SOURCES    = geany-peas-support.c demoplugin.c
libclassbuilder_la_SOURCES  = geany-peas-support.c classbuilder.c
libhtmlchars_la_SOURCES     = geany-peas-support.c htmlchars.c
libexport_la_SOURCES        = geany-peas-support.c export.c
libsaveactions_la_SOURCES   = geany-peas-support.c saveactions.c
libfilebrowser_la_SOURCES   = geany-peas-support.c filebrowser.c
libsplitwindow_la_SOURCES   = geany-peas-support.c splitwindow.c

libdemoplugin_la_CFLAGS    = -I$(top_srcdir)/api -DG_LOG_DOMAIN=\""Demoplugin"\"
libclassbuilder_la_CFLAGS  = -I$(top_srcdir)/api -DG_LOG_DOMAIN=\""Classbuilder"\"
libhtmlchars_la_CFLAGS     = -I$(top_srcdir)/api -DG_LOG_DOMAIN=\""HTMLChars"\"
libexport_la_CFLAGS        = -I$(top_srcdir)/api -DG_LOG_DOMAIN=\""Export"\"
libsaveactions_la_CFLAGS   = -I$(top_srcdir)/api -DG_LOG_DOMAIN=\""SaveActions"\"
libfilebrowser_la_CFLAGS   = -I$(top_srcdir)/api -DG_LOG_DOMAIN=\""FileBrowser"\"
libsplitwindow_la_CFLAGS   = -I$(top_srcdir)/api -DG_LOG_DOMAIN=\""SplitWindow"\"

libdemoplugin_la_LIBADD    = $(GTK_LIBS) $(PEAS_LIBS)
libclassbuilder_la_LIBADD  = $(GTK_LIBS) $(PEAS_LIBS)
libhtmlchars_la_LIBADD     = $(GTK_LIBS) $(PEAS_LIBS)
libexport_la_LIBADD        = $(GTK_LIBS) $(PEAS_LIBS) -lm
libsaveactions_la_LIBADD   = $(GTK_LIBS) $(PEAS_LIBS)
libfilebrowser_la_LIBADD   = $(GTK_LIBS) $(PEAS_LIBS)
libsplitwindow_la_LIBADD   = $(GTK_LIBS) $(PEAS_LIBS)

plugin_DATA = \
	classbuilder.plugin \
	htmlchars.plugin    \
	export.plugin       \
	saveactions.plugin  \
	filebrowser.plugin  \
	splitwindow.plugin

endif # MINGW

# Could be combined in a single rule for single plugins
abi-replace.sed: geanyplugin.h
	printf "#include \"geanyplugin.h\"\nGEANY_ABI_VERSION_EXPR" | \
		$(CPP) $(AM_CPPFLAGS) - | \
		tail -1 | xargs expr | \
		xargs printf s,@GEANY_ABI_VERSION@,%d, > $@

%.plugin: %.plugin.in abi-replace.sed
		sed -f abi-replace.sed $< | \
		LC_ALL=C $(INTLTOOL_MERGE) -d -u -c $(top_builddir)/po/.intltool-merge-cache $(top_srcdir)/po - $@

endif # PLUGINS



#
# This part allows people to build their own plugins in here.
# Yes, it's a mess.
#
SUFFIXES = .c .so
.c.so:
	$(LIBTOOL) --mode=compile $(CC) -DHAVE_CONFIG_H -I$(top_srcdir) $(AM_CPPFLAGS) $(CFLAGS) -c $< -o tmp$@.lo $(PLUGIN_CFLAGS)
	$(LIBTOOL) --mode=link    $(CC) $(CFLAGS) -o libtmp$@.la -rpath $(plugindir) tmp$@.lo $(LIBS) $(LDFLAGS) -module -avoid-version $(PLUGIN_LIBS)
	@rm -f tmp$@.lo tmp$@.o libtmp$@.la
	@cp .libs/libtmp$@.so* $@
	@rm -f .libs/libtmp$@.*
