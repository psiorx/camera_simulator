cmake_minimum_required(VERSION 2.8.3)
project(lcmnode)
include_directories(${CMAKE_SOURCE_DIR}/include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#include(ExternalProject)
#ExternalProject_Add(project_libsocket
#        PREFIX libsocket
#        GIT_REPOSITORY https://github.com/dermesser/libsocket.git
#        GIT_TAG 4cd8da1320640c8358cc7b9aafa29bd75808c4d7
#)

add_subdirectory(src)
