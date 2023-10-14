cmake_minimum_required(VERSION 3.0)
project(cargs)

set( SOURCES
    shared_data.c
    utils.c
    error_str.c
    anon_arg_list.c
    bit_vec_ops.c
    init_funcs.c
    main_funcs.c
)

add_library(cargs_shared SHARED SOURCES)


add_library(cargs_static STATIC SOURCES)