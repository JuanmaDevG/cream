#include "utils_test.h"

int main()
{
    init_test_args();
    init_ext_arg_vec();
    init_data_packages();

    _cargs_push_anon_node((const char**)tmp_argv +2, 2);
    _cargs_push_anon_node((const char**)tmp_argv +5, 3);
    _cargs_pop_anon_head_node();
    
    assert(_cargs_anon_arg_count == 3);
    _cargs_pop_anon_head_node();
    assert(_cargs_anon_arg_count == 0);
    assert(_cargs_anon_args == NULL);
    assert(_cargs_anon_last == NULL);

    //Now is completely empty
    _cargs_pop_anon_head_node();
    assert(_cargs_anon_arg_count == 0);
    assert(_cargs_anon_args == NULL);
    assert(_cargs_anon_last == NULL);

    finish(3, "anonymous argument list");
    return 0;
}