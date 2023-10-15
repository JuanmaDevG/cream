#I barely hate cmake

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


#TODO: name both libraries as cargs
add_library(cargs_static STATIC ${LIB_SRC})
add_library(cargs_dynamic SHARED ${LIB_SRC})