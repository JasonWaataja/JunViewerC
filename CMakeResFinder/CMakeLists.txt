cmake_minimum_required(VERSION 3.0)
project(CMakeResFinder)

configure_file (
   "${PROJECT_SOURCE_DIR}/src/cmrfconfig.h.in"
   "${PROJECT_BINARY_DIR}/cmrfconfig.h"
   )

include_directories("${PROJECT_BINARY_DIR}")

include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB SOURCES src/cmrf.c)
add_library(cmrf ${SOURCES})

install (TARGETS cmrf EXPORT cmrf
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib/static
   INCLUDES DIRECTORY include/cmrf
   )

install (FILES src/cmrf.h
   DESTINATION include/cmrf)

install(EXPORT cmrf
   NAMESPACE cmrf_ 
   DESTINATION lib/cmrf)

include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE  
	"${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

set (CPACK_SOURCE_IGNORE_FILES
	.vimrc
	.ycm_extra_conf
	)

include (CPack)
