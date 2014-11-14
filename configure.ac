dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.60])
AC_INIT([Geany], [1.25],
		[https://sourceforge.net/tracker/?group_id=153444&atid=787791])
AC_CONFIG_SRCDIR([src/geany.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([1.11 -Wall parallel-tests subdir-objects])
AC_CONFIG_HEADERS([config.h])

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

GEANY_PREFIX
GEANY_DOCDIR

GEANY_STATUS_ADD([Install Geany in], [${prefix}])
if test -n "${build}" -a -n "${target}"; then
	GEANY_STATUS_ADD([Building Geany on], [${build}])
	GEANY_STATUS_ADD([Building Geany for], [${target}])
fi

# why do we use this?
AC_USE_SYSTEM_EXTENSIONS

m4_ifdef([AM_PROG_AR],[AM_PROG_AR])
AC_PROG_CC
AC_PROG_CC_C99
AM_PROG_CC_C_O

AC_PROG_CXX
# check for C++ compiler explicitly and fail if none is found, do this check
# after AC_PROG_CXX has set the CXX environment variable
if ! which $CXX >/dev/null 2>&1; then
	AC_MSG_ERROR([No C++ compiler found. Please install a C++ compiler.])
fi

AC_PROG_INSTALL
AC_PROG_LN_S

# autoscan start

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h fnmatch.h glob.h stdlib.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_STRUCT_TM

# Checks for library functions.
AC_CHECK_FUNCS([ftruncate fgetpos mkstemp strerror strstr])

# autoscan end


# check for SVN revision
GEANY_CHECK_REVISION([dnl force debug mode for a SVN working copy
					  CFLAGS="-g -DGEANY_DEBUG $CFLAGS"])


AC_ARG_ENABLE([gtk3],
		[AS_HELP_STRING([--enable-gtk3],
						[compile with GTK3 support (experimental) [default=no]])],
		[enable_gtk3=$enableval],
		[enable_gtk3=no])

AS_IF([test "x$enable_gtk3" = xyes],
	  [gtk_package=gtk+-3.0
	   gtk_min_version=3.0],
	  [gtk_package=gtk+-2.0
	   gtk_min_version=2.16])
AM_CONDITIONAL([GTK3], [test "x$gtk_package" = "xgtk+-3.0"])

# GTK
gtk_modules="$gtk_package >= $gtk_min_version"
PKG_CHECK_MODULES([GTK], [$gtk_modules])
AC_SUBST([GTK_CFLAGS])
AC_SUBST([GTK_LIBS])
GTK_VERSION=`$PKG_CONFIG --modversion $gtk_package`
GEANY_STATUS_ADD([Using GTK version], [${GTK_VERSION}])
# GLib
# geany itself requires only 2.20+, but libpeas wants 2.30
glib_modules="glib-2.0 >= 2.30.0 gmodule-2.0 gio-2.0"
PKG_CHECK_MODULES([GLIB], [$glib_modules])
AC_SUBST([GLIB_CFLAGS])
AC_SUBST([GLIB_LIBS])
GLIB_VERSION=`$PKG_CONFIG --modversion glib-2.0`
GEANY_STATUS_ADD([Using GLib version], [${GLIB_VERSION}])
# GIR
# libpeas wants 1.39.0
gir_modules="gobject-introspection-1.0 >= 1.39.0"
PKG_CHECK_MODULES([GIR], [$gir_modules])
AC_SUBST([GIR_CFLAGS])
AC_SUBST([GIR_LIBS])
GOBJECT_INTROSPECTION_REQUIRE(1.39.0)
# GTHREAD checks
gthread_modules="gthread-2.0"
PKG_CHECK_MODULES([GTHREAD], [$gthread_modules])
AC_SUBST([GTHREAD_CFLAGS])
AC_SUBST([GTHREAD_LIBS])

VAPIGEN_CHECK

# so that PEAS_CFLAGS/LIBS get properly defined
PKG_CHECK_MODULES(PEAS, [
	glib-2.0
	gobject-2.0
	gmodule-2.0
	gio-2.0
	gobject-introspection-1.0
])
AC_SUBST([PEAS_CFLAGS])
AC_SUBST([PEAS_LIBS])

AC_SUBST([DEPENDENCIES], ["$gtk_modules $glib_modules $gir_modules"])

# --disable-deprecated switch for GTK2 purification
AC_ARG_ENABLE([deprecated],
		[AS_HELP_STRING([--disable-deprecated], [Disable deprecated GTK functions.])],
		[GTK_CFLAGS="$GTK_CFLAGS -DG_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED"],
		[])


# Check for binary relocation support
GEANY_CHECK_BINRELOC
# CTags source compatibility (we actually use GRegex instead of POSIX regcomp)
AC_DEFINE([HAVE_REGCOMP], [1], [Should always be 1, required for CTags.])
# Plugins support
GEANY_CHECK_PLUGINS
# check for mingw specific settings
GEANY_CHECK_MINGW
# API
AM_PROG_VALAC


GEANY_CHECK_SOCKET
GEANY_CHECK_VTE
GEANY_CHECK_THE_FORCE dnl hehe

# i18n
GEANY_I18N

GEANY_DATA_DIR=`eval echo ${datarootdir}/geany`
AC_SUBST([GEANY_DATA_DIR])

# Documentation tools
GEANY_CHECK_DOCUTILS
GEANY_CHECK_DOXYGEN

# libpeas

AC_PATH_PROG(GLIB_GENMARSHAL, glib-genmarshal)
AC_PATH_PROG(GLIB_MKENUMS, glib-mkenums)
LOADER_LIBTOOL_FLAGS="-module -avoid-version -no-undefined"
AC_SUBST(LOADER_LIBTOOL_FLAGS)

dnl ================================================================
dnl Python
dnl ================================================================

PYTHON2_REQUIRED=2.5.2
PYTHON3_REQUIRED=3.2.0

dnl Used by the Python loader to check the PyGObject version at runtime
PYGOBJECT_MAJOR_VERSION=3
PYGOBJECT_MINOR_VERSION=0
PYGOBJECT_MICRO_VERSION=0
PYGOBJECT_REQUIRED=$PYGOBJECT_MAJOR_VERSION.$PYGOBJECT_MINOR_VERSION.$PYGOBJECT_MICRO_VERSION
AC_DEFINE_UNQUOTED(PYGOBJECT_MAJOR_VERSION,[$PYGOBJECT_MAJOR_VERSION], [PyGObject major version.])
AC_DEFINE_UNQUOTED(PYGOBJECT_MINOR_VERSION,[$PYGOBJECT_MINOR_VERSION], [PyGObject minor version.])
AC_DEFINE_UNQUOTED(PYGOBJECT_MICRO_VERSION,[$PYGOBJECT_MICRO_VERSION], [PyGObject micro version.])

AC_MSG_CHECKING([for Python 2 availability.])

AC_ARG_ENABLE(python2,
    AS_HELP_STRING([--enable-python2],[Enable Python 2 support]),
    [enable_python2=$enableval],
    [enable_python2=auto])

if test "x$enable_python2" = "xno"; then
    found_python2="no (disabled, use --enable-python2 to enable)"
else
    dnl NOTE: we do not use AM_PATH_PYTHON, since it does not allow
    dnl to call it twice for py2 and py3, so we just check the prog
    AC_PATH_PROGS(PYTHON2, [python2 python2.7 python2.6 python2.5])
    if test -z "${PYTHON2}"; then
        found_python2="no (python2 not found)"
    else
        AM_PYTHON_CHECK_VERSION($PYTHON2, $PYTHON2_REQUIRED, [
            AC_PATH_PROGS(PYTHON2_CONFIG, [python2-config python-config python2.7-config python2.6-config python2.5-config])
            if test -n "${PYTHON2_CONFIG}"; then
                PKG_CHECK_MODULES(PYGOBJECT, pygobject-3.0 >= $PYGOBJECT_REQUIRED, [
                    found_python2=yes
                    PYTHON2_CFLAGS=`${PYTHON2_CONFIG} --includes`
                    PYTHON2_LIBS=`${PYTHON2_CONFIG} --libs`
                    PYTHON2_LDFLAGS=`${PYTHON2_CONFIG} --ldflags`
                    PYTHON2_PYEXECDIR=`$PYTHON2 -c "from distutils import sysconfig; print(sysconfig.get_python_lib(1, 0, prefix='\\$' '{exec_prefix}'))"`
                    AC_SUBST(PYTHON2_CFLAGS)
                    AC_SUBST(PYTHON2_LIBS)
                    AC_SUBST(PYTHON2_LDFLAGS)
                    AC_SUBST(PYTHON2_PYEXECDIR)
                    AC_DEFINE(ENABLE_PYTHON2, 1, [Define to compile with Python 2 support])
                ], [
                    found_python2="no (PyGObject not found)"
                ])
            else
                found_python2="no (python2-config not found)"
            fi
        ], [
            found_python2="no (python2 version too old)"
        ])
    fi
fi

if test "x$enable_python2" = "xyes" -a "x$found_python2" != "xyes"; then
    AC_MSG_ERROR([You need to have Python 2 and PyGobject installed to build libpeas])
fi
AC_MSG_RESULT([$found_python2])

AM_CONDITIONAL([ENABLE_PYTHON2],[test "x$found_python2" = "xyes"])


AC_MSG_CHECKING([for Python 3 availability.])

AC_ARG_ENABLE(python3,
    AS_HELP_STRING([--enable-python3],[Enable Python 3 support]),
    [enable_python3=$enableval],
    [enable_python3=auto])

if test "x$enable_python3" = "xno"; then
    found_python3="no (disabled, use --enable-python3 to enable)"
else
    AM_PATH_PYTHON($PYTHON3_REQUIRED, [
        AC_PATH_TOOL(PYTHON3_CONFIG, "python3-config")
        if test -n "${PYTHON3_CONFIG}"; then
            PKG_CHECK_MODULES(PYGOBJECT, pygobject-3.0 >= $PYGOBJECT_REQUIRED, [
                found_python3=yes
                PYTHON3_CFLAGS=`${PYTHON3_CONFIG} --includes`
                PYTHON3_LIBS=`${PYTHON3_CONFIG} --libs`
                PYTHON3_LDFLAGS=`${PYTHON3_CONFIG} --ldflags`
                PYTHON3_PYEXECDIR="${pyexecdir}"
                AC_SUBST(PYTHON3_CFLAGS)
                AC_SUBST(PYTHON3_LIBS)
                AC_SUBST(PYTHON3_LDFLAGS)
                AC_SUBST(PYTHON3_PYEXECDIR)
                AC_DEFINE(ENABLE_PYTHON3, 1, [Define to compile with Python 3 support])
            ], [
                found_python3="no (PyGObject not found)"
            ])
        else
            found_python3="no (python3-config not found)"
        fi
    ], [
        found_python3="no (python3 not found)"
    ])
fi

if test "x$enable_python3" = "xyes" -a "x$found_python3" != "xyes"; then
    AC_MSG_ERROR([You need to have Python 3 and PyGObject installed to build libpeas])
fi
AC_MSG_RESULT([$found_python3])

AM_CONDITIONAL([ENABLE_PYTHON3],[test "x$found_python3" = "xyes"])

# FIXME: always disabled
AM_CONDITIONAL([ENABLE_SEED],[test "1" = "0"])

# Output
AC_CONFIG_FILES([
		Makefile
		icons/Makefile
		icons/16x16/Makefile
		icons/24x24/Makefile
		icons/32x32/Makefile
		icons/48x48/Makefile
		icons/scalable/Makefile
		icons/tango/Makefile
		icons/tango/16x16/Makefile
		icons/tango/24x24/Makefile
		icons/tango/32x32/Makefile
		icons/tango/48x48/Makefile
		icons/tango/scalable/Makefile
		api/Makefile
		libpeas/Makefile
		libpeas-loaders/Makefile
		libpeas-loaders/python/Makefile
		libpeas-loaders/python3/Makefile
		libpeas-loaders/geany/Makefile
		tagmanager/Makefile
		tagmanager/ctags/Makefile
		tagmanager/mio/Makefile
		tagmanager/src/Makefile
		scintilla/Makefile
		scintilla/include/Makefile
		src/Makefile
		plugins/Makefile
		po/Makefile.in
		data/Makefile
		doc/Makefile
		doc/geany.1
		geany.spec
		geany.pc
		doc/Doxyfile
		tests/Makefile
		tests/ctags/Makefile
])
AC_OUTPUT

# Summary
GEANY_STATUS_OUTPUT
echo ""
echo "Configuration is done OK."
echo ""
