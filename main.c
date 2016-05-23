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
static GList *get_reg_files_in_dir(const char *file_dir);

GtkWidget *jun_image;
GtkWidget *main_window;
GtkWidget *jun_box;
GMenu *window_menu;

static void
file_activate()
{
  g_print("file activate\n");
}


static GList *
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

static GList *
get_jun_list()
{
  GList *jun_list = get_reg_files_in_dir("/home/jason/git/JunViewerC/res/Jun");
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
  if (buf != NULL)
    {
      int width = gdk_pixbuf_get_width(buf);
      int height = gdk_pixbuf_get_height(buf);
      gtk_window_resize(GTK_WINDOW(main_window), width, height);
    } else {
        GdkPixbufAnimation *ani = gtk_image_get_animation(GTK_IMAGE(jun_image));
        int width = gdk_pixbuf_animation_get_width(ani);
        int height = gdk_pixbuf_animation_get_height(ani);
        gtk_window_resize(GTK_WINDOW(main_window), width, height);
    }
}

static void
activate(GtkApplication* app, gpointer data)
{
  main_window = gtk_application_window_new(app);
  gtk_window_set_resizable(GTK_WINDOW(main_window), TRUE);
  gtk_window_set_title(GTK_WINDOW(main_window), "Jun Viewer");

  GSimpleAction *sa = g_simple_action_new("FileAction", NULL);
  char *name = g_action_print_detailed_name("FileAction", NULL);
  g_signal_connect(sa, "activate", G_CALLBACK(file_activate), NULL);

  window_menu = g_menu_new();
  g_menu_insert_item(window_menu, 0, g_menu_item_new("File", "FileAction"));
  gtk_application_set_menubar(app, G_MENU_MODEL(window_menu));

  gtk_application_window_set_show_menubar(GTK_APPLICATION_WINDOW(main_window), TRUE);

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

