
if WITH_PYTHON

INTROSPECTION_GIRS = Geany-1.0.gir GeanyScintilla-1.0.gir
INTROSPECTION_SCANNER_ARGS = --accept-unprefixed --c-include=geanyplugin.h

Geany_1_0_gir_NAMESPACE = Geany
Geany_1_0_gir_VERSION = 1.0
Geany_1_0_gir_INCLUDES = Gtk-3.0 GLib-2.0 GObject-2.0
Geany_1_0_gir_CFLAGS = @GTK_CFLAGS@ -DG_IR_SCANNING
Geany_1_0_gir_LIBS = $(top_builddir)/src/libgeany.la
Geany_1_0_gir_FILES = $(top_builddir)/doc/geany-gtkdoc.h

GeanyScintilla_1_0_gir_NAMESPACE = GeanyScintilla
GeanyScintilla_1_0_gir_VERSION = 1.0
GeanyScintilla_1_0_gir_INCLUDES = Gtk-3.0 GLib-2.0 GObject-2.0
GeanyScintilla_1_0_gir_CFLAGS = @GTK_CFLAGS@ -DGTK -DG_IR_SCANNING -include gtk/gtk.h
GeanyScintilla_1_0_gir_LIBS = $(top_builddir)/src/libgeany.la
GeanyScintilla_1_0_gir_FILES =  $(top_srcdir)/scintilla/include/ScintillaWidget.h \
								$(top_srcdir)/scintilla/include/Scintilla.h \
								$(top_srcdir)/scintilla/include/SciLexer.h


girdir = $(datadir)/gir-1.0
dist_gir_DATA = Geany-1.0.gir GeanyScintilla-1.0.gir
typelibdir = $(libdir)/girepository-1.0
dist_typelib_DATA = Geany-1.0.typelib GeanyScintilla-1.0.typelib

all-local: $(dist_gir_DATA) $(dist_typelib_DATA)

if HAVE_INTROSPECTION
include $(INTROSPECTION_MAKEFILE)
endif

endif
