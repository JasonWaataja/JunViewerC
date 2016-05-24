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
#include <stdlib.h>

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
  gtk_application_window_set_show_menubar(GTK_APPLICATION_WINDOW(win), TRUE);
  JunViewerWindowPrivate *priv;
  priv = jun_viewer_window_get_instance_private(win);
  g_signal_connect_swapped(priv->click_box, "button_press_event", G_CALLBACK(jun_viewer_window_load_random_jun), win);
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

GList *
get_reg_files_in_dir(const char *dir_path)
{
  GList *file_list = NULL;
  if (dir_path == NULL)
    return NULL;

  GFile *file_dir = g_file_new_for_path(dir_path);
  GFileType file_type;
  file_type = g_file_query_file_type(file_dir, G_FILE_QUERY_INFO_NONE, NULL);
  if (file_type == G_FILE_TYPE_DIRECTORY)
    {
      GFileEnumerator *en;
      en = g_file_enumerate_children(file_dir,
                                     "*",
                                     G_FILE_QUERY_INFO_NONE,
                                     NULL,
                                     NULL);
      GFileInfo *file_info;
      GFile *temp_file;
      int result;
      result = g_file_enumerator_iterate(en,
                                         &file_info,
                                         &temp_file,
                                         NULL,
                                         NULL);
      while (result && file_info)
        {
          file_type = g_file_query_file_type(temp_file,
                                             G_FILE_QUERY_INFO_NONE,
                                             NULL);
          if (file_type == G_FILE_TYPE_REGULAR)
            {
              const char *file_path = g_file_get_path(temp_file);
              GFile *reg_file = g_file_new_for_path(file_path);
              file_list = g_list_append(file_list, reg_file);
            }
          result = g_file_enumerator_iterate(en,
                                             &file_info,
                                             &temp_file,
                                             NULL,
                                             NULL);
        }
    }
  return file_list;
}

void
jun_viewer_window_load_random_jun(JunViewerWindow *self)
{
  printf("%i\n", rand());
  GFile *jun_file;
  puts("starting");
  jun_file = get_random_jun_file();
  puts("got random file");
  JunViewerWindowPrivate *priv = jun_viewer_window_get_instance_private(self);
  puts("got instance");
  GtkWidget *jun_image = GTK_WIDGET(priv->jun_image);
  puts("got image");
  gtk_image_set_from_file(GTK_IMAGE(jun_image), g_file_get_path(jun_file));
  puts("set file");
  
  GdkPixbuf *buf = gtk_image_get_pixbuf(GTK_IMAGE(jun_image));
  if (buf != NULL)
    {
      int width = gdk_pixbuf_get_width(buf);
      int height = gdk_pixbuf_get_height(buf);
      gtk_window_resize(GTK_WINDOW(self), width, height);
    }
  else
    {
      GdkPixbufAnimation *ani = gtk_image_get_animation(GTK_IMAGE(jun_image));
      int width = gdk_pixbuf_animation_get_width(ani);
      int height = gdk_pixbuf_animation_get_height(ani);
      gtk_window_resize(GTK_WINDOW(self), width, height);
    }
  puts("resized");
}

GFile *
get_random_jun_file()
{
  srand(time(NULL));
  GList *jun_list = get_jun_list();
  int list_length = g_list_length(jun_list);
  int rand_index = rand() % list_length;
  GFile *jun_file = g_list_nth(jun_list, rand_index)->data;
  return jun_file;
}

GList *get_jun_list()
{
  GList *jun_list = get_reg_files_in_dir("/home/jason/git/JunViewerC/res/Jun");
  return jun_list;
}
