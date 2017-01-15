cmake_minimum_required (VERSION 3.6)

set(META_PROJECT_NAME        "traceraptor")
set(META_PROJECT_DESCRIPTION "A simple ray tracer")
set(META_AUTHOR_NAME         "Bogdan Gochev")
set(META_AUTHOR_DOMAIN       "http://bogdangochev.me")
set(META_AUTHOR_MAINTAINER   "bogdan.dgochev@gmail.com")
set(META_VERSION_MAJOR       "2")
set(META_VERSION_MINOR       "0")

project(${META_PROJECT_NAME} C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

include(cmake/CompileOptions.cmake)

set(CUSTOM_INCLUDE_DIRECTORIES)
list(APPEND CUSTOM_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/lib")
list(APPEND CUSTOM_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/src")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(src)
add_subdirectory(test)
