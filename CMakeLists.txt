cmake_minimum_required(VERSION 3.0.0)
set(PROJECT_NAME inDepthCpp)
set(CMAKE_CXX_STANDARD 11)

set(LOG_LEVEL 0xf CACHE STRING INTERNAL)

option(OUTPUT_EXECUTABLE "output executable(YES)? or API lib(No)" YES)
option(OUTPUT_TESTPROJECT "output test project(YES)? or (No)" NO)
option(OUTPUT_SVN_VERSION "enbedded SVN Rev nouber in code? " NO)

project(${PROJECT_NAME})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/Targets)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/Targets)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/Targets)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/Targets)

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DLL")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
else()
    set(CMAKE_C_FLAGS "-m32")
    set(CMAKE_CXX_FLAGS "-m32")
endif()

if(OUTPUT_SVN_VERSION)
    add_definitions(-DSVN_OPTION)
endif()

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-DCMODEL_CODE)

if((LOG_LEVEL less 0x10) AND (LOG_LEVEL GREATER -1))
    message("LOG_LEVEL={LOG_LEVEL}")
    add_definitions(-DLOG_LEVEL=${LOG_LEVEL})
else()
    message("LOG_LEVEL error, use default value oxf")
    add_definitions(-DLOG_LEVEL=0xf)
endif()

if(OUTPUT_TESTPROJECT)
    add_definitions(-DGTEST)
endif()

if(OUTPUT_EXECUTABLE)
    add_definitions(-DCMODEL_DLL_OPTION=0)
else()
    add_definitions(-DCMODEL_DLL_OPTION=1)
endif()

# set(PROJECT_MODULES SQLite)

# foreach(FOLDER IN LISTS PROJECT_MODULES)
#     include_directories(${FOLDER})
# endforeach()

# include_directories(SQLite)
# file(GLOB_RECURSE CURRENT_HEADERS SQLite/include/*h)
# source_group("Include" FILES ${CURRENT_HEADERS})

# include_directories(include/SQLite)
add_library(sqlite3 include/SQLite/sqlite3.c include/SQLite/sqlite3ext.h)

aux_source_directory(Main DIR_MAIN_SRC)

foreach(FOLDER IN LISTS PROJECT_MODULES)
    add_subdirectory(${FOLDER})
endforeach()

if(OUTPUT_TESTPROJECT)
    add_subdirectory(UnitTest)
endif()

if(OUTPUT_EXECUTABLE)
    add_executable(${PROJECT_NAME} ${DIR_MAIN_SRC} ${CURRENT_HEADERS})

    if(MSVC)
        target_link_libraries(${PROJECT_NAME} ${PROJECT_MODULES} sqlite3)
    else()
        target_link_libraries(${PROJECT_NAME}
        -Wl,--start-group
        ${PROJECT_MODULES}
        -Wl,--end-group)
    endif()
else()
endif()

if(OUTPUT_SVN_VERSION)
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -D SOURCE_DIR={PROJECT_SOURCE_DIR} -P ${PROJECT_SOURCE_DIR}/Main/SVNVersion.cmake VERBATIM)
endif()
