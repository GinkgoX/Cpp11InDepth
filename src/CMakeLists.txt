file(GLOB_RECURSE CURRENT_HEADERS *.h)
source_group("Include" FILES ${CURRENT_HEADERS})

aux_source_directory(. source)
message("current module ${CMAKE_CURRENT_SOURCE_DIR}")
string(REGEX MATCH "([^/]*)$" MODULE_NAME ${CMAKE_CURRENT_SOURCE_DIR})
add_library(${MODULE_NAME} ${source} ${CURRENT_HEADERS})
# cmake_minimum_required(VERSION 3.0.0)
# include_directories("../include")
# add_library(sqlite3 ../include/sqlite3.c)