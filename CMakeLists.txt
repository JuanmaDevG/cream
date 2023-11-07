cmake_minimum_required(VERSION 3.26)
project(cargs VERSION 1.1)

option(BUILD_SHARED_LIBS "Build the DLL or Shared Object version of the library?")
option(USE_CLANG_COMPILER "Use clang as default compiler?")
option(USE_NINJA_AS_BUILD_SYSTEM "Use ninja to write the build scripts?")
option(USE_O3_OPTIMIZATION "Use compiler's level of optimization 3?")

if(${USE_CLANG_COMPILER})
    set(CMAKE_C_COMPILER "clang")
endif()

if(${USE_NINJA_AS_BUILD_SYSTEM})
    set(CMAKE_GENERATOR "Ninja")
endif()

if(${USE_O3_OPTIMIZATION})
    list(APPEND CMAKE_C_FLAGS "-O3")
endif()

include_directories(BEFORE ${CMAKE_SOURCE_DIR}/include/internal)    # For preprocessed files
aux_source_directory(                                               #Getting all source filenames
    ${CMAKE_SOURCE_DIR}/src 
    src_files
)

add_library(cargs ${src_files})