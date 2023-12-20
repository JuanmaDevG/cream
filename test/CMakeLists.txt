# Test configuration file:
# 
# This file takes the test directories and configures the test executables inside each dir. 
# Test executables are grouped with different label groups to avoid having to pass all the 
# tests each time ctest is run.

function(get_index_list actual_list _idx_list)
    list(LENGTH actual_list list_length)
    math(EXPR list_length "${${list_length}} - 1")

    foreach(idx RANGE list_length)
        list(APPEND _idx_list idx)
    endforeach()

    set(_idx_list ${_idx_list} PARENT_SCOPE)
endfunction()


#Start

set( CARGS_TEST_SOURCE_DIR ${CMAKE_SOURCE_DIR}/test)

set( CARGS_TEST_DIRECTORIES 
    ${CARGS_TEST_SOURCE_DIR}/exp_stack 
    ${CARGS_TEST_SOURCE_DIR}/utils 
    ${CARGS_TEST_SOURCE_DIR}/init_funcs 
    ${CARGS_TEST_SOURCE_DIR}/main_funcs
)

set( CARGS_TEST_GROUPS 
    "expandable stack" "utilities" "init functions" "main functions"
)

get_index_list(CARGS_TEST_DIRECTORIES _index_list)

foreach(_idx _index_list)
    # Get source dir and group
    list(GET CARGS_TEST_DIRECTORIES ${_idx} _src_dir)
    list(GET CARGS_TEST_GROUPS ${_idx} _test_group)
    aux_source_directory(${_src_dir} _sources)

    foreach(_src_file _sources)
        #TODO: manipulate source file to manage executable names
        add_executable(executable_name_TODO ${_src_file})
        target_link_libraries(executable_name cargs)
        add_test(NAME test_name COMMAND ${executable_name})
    endforeach()
endforeach()