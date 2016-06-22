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

#define DEFAULT_IMAGE_WIDTH 540

/*const char *file_dir = "Jun";*/

static GFile *
get_random_jun ();

static GList *
get_reg_files_in_dir (const char *file_dir);

static void
on_main_window_destroy ();

static void
on_jun_image_box_button_press_event ();

static void
on_next_jun_button_clicked ();

static void
load_random_jun ();

static void
load_images_from_directory (const char *dir);

static void
select_image_dir ();

static void
save_current_jun ();

static void
on_quit_item_activate ();

static void
on_about_activate ();

static void
set_image_to_src_size ();

static void
set_image_to_default_size ();

/* Set jun_image's size to the window size from src_pixbuf.  */
/*static void*/
/*on_size_allocate (GtkWidget *widget,*/
                  /*GdkRectangle *allocation,*/
                  /*gpointer userdata);*/

GList *file_list;
/*GFile *current_directory;*/

GtkWidget *main_window;
GtkWidget *jun_image;
GtkWidget *next_jun_button;
GtkWidget *jun_image_box;

/* These are only used if it's a pixbuf and not an animation.  */
GdkPixbuf *src_pixbuf;
GdkPixbuf *current_pixbuf;


int
main (int argc,
     char *argv[])
{
  gtk_init (&argc, &argv);

  srand(time(NULL));

  GtkBuilder *builder;
  builder = gtk_builder_new_from_file ("window.ui");

  main_window = GTK_WIDGET (gtk_builder_get_object (builder, "main_window"));
  jun_image = GTK_WIDGET (gtk_builder_get_object (builder, "jun_image"));
  next_jun_button = GTK_WIDGET (gtk_builder_get_object (builder,
                                                        "next_jun_button"));
  jun_image_box = GTK_WIDGET (gtk_builder_get_object (builder,
                                                      "jun_image_box"));
  

  g_signal_connect (G_OBJECT (main_window), "destroy",
                    G_CALLBACK (on_main_window_destroy), NULL);
  g_signal_connect (G_OBJECT (jun_image_box), "button-press-event",
                    G_CALLBACK (on_jun_image_box_button_press_event), NULL);
  g_signal_connect (G_OBJECT (next_jun_button), "clicked",
                    G_CALLBACK (on_next_jun_button_clicked), NULL);
  g_signal_connect (G_OBJECT (gtk_builder_get_object (builder,
                                                      "next_jun_item")),
                    "activate",
                    G_CALLBACK(load_random_jun),
                    NULL);
  g_signal_connect (G_OBJECT (gtk_builder_get_object (builder,
                                                      "open_item")),
                    "activate",
                    G_CALLBACK(select_image_dir),
                    NULL);
  g_signal_connect (G_OBJECT (gtk_builder_get_object (builder,
                                                      "save_item")),
                    "activate",
                    G_CALLBACK(save_current_jun),
                    NULL);
  g_signal_connect (G_OBJECT (gtk_builder_get_object (builder,
                                                      "quit_item")),
                    "activate",
                    G_CALLBACK(on_quit_item_activate),
                    NULL);
  g_signal_connect (G_OBJECT (gtk_builder_get_object (builder,
                                                      "about_item")),
                    "activate",
                    G_CALLBACK(on_about_activate),
                    NULL);
  g_signal_connect (G_OBJECT (gtk_builder_get_object (builder,
                                                      "set_to_original"
                                                      "_size_item")),
                    "activate",
                    G_CALLBACK (set_image_to_src_size),
                    NULL);
  g_signal_connect (G_OBJECT (gtk_builder_get_object (builder,
                                                      "set_to_default"
                                                      "_size_item")),
                    "activate",
                    G_CALLBACK (set_image_to_default_size),
                    NULL);

  /*g_signal_connect (G_OBJECT (jun_image),*/
                    /*"size-allocate",*/
                    /*G_CALLBACK (on_size_allocate),*/
                    /*NULL);*/

  gtk_builder_connect_signals (builder, NULL);
  g_object_unref (builder);

  load_images_from_directory("Jun");
  gtk_widget_show (main_window);
  load_random_jun();

  gtk_main ();

  return EXIT_SUCCESS;
}


static GFile *
get_random_jun ()
{
  unsigned int length = g_list_length (file_list);
  int rand_i = rand () % length;
  file_list = g_list_first (file_list);
  GFile *jun_file = g_list_nth (file_list, rand_i)->data;
  return jun_file;
}

