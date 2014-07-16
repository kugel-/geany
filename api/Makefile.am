# general stuff
scanned_headers = \
	$(top_srcdir)/src/build.h \
	$(top_srcdir)/src/document.h \
	$(top_srcdir)/src/filetypes.h \
	$(top_srcdir)/src/editor.h \
	$(top_srcdir)/src/highlighting.h \
	$(top_srcdir)/src/prefs.h \
	$(top_srcdir)/src/ui_utils.h \
	$(top_srcdir)/src/templates.h \
	$(top_srcdir)/src/toolbar.h \
	$(top_srcdir)/src/search.h

geanyheaders.gir: $(scanned_headers)
	$(AM_V_GEN) \
	$(INTROSPECTION_SCANNER) --library geany --header-only -n Geany --nsversion 1.0 . \
	--c-include geanyplugin.h -DG_IR_SCANNER @GTK_CFLAGS@ \
	-i GLib-2.0 -i Gtk-2.0 -i GObject-2.0 \
	$+ -o $@


# err, vapigen doesn't update the timestamp if the vapi didn't change
geanyheaders.vapi: geanyheaders.gir
	$(AM_V_GEN)$(VAPIGEN) --pkg gtk+-2.0 --pkg gobject-2.0 \
	$< --library $(basename $<)
	@touch $@

geanyplugin2.vapi: libgeany-1.0.la

geany.vapi: geany.vapi.in geanyheaders.vapi geanyplugin2.vapi
	$(AM_V_GEN)cat $+ > $@

# same, for now
scintilla.vapi: scintilla.vapi.in
	@cp $< $@

Geany-0.9.gir: libgeany-1.0.la
# g-ir-compiler has some issues with some .gir files so the merging must be done in a slightly
# intelligent way. Note that the merge is only necessary in the first place because g-ir-compiler
# can only compile one .gir into one .typelib
Geany-1.0.gir: Geany-0.9.gir filetypes.gir
	$(AM_V_GEN)python girmerge.py $+ -o $@

# geanyplugin2 section
libgeanydir = $(libdir)/geany/geany
libgeany_LTLIBRARIES = libgeany-1.0.la

libgeany_1_0_la_CFLAGS = \
	@GLIB_CFLAGS@ \
	@GTK_CFLAGS@

libgeany_1_0_la_LIBADD = \
	@GLIB_LIBS@ \
	@GTK_LIBS@

libgeany_1_0_la_VALAFLAGS = \
	--pkg gtk+-2.0 \
	--pkg glib-2.0 \
	--header geanyplugin2.h --use-header \
	--library geany-1.0 \
	--vapi geanyplugin2.vapi \
	--gir Geany-0.9.gir

libgeany_1_0_la_SOURCES = geanyplugin2.vala

libgeany_DATA = Geany-1.0.typelib

# do not add generated *.{c,h,vapi} to CLEANFILES
# https://blogs.gnome.org/jessevdk/2014/01/07/vala-and-autobuild/
CLEANFILES = geanyplugin2.o geany.vapi geanyheaders.vapi geanyplugin2.vapi scintilla.vapi

vapidir = $(datadir)/vala/vapi
vapi_DATA =  geany.vapi scintilla.vapi geany.deps

girdir = $(libdir)/geany/geany
gir_DATA = Geany-1.0.gir

typelibdir =  $(libdir)/geany/geany
typelib_DATA = Geany-1.0.typelib

if HAVE_INTROSPECTION
-include $(INTROSPECTION_MAKEFILE)

endif
