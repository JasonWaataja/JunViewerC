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
#include "junviewerwin.h"
#include <gtk/gtk.h>

struct _JunViewerApp
{
  GtkApplication parent;
};

struct _JunViewerAppClass
{
  GtkApplicationClass parent_class;
};

G_DEFINE_TYPE (JunViewerApp, jun_viewer_app, GTK_TYPE_APPLICATION);

static void
jun_viewer_app_init (JunViewerApp *app)
{
}

static void
jun_viewer_app_startup (GApplication *app)
{
  G_APPLICATION_CLASS (jun_viewer_app_parent_class)->startup (app);
}

static void
jun_viewer_app_activate (GApplication *app)
{
  JunViewerWindow *win;
  win = jun_viewer_window_new (JUN_VIEWER_APP (app));
  gtk_window_present (GTK_WINDOW (win));
}

static void
jun_viewer_app_class_init (JunViewerAppClass *class)
{
  G_APPLICATION_CLASS (class)->startup = jun_viewer_app_startup;
  G_APPLICATION_CLASS (class)->activate = jun_viewer_app_activate;
}

JunViewerApp *
jun_viewer_app_new (void)
{
  return g_object_new (JUN_VIEWER_APP_TYPE,
                       "application-id", "com.waataja.junviewerapp",
                       NULL);
}
