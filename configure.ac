

AC_INIT([junviewerc], [0.1.0], [jasonswaataja@gmail.com])
AM_INIT_AUTOMAKE([-Wall foreign])
AM_SILENT_RULES([yes])

AC_PROG_CC
PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES([GTK], [
	glib-2.0
	gtk+-3.0
])

AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 res/Makefile
                 ])
AC_OUTPUT
