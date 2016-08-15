
AM_CFLAGS = $(GTK_CFLAGS)
AM_CPPFLAGS = -DDATADIR=\"$(pkgdatadir)\"
bin_PROGRAMS = jvc
jvc_SOURCES = main.c junviewerapp.c junviewerwin.c resources.c
jvc_LDADD = $(GTK_LIBS)

BUILT_SOURCES = resources.c

resources.c: $(top_srcdir)/res/junviewer.gresource.xml $(top_srcdir)/res/ui/window.ui
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) \
	$(top_srcdir)/res/junviewer.gresource.xml \
		--target=$@ --sourcedir=$(top_srcdir)/res --generate-source
