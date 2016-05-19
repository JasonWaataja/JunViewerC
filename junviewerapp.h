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

#ifndef JUNVIEWERAPP_H
#define JUNVIEWERAPP_H

#include <gtk/gtk.h>

#define JUN_VIEWER_APP_TYPE (jun_viewer_app_get_type ())
#define JUN_VIEWER_APP(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), JUN_VIEWER_APP_TYPE, JunViewerApp))

typedef struct _JunViewerApp JunViewerApp;
typedef struct _JunViewerAppClass JunViewerAppClass;

GType jun_viewer_app_get_type(void);
JunViewerApp *jun_viewer_app_new(void);

#endif /* JUNVIEWERAPP_H */
