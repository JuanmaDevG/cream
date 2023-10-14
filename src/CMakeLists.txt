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

add_library(cargs_static STATIC ${LIB_SRC})
add_library(cargs_dynamic SHARED ${LIB_SRC})