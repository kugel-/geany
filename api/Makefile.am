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
Geany-1.0.gir: Geany-0.9.gir geanyheaders.gir
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

# GeanyGI section
libgeanygidir = $(libdir)/geany/geany
libgeanygi_LTLIBRARIES = libgeanygi-1.0.la

libgeanygi_1_0_la_CFLAGS = \
	-I$(top_srcdir)/src \
	-I$(top_srcdir)/plugins \
	-I$(top_srcdir)/scintilla/include \
	-I$(top_srcdir)/tagmanager/src \
	@GLIB_CFLAGS@ \
	@GTK_CFLAGS@ \
	-include geany-gi.h \
	-DGTK -DGEANY_FUNCTIONS_H

libgeanygi_1_0_la_LIBADD = \
	@GLIB_LIBS@ \
	@GTK_LIBS@

libgeanygi_1_0_la_VALAFLAGS = \
	--pkg gtk+-2.0 \
	--pkg glib-2.0 \
	--header geany-gi.h --use-header \
	--vapi geanygi-1.0.vapi \
	--library geanygi-1.0 \
	--target-glib=2.28 \
	--gir GeanyGI-1.0.gir

# add *.vapi to _SOURCES so that the dependency is picked up automatically (valac handles this fine)
libgeanygi_1_0_la_SOURCES = \
	geany.vapi \
	scintilla.vapi \
	data.vala \
	document.vala \
	editor.vala \
	keybindings.vala \
	filetype.vala

libgeanygi_DATA = Geany-1.0.typelib geany.vapi

CLEANFILES += data.o document.o editor.o filetype.o keybindings.o


# everything for Scintilla-1.0.typelib
check_PROGRAMS = girhelperscintilla

girhelperscintilla_SOURCES = girhelperscintilla.c

girhelperscintilla_CFLAGS = \
	@INTROSPECTION_CFLAGS@ \
	@GTK_CFLAGS@ \
	@GLIB_CFLAGS@ \
	-I$(top_srcdir)/scintilla/include

girhelperscintilla_LDADD = \
	$(top_builddir)/scintilla/libscintilla.a \
	-lstdc++ -lm \
	@INTROSPECTION_LIBS@ \
	@GTK_LIBS@ \
	@GLIB_LIBS@

# scidefines.gir: gen-sci-defines.py $(top_srcdir)/scintilla/scrips/Face.py $(top_srcdir)/scintilla/scrips/HFacer.py

scidefines.h: $(top_srcdir)/scintilla/include/Scintilla.iface
	$(AM_V_GEN)python gen-sci-defines.py $< -o $@

# this cannot be part of the scintilla.gir rule because the additional --accept-unprefixed
# option which is not desired for other headers
scidefines.gir: scidefines.h
	$(AM_V_GEN)\
	$(INTROSPECTION_SCANNER) --header-only \
	-n Scintilla --nsversion 0.1 --accept-unprefixed \
	$(filter %.h,$^) -o $@

scintilla.gir: $(top_srcdir)/scintilla/include/ScintillaWidget.h $(top_srcdir)/src/sciwrappers.h $(top_srcdir)/src/scimethods.h girhelperscintilla
	$(AM_V_GEN)\
	$(INTROSPECTION_SCANNER) --program ./girhelperscintilla \
	-DG_IR_SCANNER -DGTK -Duptr_t=gulong -Dsptr_t=glong \
	@GTK_CFLAGS@ -I$(top_srcdir)/scintilla/include \
	--cflags-begin -include gtk/gtk.h --cflags-end \
	-n Scintilla --symbol-prefix=scintilla --symbol-prefix=sci --nsversion 0.1 \
	-i Gtk-2.0 -i GObject-2.0 \
	--warn-all \
	--c-include sciwrappers.h --c-include scimethods.h \
	$(filter %.h,$^) -o $@

# sciwrappers.gir as name is required for vapigen to generate [CCode (cheader_filename="sciwrappers.h")]
sciwrappers.gir: scintilla.gir scidefines.gir
	$(AM_V_GEN)python girmerge.py $^ -o $@

scintilla.vapi: sciwrappers.gir scintilla.vapi.in
	$(AM_V_GEN)\
	$(VAPIGEN) $< --library scintilla-tmp \
	--pkg gtk+-2.0 --pkg gobject-2.0
	@cat scintilla.vapi.in scintilla-tmp.vapi > $@

CLEANFILES += scidefines.h scidefines.gir scintilla.gir

# the filename must match the <namespace> for g-ir-compiler
Scintilla-0.1.gir: sciwrappers.gir
	$(AM_V_GEN)cp $< $@

vapidir = $(datadir)/vala/vapi
vapi_DATA =  geany.vapi scintilla.vapi geany.deps

girdir = $(libdir)/geany/geany
gir_DATA = Geany-1.0.gir GeanyGI-1.0.gir Scintilla-0.1.gir

typelibdir =  $(libdir)/geany/geany
typelib_DATA = Geany-1.0.typelib GeanyGI-1.0.typelib Scintilla-0.1.typelib

if HAVE_INTROSPECTION
-include $(INTROSPECTION_MAKEFILE)

endif

CLEANFILES += Geany-1.0.gir GeanyGI-1.0.gir Scintilla-0.1.gir \
	Geany-1.0.typelib GeanyGI-1.0.typelib Scintilla-0.1.typelib
