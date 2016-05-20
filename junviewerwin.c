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


typedef struct _JunViewerWindowPrivate JunViewerWindowPrivate;
struct _JunViewerWindowPrivate
{
  GtkImage* jun_image;
  GtkEventBox *click_box;
};

G_DEFINE_TYPE_WITH_PRIVATE(JunViewerWindow, jun_viewer_window, GTK_TYPE_APPLICATION_WINDOW);

static void
jun_viewer_window_init(JunViewerWindow *win)
{
  gtk_widget_init_template(GTK_WIDGET(win));
}

static void
jun_viewer_window_class_init(JunViewerWindowClass *class)
{
  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS (class),
                                              "/com/waataja/junviewer/window.ui");
  gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), JunViewerWindow, jun_image);
  gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), JunViewerWindow, click_box);
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

GList *get_jun_images()
{
  return get_jun_images_internal();
}

GList *get_jun_images_internal()
{
  GFile *res_folder;
  GList *jun_file_list = NULL;
  res_folder = g_file_new_for_path(jun_directory);
  GFileType file_type = g_file_query_file_type(res_folder, G_FILE_QUERY_INFO_NONE, NULL);
  if (file_type == G_FILE_TYPE_DIRECTORY)
    {
      GFileEnumerator *enumer = g_file_enumerate_children(res_folder,
                                                          "*",
                                                          G_FILE_QUERY_INFO_NONE,
                                                          NULL,
                                                          NULL);
      GFileInfo *info;
      info = g_file_enumerator_next_file(enumer, NULL, NULL);
      while (info != NULL)
        {
          file_type = g_file_info_get_file_type(info);
          if (file_type == G_FILE_TYPE_REGULAR)
            {
              printf("%s", g_file_info_get_name(info));
              GFile* jun_file;
              g_file_set_attributes_from_info(jun_file,
                                              info,
                                              G_FILE_QUERY_INFO_NONE,
                                              NULL,
                                              NULL);
              jun_file_list = g_list_append(jun_file_list, jun_file);
            }
          info = g_file_enumerator_next_file(enumer, NULL, NULL);
        }
    }
  return jun_file_list;
}

void
jun_viewer_window_load_jun(JunViewerWindow *self)
{
  GList *jun_images = get_jun_images();
}
