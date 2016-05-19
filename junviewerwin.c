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

#include "junviewerwin.h"

#include <gtk/gtk.h>
#include "junviewerapp.h"
#include <stdio.h>

struct _JunViewerWindow
{
  GtkApplicationWindow parent;
};

struct _JunViewerWindowClass
{
  GtkApplicationWindowClass parent_class;
};

G_DEFINE_TYPE(JunViewerWindow, jun_viewer_window, GTK_TYPE_APPLICATION_WINDOW);

static void
jun_viewer_window_init(JunViewerWindow *app)
{
}

static void
jun_viewer_window_class_init(JunViewerWindowClass *class)
{
  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS (class),
					      "/com/waataja/junviewer/window.ui");
}

JunViewerWindow *
jun_viewer_window_new(JunViewerApp *app)
{
  return g_object_new(JUN_VIEWER_WINDOW_TYPE, "application", app, NULL);
}

void
jun_viewer_window_open(JunViewerWindow *win, GFile *file)
{
}