static GList *
get_reg_files_in_dir (const char *dir_path)
{
  GList *file_list = NULL;

  if (dir_path == NULL)
    return NULL;

  GFile *file_dir = g_file_new_for_path (dir_path);
  GFileType file_type;
  file_type = g_file_query_file_type (file_dir, G_FILE_QUERY_INFO_NONE, NULL);
  if (file_type == G_FILE_TYPE_DIRECTORY)
    {
      GFileEnumerator *en;
      en = g_file_enumerate_children (file_dir,
                                     "*",
                                     G_FILE_QUERY_INFO_NONE,
                                     NULL,
                                     NULL);
      GFileInfo *file_info;
      GFile *temp_file;
      int result;
      result = g_file_enumerator_iterate (en,
                                         &file_info,
                                         &temp_file,
                                         NULL,
                                         NULL);
      while (result && file_info)
        {
          file_type = g_file_query_file_type (temp_file,
                                             G_FILE_QUERY_INFO_NONE,
                                             NULL);
          if (file_type == G_FILE_TYPE_REGULAR)
            {
              const char *file_path = g_file_get_path (temp_file);
              GFile *reg_file = g_file_new_for_path (file_path);
              file_list = g_list_append (file_list, reg_file);
            }
          result = g_file_enumerator_iterate (en,
                                             &file_info,
                                             &temp_file,
                                             NULL,
                                             NULL);
        }
    }

  return file_list;
}

static void
on_main_window_destroy ()
{
  gtk_main_quit ();
}

static void
on_jun_image_box_button_press_event ()
{
  load_random_jun ();
}

static void
on_next_jun_button_clicked ()
{
  load_random_jun ();
}

static void
load_random_jun ()
{
  GFile *jun_file = get_random_jun ();
  gtk_image_set_from_file (GTK_IMAGE (jun_image), g_file_get_path (jun_file));

  GtkImageType image_type = gtk_image_get_storage_type (GTK_IMAGE (jun_image));

  if (image_type == GTK_IMAGE_PIXBUF)
    {
      src_pixbuf = gdk_pixbuf_copy (gtk_image_get_pixbuf (GTK_IMAGE (jun_image
                                                                     )));
      int src_width = gdk_pixbuf_get_width (src_pixbuf);
      int src_height = gdk_pixbuf_get_height (src_pixbuf);

      int new_width = DEFAULT_IMAGE_WIDTH;
      int new_height = new_width * src_height / src_width;
      current_pixbuf = gdk_pixbuf_scale_simple (src_pixbuf,
                                                new_width,
                                                new_height,
                                                GDK_INTERP_HYPER);
      gtk_image_set_from_pixbuf (GTK_IMAGE (jun_image), current_pixbuf);

      gtk_window_resize (GTK_WINDOW (main_window), new_width, new_height);
    }
  else
    {
      src_pixbuf = NULL;
      current_pixbuf = NULL;

      if (image_type == GTK_IMAGE_ANIMATION)
        {
          GdkPixbufAnimation *ani;
          ani = gtk_image_get_animation ( GTK_IMAGE (jun_image));
          int new_width = gdk_pixbuf_animation_get_width (ani);
          int new_height = gdk_pixbuf_animation_get_height (ani);

          gtk_window_resize (GTK_WINDOW (main_window), new_width, new_height);
        }
    }
}

static void
load_images_from_directory (const char *dir)
{
  file_list = get_reg_files_in_dir(dir);
}

static void
select_image_dir ()
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Open File",
                                        GTK_WINDOW (main_window),
                                        action,
                                        "Cancel",
                                        GTK_RESPONSE_CANCEL,
                                        "Open",
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);

  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT)
    {
      char *filename;
      GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
      filename = gtk_file_chooser_get_filename (chooser);
      load_images_from_directory(filename);
      g_free (filename);
    }

  gtk_widget_destroy (dialog);

  load_random_jun ();
}

