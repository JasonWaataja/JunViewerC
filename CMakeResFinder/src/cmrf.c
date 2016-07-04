#include "cmrf.h"

#include "cmrfconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>

#define INITIAL_SEARCH_PATHS_SIZE 3

/* True if the library init function has been called, false otherwise.  */
static int init_status;

static char **search_paths;

static unsigned int search_path_count;

static int search_paths_size = INITIAL_SEARCH_PATHS_SIZE;

static int
add_to_search_paths (char *new_dir);

int
cmrf_init (int n_args, ...)
{
  search_path_count = (n_args > 0) ? n_args : 0;

  search_paths_size = (n_args > INITIAL_SEARCH_PATHS_SIZE) 
    ? n_args : INITIAL_SEARCH_PATHS_SIZE;

  search_paths = (char **) malloc (sizeof (char *) * (search_paths_size));

  if (!search_paths)
    {
      fprintf (stderr, "cmrf: Error allocating memory.\n");
      init_status = 0;
      return 0;
    }

  va_list arg;
  va_start (arg, n_args);
  for (int i = 0; i < n_args; i++)
    {
      const char *temp_str = va_arg (arg, const char *);
      if (temp_str != NULL)
	{
	  size_t length = strlen (temp_str);
	  search_paths[i] = (char *) malloc (sizeof (char) * (length +1));
	  if (!search_paths[i])
	    {
	      fprintf (stderr, "cmrf: Error allocating memory.\n");
	      init_status = 0;
	      return 0;
	    }
	  strcpy (search_paths[i], temp_str);
	}

    }
  va_end (arg);
  init_status = 1;
  return 1;
}

int
cmrf_exit ()
{
  for (int i = 0; i < search_path_count; i++)
    {
      free (search_paths[i]);
      search_paths[i] = NULL;
    }
  free (search_paths);
  return 1;
}

int
cmrf_add_path_cmake_prefix (const char *dir_name)
{

  /* I don't know if it's better to just do +1 at the end.  */
  size_t prefix_len = strlen (CMAKE_INSTALL_PREFIX) + strlen ("/");
  size_t dir_name_len = strlen (dir_name);
  size_t total_len = prefix_len + dir_name_len;

  char *search_path_absolute = (char *) malloc (sizeof (char) * (total_len + 1));

  if (!search_path_absolute)
    {
      fprintf (stderr, "cmrf: Error allocating new search path.\n");
      return 0;
    }

  /* This could be done in one long line.  */
  strcpy (search_path_absolute, CMAKE_INSTALL_PREFIX);
  strcat (search_path_absolute, "/");
  strcat (search_path_absolute, dir_name);

  return add_to_search_paths (search_path_absolute);
}

int
cmrf_add_path_absolute (const char *dir_name)
{
  int total_len = strlen (dir_name);
  char *new_search_path = (char *) malloc (sizeof (char) * (total_len + 1));

  if (!new_search_path)
    {
      fprintf (stderr, "cmrf: Error allocating new search path.\n");
      return 0;
    }

  strcpy (new_search_path, dir_name);

  return add_to_search_paths (new_search_path);
}

int
cmrf_add_path_relative (const char *dir_name)
{
  char relative_prefix[] = "./";
  size_t prefix_len = strlen (relative_prefix);
  size_t dir_name_len = strlen (dir_name);
  size_t total_len = prefix_len + dir_name_len;

  char *new_search_path = (char *) malloc (sizeof (char) * (total_len + 1));

  if (!new_search_path)
    {
      fprintf (stderr, "cmrf: Error allocating new search path\n");
      return 0;
    }

  strcpy (new_search_path, relative_prefix);
  strcat (new_search_path, dir_name);

  return add_to_search_paths (new_search_path);
}

int
cmrf_is_in_search_paths (const char *dir_name)
{
  int path_found = 0;
  for (int i = 0; i < search_path_count && !path_found; i++)
    {
      if (strcmp (search_paths[i], dir_name) == 0)
	{
	  path_found = 1;
	  return 1;
	}
    }
  return 0;
}

char *cmrf_find_resource (const char *res_name)
{

  int found_res = 0;

  size_t res_name_len = strlen (res_name);

  for (int i = 0; i < search_path_count && !found_res; i++)
    {
      size_t prefix_len = strlen (search_paths[i]) + strlen ("/");
      size_t total_len = prefix_len + res_name_len;

      char *full_res_path = (char *) malloc (sizeof (char) * (total_len + 1));
      if (!full_res_path)
	{
	  return NULL;
	}
      strcpy (full_res_path, search_paths[i]);
      strcat (full_res_path, "/");
      strcat (full_res_path, res_name);

      /* Apparently, you can do this with access as well but neither way works
       * with files over 2 gigs which might be a problem.  */

      struct stat file_info;
      if (stat (full_res_path, &file_info) == 0)
	{
	  return full_res_path;
	}
      else
	{
	  free (full_res_path);
	}
    }
  return NULL;
}

static int
add_to_search_paths (char *new_dir)
{
  int new_count = search_path_count + 1;
  if (new_count > search_paths_size)
    {
      int new_size = search_paths_size + 1;
      if (!realloc (search_paths, sizeof (char *) * new_size))
	{
	  fprintf (stderr, "cmrf: Error adding new search path to memory.\n");
	  return 0;
	}
      search_paths_size = new_size;
    }
  search_paths[search_path_count] = new_dir;
  search_path_count = new_count;
  return 1;
}

const char *
cmrf_get_cmake_install_prefix ()
{
  return CMAKE_INSTALL_PREFIX;
}
