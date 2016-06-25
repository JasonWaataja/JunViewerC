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

#ifndef __JUNVIEWERWIN_H
#define __JUNVIEWERWIN_H

#include <gtk/gtk.h>
#include "junviewerapp.h"

#define JUN_VIEWER_WINDOW_TYPE (jun_viewer_window_get_type ())
#define JUN_VIEWER_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), JUN_VIEWER_WINDOW_TYPE, JunViewerWindow))

typedef struct _JunViewerWindow JunViewerWindow;
typedef struct _JunViewerWindowClass JunViewerWindowClass;

#define DEFAULT_IMAGE_WIDTH 540

GType
jun_viewer_window_get_type (void);

JunViewerWindow *
jun_viewer_window_new (JunViewerApp *app);

JunViewerWindow *
jun_viewer_window_new_in_dir (JunViewerApp *app,
                              const char *image_dir);

void
jun_viewer_window_open (JunViewerWindow *win);

void
jun_viewer_window_load_image_from_file (JunViewerWindow *win, GFile *image_file);

void
jun_viewer_window_load_random_jun (JunViewerWindow *win);

void
jun_viewer_window_load_images_from_dir (JunViewerWindow *win, const char *image_dir);

void
jun_viewer_window_resize_to_image (JunViewerWindow *win);

//void
//jun_viewer_window_select_image_dir (JunViewerWindow *win);

GList *
get_reg_files_in_dir (const char *dir_path);

void
jun_viewer_window_set_image_to_src_size (JunViewerWindow *win);

void
jun_viewer_window_set_image_to_default_size (JunViewerWindow *win);

#endif /* __JUNVIEWERWIN_H */
