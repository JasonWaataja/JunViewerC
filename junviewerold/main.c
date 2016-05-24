#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "junviewerapp.h"

int main(int argc, char** argv)
{
  return g_application_run(G_APPLICATION(jun_viewer_app_new()), argc, argv);
}
