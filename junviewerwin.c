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
  GtkWidget *jun_image;
  GtkWidget *next_jun_button;
  GtkWidget *jun_image_box;

  GList *image_files;
  GdkPixbuf *src_pixbuf;
};

G_DEFINE_TYPE_WITH_PRIVATE (JunViewerWindow, jun_viewer_window,
                            GTK_TYPE_APPLICATION_WINDOW);

static void
quit_activated (GSimpleAction *action,
                GVariant *parameter,
                gpointer win)
{
  GApplication *app;
  app = G_APPLICATION (gtk_window_get_application (GTK_WINDOW (win)));
  g_application_quit (app);
}

static void
next_jun_activated (GSimpleAction *action,
                    GVariant *parameter,
                    gpointer win)
{
  jun_viewer_window_load_random_jun (win);
}

static void
set_image_to_src_size_activated (GSimpleAction *action,
                                 GVariant *parameter,
                                 gpointer win)
{
  jun_viewer_window_set_image_to_src_size (win);
}

static void
set_image_to_default_size_activated (GSimpleAction *action,
                                 GVariant *parameter,
                                 gpointer win)
{
  jun_viewer_window_set_image_to_default_size (win);
}

static void
open_activated (GSimpleAction *action,
                GVariant *parameter,
                gpointer win)
{
  jun_viewer_window_select_dir_dialog (JUN_VIEWER_WINDOW (win));
}

static void
save_activated (GSimpleAction *action,
                GVariant *parameter,
                gpointer win)
{
  jun_viewer_window_save_current_dialog (JUN_VIEWER_WINDOW (win));
}

/*static void*/
/*new_activated (GSimpleAction *action,*/
               /*GVariant *parameter,*/
               /*gpointer win)*/
/*{*/
  /*JunViewerApp *app = jun_viewer_app_new ();*/
  /*g_application_run (G_APPLICATION (app), 0, NULL);*/
/*}*/

static void
about_activated (GSimpleAction *action,
                 GVariant *parameter,
                 gpointer win)
{
  jun_viewer_window_about (win);
}

static GActionEntry win_entries[] =
{
    {"quit", quit_activated, NULL, NULL, NULL},
    {"next-jun", next_jun_activated, NULL, NULL, NULL},
    {"set-image-to-src-size", set_image_to_src_size_activated,
      NULL, NULL, NULL},
    {"set-image-to-default-size", set_image_to_default_size_activated,
      NULL, NULL, NULL},
    {"open", open_activated, NULL, NULL, NULL},
    {"save", save_activated, NULL, NULL, NULL},
    /*{"new", new_activated, NULL, NULL, NULL}*/
    {"about", about_activated, NULL, NULL, NULL}
};

static void
jun_viewer_window_init (JunViewerWindow *win)
{
  JunViewerWindowPrivate *priv;

  priv = jun_viewer_window_get_instance_private (win);
  gtk_widget_init_template (GTK_WIDGET (win));

  g_action_map_add_action_entries ( G_ACTION_MAP (win),
                                   win_entries,
                                   G_N_ELEMENTS (win_entries),
                                   win);

  g_signal_connect_swapped (priv->jun_image_box, "button-press-event",
                            G_CALLBACK (jun_viewer_window_load_random_jun),
                            win);
  g_signal_connect_swapped (priv->next_jun_button, "clicked",
                            G_CALLBACK (jun_viewer_window_load_random_jun),
                            win);
}

static void
jun_viewer_window_class_init (JunViewerWindowClass *class)
{
  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (class),
                                               "/com/waataja/junviewer/"
                                               "window.ui");
  gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class),
                                                JunViewerWindow,
                                                jun_image);
  gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class),
                                                JunViewerWindow,
                                                next_jun_button);
  gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (class),
                                                JunViewerWindow,
                                                jun_image_box);
}

JunViewerWindow *
jun_viewer_window_new (JunViewerApp *app)
{
  JunViewerWindow *win = g_object_new (JUN_VIEWER_WINDOW_TYPE, "application",
                                       app, NULL);
  jun_viewer_window_load_images_from_dir (win, "Jun");
  jun_viewer_window_load_random_jun (win);
  return win;
}

JunViewerWindow *
jun_viewer_window_new_in_dir (JunViewerApp *app, const char *image_dir)
{
  JunViewerWindow *win = g_object_new (JUN_VIEWER_WINDOW_TYPE,
                                       "application",
                                       app,
                                       NULL);
  /*jun_viewer_window_load_images_from_dir (win);*/
  return win;
}

