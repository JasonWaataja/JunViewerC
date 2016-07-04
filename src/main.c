/*
Copyright (C) 2016 Jason Waataja

This file is part of JunViewerC.

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

#include "junviewerapp.h"
#include "junviewerwin.h"
#include "cmrf.h"


int
main (int argc,
     char *argv[])
{
  srand (time (NULL));

  cmrf_init (0);
  cmrf_add_path_relative ("res");
  cmrf_add_path_cmake_prefix ("share/jvc");

  JunViewerApp *app = jun_viewer_app_new ();
  int result = g_application_run (G_APPLICATION (app), argc, argv);

  cmrf_exit ();
  return result;
}
