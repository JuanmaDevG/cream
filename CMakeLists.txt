cmake_minimum_required(VERSION 3.26)
project(cargs VERSION 1.1)

option(BUILD_SHARED_LIBS "Build the DLL or Shared Object version of the library?")

include_directories(BEFORE ${CMAKE_SOURCE_DIR}/include/internal)    # For preprocessed files
aux_source_directory(                                               #Getting source filenames (with path)
    ${CMAKE_SOURCE_DIR}/src 
    src_files
)

add_library(cargs ${src_files})