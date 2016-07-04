#ifndef CMRF_H
#define CMRF_H

#include <stdarg.h>


/* Initialize the cmrf library with the given absolute paths.  This functions
 * expects a set of char * and n_args should be set to the number of strings
 * passed.  It returns whether or not it was successful.  Make sure to call the
 * cmrf_exit funtion after this.  */
int cmrf_init (int n_args, ...);

/* Clean up the resources used by the cmrf library.  It primarily frees the
 * resources that store the paths to search.  Returns whether or not it was
 * successful.  */
int cmrf_exit ();

/* Adds a search path to be searched.  This path will be appended to whatever
 * CMAKE_INSTALL_PREFIX is.  Returns whether or not it was successful.  */
int cmrf_add_path_cmake_prefix (const char *dir_name);

/* Adds a search path with the prefix "./" to thes search paths.  Returns
 * whether or not it was successful.  */
int cmrf_add_path_relative (const char *dir_name);

/* Adds a search path directly without any modifications.  Returns whether or
 * not it succeeded.  */
int cmrf_add_path_absolute (const char *dir_name);

/* Tests whether or not the given path is contained in the search paths.  */
int cmrf_is_in_search_paths (const char *dir_name);

/* Searches the paths in the order that they were added. It tests to see if
 * there exists a file or directory with the given name if dir_name is
 * prepeneded to it.  It returns a string with the full path and NULL,
 * otherwise.  */
char *cmrf_find_resource (const char *res_name);

/* Returns the value of CMAKE_INSTALL_PREFIX for the current platform.  */
const char *
cmrf_get_cmake_install_prefix ();

#endif /* CMRF_H */
