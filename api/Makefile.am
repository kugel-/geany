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
	--gir Geany-1.0.gir

libgeany_1_0_la_SOURCES = geanyplugin2.vala

# do not add generated *.{c,h,vapi} to CLEANFILES
# https://blogs.gnome.org/jessevdk/2014/01/07/vala-and-autobuild/
CLEANFILES = geanyplugin2.o
