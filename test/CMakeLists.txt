#TODO: make the functions to set the tests.

function(get_test_source_files SRC_DIR_LIST _test_files)
    foreach(src_dir SRC_DIR_LIST)
        aux_source_directory(src_dir COLLECTED_TEST_FILES)
        list(APPEND _test_files COLLECTED_TEST_FILES)
    endforeach()
endfunction()