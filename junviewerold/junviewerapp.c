/*   This file is part of JunViewerC.

     JunViewerC is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     JunViewerC is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with JunViewerC.  If not, see <http://www.gnu.org/licenses/>.  */

#include "junviewerapp.h"

#include <gtk/gtk.h>
#include "junviewerwin.h"

struct _JunViewerApp
{
  GtkApplication parent;
};

struct _JunViewerAppClass
{
  GtkApplicationClass parent_class;
};

G_DEFINE_TYPE(JunViewerApp, jun_viewer_app, GTK_TYPE_APPLICATION);

static void
jun_viewer_app_init(JunViewerApp *app)
{
}

static void
jun_viewer_app_startup(GApplication *app)
{
  GtkBuilder *builder;
  GMenuModel *menu_bar;
  G_APPLICATION_CLASS(jun_viewer_app_parent_class)->startup(app);
  builder = gtk_builder_new_from_resource("/com/waataja/junviewer/app-menu.ui");
  menu_bar = G_MENU_MODEL(gtk_builder_get_object(builder, "menubar"));
  gtk_application_set_menubar(GTK_APPLICATION(app), menu_bar);
  g_object_unref(builder);
}

static void 
jun_viewer_app_activate(GApplication *app)
{
  JunViewerWindow *win;
  win = jun_viewer_window_new(JUN_VIEWER_APP(app));
  gtk_window_present(GTK_WINDOW(win));
}

static void
jun_viewer_app_open(GApplication *app,
		    GFile **files,
		    gint n_files,
		    const gchar *hint)
{
  GList *windows;
  JunViewerWindow *win;
  int i;
  windows = gtk_application_get_windows(GTK_APPLICATION(app));
  if (windows)
    win = JUN_VIEWER_WINDOW(windows->data);
  else
    win = jun_viewer_window_new(JUN_VIEWER_APP(app));


  for (i = 0; i < n_files; i++)
    jun_viewer_window_open(win, files[i]);

  gtk_window_present(GTK_WINDOW(win));
}

static void
jun_viewer_app_class_init(JunViewerAppClass *class)
{
  G_APPLICATION_CLASS(class)->startup = jun_viewer_app_startup;
  G_APPLICATION_CLASS(class)->activate = jun_viewer_app_activate;
  G_APPLICATION_CLASS(class)->open = jun_viewer_app_open;
}

JunViewerApp *
jun_viewer_app_new(void)
{
  return g_object_new(JUN_VIEWER_APP_TYPE,
		      "application-id", "com.waataja.junviewerapp",
		      "flags", G_APPLICATION_HANDLES_OPEN,
		      NULL);
}