static void
save_current_jun ()
{
  GtkImageType image_type = gtk_image_get_storage_type (GTK_IMAGE (jun_image));
  if (image_type != GTK_IMAGE_PIXBUF)
    {
      GtkWidget *error_dialog;
      error_dialog = gtk_message_dialog_new (GTK_WINDOW (main_window),
                                       GTK_DIALOG_MODAL,
                                       GTK_MESSAGE_ERROR,
                                       GTK_BUTTONS_OK,
                                       "Error saving image. It might be a "
                                       "gif, which isn't supported yet.");
      gtk_dialog_run (GTK_DIALOG (error_dialog));
      gtk_widget_destroy (error_dialog);
    }
  else
    {
      GtkWidget *dialog;
      GtkFileChooser *chooser;
      GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
      gint res;

      dialog = gtk_file_chooser_dialog_new ("Save File",
                                            GTK_WINDOW (main_window),
                                            action,
                                            "Cancel",
                                            GTK_RESPONSE_CANCEL,
                                            "Save",
                                            GTK_RESPONSE_ACCEPT,
                                            NULL);
      chooser = GTK_FILE_CHOOSER (dialog);

      gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);
      gtk_file_chooser_set_current_name (chooser,
                                         "Untitled image.png");

      res = gtk_dialog_run (GTK_DIALOG (dialog));
      if (res == GTK_RESPONSE_ACCEPT)
        {
          char *filename;
          filename = gtk_file_chooser_get_filename (chooser);

          if (src_pixbuf != NULL)
            {
              gdk_pixbuf_save(src_pixbuf, filename, "png", NULL, NULL);
            }
          else
            {
              dialog = gtk_message_dialog_new (GTK_WINDOW (main_window),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_CLOSE,
                                               "Error saving image.");
              gtk_dialog_run (GTK_DIALOG (dialog));
              gtk_widget_destroy (dialog);
            }
          g_free (filename);
        }

      gtk_widget_destroy (dialog);
    }
}

static void
on_quit_item_activate ()
{
  gtk_widget_destroy (GTK_WIDGET (main_window));
}

static void
on_about_activate ()
{
  gtk_show_about_dialog (NULL,
                         "program-name", "JunViewerC",
                         "title" "About JunViewerC",
                         NULL);
}

static void
set_image_to_src_size ()
{
  GtkImageType image_type = gtk_image_get_storage_type (GTK_IMAGE (jun_image));
  if (image_type == GTK_IMAGE_PIXBUF && src_pixbuf != NULL)
    {
      current_pixbuf = gdk_pixbuf_copy (src_pixbuf);
      gtk_image_set_from_pixbuf (GTK_IMAGE (jun_image), current_pixbuf);

      int new_width = gdk_pixbuf_get_width (current_pixbuf);
      int new_height = gdk_pixbuf_get_height (current_pixbuf);

      gtk_window_resize (GTK_WINDOW (main_window), new_width, new_height);
    }
}

static void
set_image_to_default_size ()
{
  GtkImageType image_type;
  image_type = gtk_image_get_storage_type (GTK_IMAGE (jun_image));
  if (image_type == GTK_IMAGE_PIXBUF && src_pixbuf != NULL)
    {
      int src_width = gdk_pixbuf_get_width (src_pixbuf);
      int src_height = gdk_pixbuf_get_height (src_pixbuf);

      int new_width = DEFAULT_IMAGE_WIDTH;
      int new_height = new_width * src_height / src_width;

      current_pixbuf = gdk_pixbuf_scale_simple (src_pixbuf,
                                                new_width,
                                                new_height,
                                                GDK_INTERP_HYPER);
      gtk_image_set_from_pixbuf (GTK_IMAGE (jun_image), current_pixbuf);

      gtk_window_resize (GTK_WINDOW (main_window), new_width, new_height);
    }
}

/*static void*/
/*on_size_allocate(GtkWidget *widget,*/
                 /*GdkRectangle *allocation,*/
                 /*gpointer user_data)*/
/*{*/
  /*GtkImageType image_type = gtk_image_get_storage_type (GTK_IMAGE (jun_image));*/

  /*if (image_type == GTK_IMAGE_PIXBUF)*/
    /*{*/
      /*int new_width = allocation->width;*/
      /*int new_height = allocation->height;*/
      /*printf("allocated size %i\n", new_height);*/
      /*current_pixbuf = gdk_pixbuf_scale_simple (src_pixbuf,*/
                                                /*new_width,*/
                                                /*new_height,*/
                                                /*GDK_INTERP_BILINEAR);*/
      /*gtk_image_set_from_pixbuf (GTK_IMAGE (jun_image), current_pixbuf);*/
    /*}*/
/*}*/
