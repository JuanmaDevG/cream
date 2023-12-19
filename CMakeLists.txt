cmake_minimum_required(VERSION 3.22)
project(cargs VERSION 2.0 C)

option(BUILD_SHARED_LIBS "Build the DLL or Shared Object version of the library?")
option(USE_O3_OPTIMIZATION "Use compiler's level of optimization 3?")

if(${USE_O3_OPTIMIZATION})
    list(APPEND CMAKE_C_FLAGS "-O3")
endif()

include_directories(BEFORE ${CMAKE_SOURCE_DIR}/include/internal)    # For preprocessed files
aux_source_directory(                                               # Getting all source filenames
    ${CMAKE_SOURCE_DIR}/src 
    src_files
)

add_library(cargs ${src_files})

#Test files (subdirs can access parent scope)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)