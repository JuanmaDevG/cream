#I hate cmake because of making multiple files but I need to learn how to use it

set(LIB_SRC
    shared_data.c
    utils.c
    anon_arg_list.c 
    bit_vec_ops.c
    error_str.c
    init_funcs.c
    main_funcs.c
)

add_compile_options(-Wall -Wextra -Werror -Wno-undefined-inline -O3)

add_library(cargs_static STATIC ${LIB_SRC})
target_include_directories(cargs_static PUBLIC ${PROJECT_SOURCE_DIR}/include/internal)

add_library(cargs_dynamic SHARED ${LIB_SRC})