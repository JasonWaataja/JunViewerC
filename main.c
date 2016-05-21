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

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

static GFile *get_random_jun();
static GList *get_jun_list();
static void load_random_jun();
static void activate(GtkApplication *app,
                     gpointer data);

GtkWidget *jun_image;
GtkWidget *main_window;
GtkWidget *jun_box;

static GList *
get_jun_list()
{
  GList *jun_list = NULL;
  GFile *res_dir = g_file_new_for_path("/home/jason/git/JunViewerC/res/Jun");
  GFileType file_type = g_file_query_file_type(res_dir, G_FILE_QUERY_INFO_NONE, NULL);
  if (file_type == G_FILE_TYPE_DIRECTORY)
    {
      GFileEnumerator *en = g_file_enumerate_children(res_dir,
                                                      "*",
                                                      G_FILE_QUERY_INFO_NONE,
                                                      NULL,
                                                      NULL);
      GFileInfo *file_info;
      GFile *jun_file;
      int result;
      result = g_file_enumerator_iterate(en,
                                         &file_info,
                                         &jun_file,
                                         NULL,
                                         NULL);
      while (result && file_info)
        {
          if (g_file_info_get_file_type(file_info) == G_FILE_TYPE_REGULAR)
            {
              GFile *test_file = g_file_new_for_path(g_file_get_path(jun_file));
              jun_list = g_list_append(jun_list, test_file);
            }
          result = g_file_enumerator_iterate(en,
                                             &file_info,
                                             &jun_file,
                                             NULL,
                                             NULL);
        }
    }
  return jun_list;
}

static void load_random_jun()
{
  GList *jun_list = get_jun_list();
  unsigned int length = g_list_length(jun_list);
  int rand_i = rand() % length;
  jun_list = g_list_first(jun_list);
  GFile *jun_file = g_list_nth(jun_list, rand_i)->data;
  gtk_image_set_from_file(GTK_IMAGE(jun_image), g_file_get_path(jun_file));

  GdkPixbuf *buf = gtk_image_get_pixbuf(GTK_IMAGE(jun_image));
  int width = gdk_pixbuf_get_width(buf);
  int height = gdk_pixbuf_get_height(buf);
  gtk_widget_set_size_request(jun_image, width, height);
  gtk_widget_set_size_request(jun_box, width, height);

  gtk_widget_queue_resize(jun_image);
  gtk_container_check_resize(GTK_CONTAINER(jun_box));
  gtk_container_check_resize(GTK_CONTAINER(main_window));

  /*gtk_container_remove(GTK_CONTAINER(main_window), jun_box);*/
  /*gtk_container_add(GTK_CONTAINER(main_window), jun_box);*/
}

static void
activate(GtkApplication* app, gpointer data)
{
  main_window = gtk_application_window_new(app);
  gtk_window_set_resizable(GTK_WINDOW(main_window), TRUE);

  jun_image = gtk_image_new();

  jun_box = gtk_event_box_new();
  g_signal_connect(jun_box, "button_press_event", G_CALLBACK(load_random_jun), NULL);

  gtk_container_add(GTK_CONTAINER(jun_box), jun_image);
  gtk_container_add(GTK_CONTAINER(main_window), jun_box);

  gtk_widget_show_all(main_window);

  load_random_jun();
}

int main(int argc, char** argv)
{
  GtkApplication *app;
  app = gtk_application_new("com.waataja.junviewer", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  int status;
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}