GList *
get_reg_files_in_dir (const char *dir_path)
{

  if (dir_path == NULL)
    {
      return NULL;
    }

  GFile *file_dir = g_file_new_for_path (dir_path);
  GList *file_list = NULL;
  GFileType file_type;
  file_type = g_file_query_file_type (file_dir,
                                      G_TYPE_FILE_QUERY_INFO_FLAGS, NULL);
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
  else
    {
      return NULL;
    }

  return file_list;
}

void
jun_viewer_window_load_image_from_file (JunViewerWindow *win,
                                 GFile *image_file)
{
  JunViewerWindowPrivate *priv = jun_viewer_window_get_instance_private (win);
  GtkImage *jun_image = GTK_IMAGE (priv->jun_image);
  gtk_image_set_from_file (jun_image,
                           g_file_get_path (image_file));

  GtkImageType image_type;
  image_type = gtk_image_get_storage_type (jun_image);

  if (image_type == GTK_IMAGE_PIXBUF)
    {
      GdkPixbuf *src_pixbuf;
      GdkPixbuf *image_pixbuf;
      image_pixbuf = gtk_image_get_pixbuf (jun_image);
      src_pixbuf = gdk_pixbuf_copy (image_pixbuf);

      priv->src_pixbuf = src_pixbuf;

      int src_width = gdk_pixbuf_get_width (src_pixbuf);
      int src_height = gdk_pixbuf_get_height (src_pixbuf);

      int new_width = DEFAULT_IMAGE_WIDTH;
      int new_height = new_width * src_height / src_width;

      GdkPixbuf *new_pixbuf = gdk_pixbuf_scale_simple (src_pixbuf,
                                                       new_width,
                                                       new_height,
                                                       GDK_INTERP_HYPER);
      gtk_image_set_from_pixbuf (jun_image, new_pixbuf);
    }

  jun_viewer_window_resize_to_image (win);
}

void
jun_viewer_window_load_random_jun (JunViewerWindow *win)
{
  JunViewerWindowPrivate *priv = jun_viewer_window_get_instance_private (win);

  int image_count = g_list_length (priv->image_files);

  if (image_count != 0)
    {
      int rand_index = rand() % image_count;

      GFile *image_file = g_list_nth (priv->image_files, rand_index)->data;
      jun_viewer_window_load_image_from_file (win, image_file);
    }
}

void
jun_viewer_window_load_images_from_dir (JunViewerWindow *win,
                                        const char *image_dir)
{
  JunViewerWindowPrivate *priv = jun_viewer_window_get_instance_private (win);
  priv->image_files = get_reg_files_in_dir (image_dir);
  /*jun_viewer_window_load_random_jun (win);*/
}

void
jun_viewer_window_resize_to_image (JunViewerWindow *win)
{
  JunViewerWindowPrivate *priv = jun_viewer_window_get_instance_private (win);

  GtkImage *jun_image = GTK_IMAGE (priv->jun_image);

  GtkImageType image_type = gtk_image_get_storage_type (jun_image);

  if (image_type == GTK_IMAGE_PIXBUF)
    {
      GdkPixbuf *current_pixbuf = gtk_image_get_pixbuf (jun_image);
      int width = gdk_pixbuf_get_width (current_pixbuf);
      int height = gdk_pixbuf_get_height (current_pixbuf);

      gtk_window_resize (GTK_WINDOW (win), width, height);
    }
  else if (image_type == GTK_IMAGE_ANIMATION)
    {
      GdkPixbufAnimation *ani = gtk_image_get_animation (jun_image);
      int width = gdk_pixbuf_animation_get_width (ani);
      int height = gdk_pixbuf_animation_get_height (ani);

      gtk_window_resize (GTK_WINDOW (win), width, height);
    }
}

void
jun_viewer_window_set_image_to_src_size (JunViewerWindow *win)
{
  JunViewerWindowPrivate *priv = jun_viewer_window_get_instance_private (win);

  GtkImageType image_type;
  image_type = gtk_image_get_storage_type (GTK_IMAGE (priv->jun_image));
  if (image_type == GTK_IMAGE_PIXBUF && priv->src_pixbuf != NULL)
    {
      gtk_image_set_from_pixbuf (GTK_IMAGE (priv->jun_image),
                                 priv->src_pixbuf);
    }
  jun_viewer_window_resize_to_image (win);
}

