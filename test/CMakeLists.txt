# Test configuration file:
# 
# This file takes the test directories and configures the test executables inside each dir. 
# Test executables are grouped with different label groups to avoid having to pass all the 
# tests each time ctest is run.

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

set(label_id 0)

foreach(test_dir CARGS_TEST_DIRECTORIES)
    aux_source_directory(test_dir test_source_files)
    foreach(test_source test_source_files)
        add_executable(test_executable test_source)
        target_link_libraries(test_executable cargs)
        add_test(NAME ${test_executable} COMMAND ${test_executable})
        # TODO: Manage list indices or anidate lists into lists
    endforeach()
endforeach()