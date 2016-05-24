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

#ifndef JUNVIEWERWIN_H
#define JUNVIEWERWIN_H

#include <gtk/gtk.h>
#include "junviewerapp.h"

#define JUN_VIEWER_WINDOW_TYPE (jun_viewer_window_get_type ())
#define JUN_VIEWER_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), JUN_VIEWER_WINDOW_TYPE, JunViewerWindow))

typedef struct _JunViewerWindow JunViewerWindow;
typedef struct _JunViewerWindowClass JunViewerWindowClass;

GType jun_viewer_window_get_type(void);
JunViewerWindow *jun_viewer_window_new(JunViewerApp *app);
void jun_viewer_window_open(JunViewerWindow *win, GFile *file);

/* const char *jun_directory = "res"; */

GList *get_reg_files_in_dir(const char *dir_path);
GFile *get_random_jun_file();
GList *get_jun_list();
void jun_viewer_window_load_random_jun(JunViewerWindow* self);

#endif /* JUNVIEWERWIN_H */