void
jun_viewer_window_set_image_to_default_size (JunViewerWindow *win)
{
  JunViewerWindowPrivate *priv = jun_viewer_window_get_instance_private (win);

  GtkImageType image_type;
  image_type = gtk_image_get_storage_type (GTK_IMAGE (priv->jun_image));
  if (image_type == GTK_IMAGE_PIXBUF && priv->src_pixbuf != NULL)
    {
      int src_width = gdk_pixbuf_get_width (priv->src_pixbuf);
      int src_height = gdk_pixbuf_get_height (priv->src_pixbuf);

      int new_width = DEFAULT_IMAGE_WIDTH;
      int new_height = new_width * src_height / src_width;

      GdkPixbuf *new_pixbuf;
      new_pixbuf = gdk_pixbuf_scale_simple (priv->src_pixbuf,
                                            new_width,
                                            new_height,
                                            GDK_INTERP_HYPER);
      gtk_image_set_from_pixbuf (GTK_IMAGE (priv->jun_image), new_pixbuf);
    }
  jun_viewer_window_resize_to_image (win);
}

void
jun_viewer_window_select_dir_dialog (JunViewerWindow *win)
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
  int result;

  dialog = gtk_file_chooser_dialog_new ("Open Directory",
                                        GTK_WINDOW (win),
                                        action,
                                        "Cancel",
                                        GTK_RESPONSE_CANCEL,
                                        "Open",
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);

  result = gtk_dialog_run (GTK_DIALOG (dialog));

  if (result == GTK_RESPONSE_ACCEPT)
    {
      char *filename;
      GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
      filename = gtk_file_chooser_get_filename (chooser);
      jun_viewer_window_load_images_from_dir (win, filename);
      jun_viewer_window_load_random_jun (win);
      filename = gtk_file_chooser_get_filename (chooser);
      g_free (filename);
    }

  gtk_widget_destroy (dialog);

  jun_viewer_window_load_random_jun (win);
}

void
jun_viewer_window_save_current_dialog (JunViewerWindow *win)
{
  JunViewerWindowPrivate *priv = jun_viewer_window_get_instance_private (win);

  GtkImageType image_type = gtk_image_get_storage_type (GTK_IMAGE (priv->jun_image));
  
  if (image_type != GTK_IMAGE_PIXBUF)
    {
      GtkWidget *error_dialog;
      /* I forget if this will be destroyed or not.  */
      char *error_message = "Can't save image. It's not in an image format"
        " and might be an animation.";
      error_dialog = gtk_message_dialog_new (GTK_WINDOW (win),
                                             GTK_DIALOG_MODAL,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_OK,
                                             "%s",
                                             error_message);
      gtk_dialog_run (GTK_DIALOG (error_dialog));
      gtk_widget_destroy (error_dialog);
    }
  else
    {
      GtkWidget *dialog;
      GtkFileChooser *chooser;
      GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
      int result;

      dialog = gtk_file_chooser_dialog_new ("Save Image",
                                            GTK_WINDOW (win),
                                            action,
                                            "Cance",
                                            GTK_RESPONSE_CANCEL,
                                            "Save",
                                            GTK_RESPONSE_ACCEPT,
                                            NULL);
      chooser = GTK_FILE_CHOOSER (dialog);

      gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);
      gtk_file_chooser_set_current_name (chooser,
                                         "Untitled Image.png");

      result = gtk_dialog_run (GTK_DIALOG (dialog));

      if (result == GTK_RESPONSE_ACCEPT)
        {
          char *filename;
          filename = gtk_file_chooser_get_filename (chooser);

          if (priv->src_pixbuf != NULL)
            {
              gdk_pixbuf_save (priv->src_pixbuf, filename, "png", NULL, NULL);
            }
          else
            {
              dialog = gtk_message_dialog_new (GTK_WINDOW (win),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_OK,
                                               "Error, couldn't get the image data.");
              gtk_dialog_run (GTK_DIALOG (dialog));
              gtk_widget_destroy (dialog);
            }
          g_free (filename);
        }
      gtk_widget_destroy (dialog);
    }
}

void
jun_viewer_window_about (JunViewerWindow *win)
{
  /* This is weird. It worked before but now using the authors thing gives a
     segmentation fault I think.  */
  /*char *authors = "Jason Waataja";*/

  gtk_show_about_dialog (GTK_WINDOW (win),
                         "program-name", "JunViewerC",
                         "title", "About JunViewerC",
                         /*"authors", &authors,*/
                         "copyright", "Copyright (C) 2016 Jason Waataja",
                         "comments", "JunViewerC is an application to view"
                         " Jun from a set of random images but you can also"
                         " use your own.",
                         "version", "Version 0.10",
                         "logo-icon-name", "image-x-generic",
                         NULL);
}
