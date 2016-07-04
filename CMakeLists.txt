cmake_minimum_required(VERSION 3.0)
project(JunViewerC)

set(SOURCES src/main.c src/junviewerapp.c src/junviewerwin.c)

find_package(PkgConfig REQUIRED)

execute_process(
  COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=glib_compile_resources gio-2.0 OUTPUT_VARIABLE GLIB_COMPILE_RESOURCES
  OUTPUT_STRIP_TRAILING_WHITESPACE)

#message(${GLIB_COMPILE_RESOURCES})

#add_custom_command(
  #OUTPUT resources.c
  #COMMAND ${GLIB_COMPILE_RESOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/exampleapp.gresource.xml --target=resources.c --sourcedir=${CMAKE_CURRENT_SOURCE_DIR} --generate-source
  #)
  add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/resources.c
	COMMAND ${GLIB_COMPILE_RESOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/res/ui/junviewer.gresource.xml --target=${CMAKE_CURRENT_SOURCE_DIR}/src/resources.c --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}/res --generate-source
  )


#message(${GLIB_COMPILE_RESOURCES} exampleapp.gresource.xml --target=resources.c --sourcedir=${CMAKE_CURRENT_SOURCE_DIR} --generate-source)

#add_executable(exampleapp ${SOURCES} resources.c)
add_executable(jvc ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/resources.c)
#add_executable(jvc ${SOURCES} resources.c)

#add_custom_command(TARGET jvc
  #PRE_BUILD
  #COMMAND ${GLIB_COMPILE_RESOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/junviewer.gresource.xml --target=resources.c --sourcedir=${CMAKE_CURRENT_SOURCE_DIR} --generate-source
  #)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})
#target_link_libraries(exampleapp ${GTK3_LIBRARIES})
target_link_libraries(jvc ${GTK3_LIBRARIES})

#add_definitions(-export-dynamic)

#add_custom_command(TARGET jvc
  #POST_BUILD
  #COMMAND ${CMAKE_COMMAND} -E copy_if_different
  #${CMAKE_CURRENT_SOURCE_DIR}/window.ui
  #$<TARGET_FILE_DIR:jvc>)
add_custom_command(TARGET jvc
  POST_BUILD
  #COMMAND ${CMAKE_COMMAND} -E make_directory
  #COMMAND ${CMAKE_COMMAND} -E copy_if_different
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/Jun
  $<TARGET_FILE_DIR:jvc>/Jun)

add_subdirectory (CMakeResFinder)
include_directories (CMakeResFinder/src)
target_link_libraries (jvc cmrf)
