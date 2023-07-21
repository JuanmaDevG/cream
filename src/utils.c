#include "utils.h"


inline _cargs_queue* _create_cargs_queue()
{
    _cargs_queue* q = (_cargs_queue*)calloc(1, sizeof(_cargs_queue));
    q->next = NULL;
    return q;
}

inline void _push_node(_cargs_queue* last_q_pos, const uint32_t elem)
{
    last_q_pos->next = _create_cargs_queue();
    last_q_pos = last_q_pos->next;
    last_q_pos->value = elem;
}

inline void _pop_node(_cargs_queue* head)
{
    _cargs_queue* keeper = head->next;
    free(head);
    head = keeper;
}

inline uint8_t _q_is_empty(_cargs_queue* q) { return (q == NULL ? 1 : 0); }

/*
    Reserve the needed space for the new argument string and copies the values ignoring redundant ones
*/
void _transform_redundant_str(
    _cargs_queue* q, char* _reductible_args, uint64_t* _reductible_arg_count, const uint32_t redundancy_count
) {
    char* new_args = (char*)malloc((*_reductible_arg_count) - redundancy_count +1);
    char* copy_point = new_args, *read_point = _reductible_args;
    uint32_t offset = 0, diff = 0;

    while(_q_is_empty(q) != (uint8_t)1)
    {
        diff = q->value - offset;
        memcpy(copy_point, read_point, diff);
        read_point += diff +1;
        copy_point += diff;

        offset = q->value +1;
        _pop_node(q);
    }
    memcpy(copy_point, read_point, (*_reductible_arg_count) - offset);

    free(_reductible_args);
    _reductible_args = new_args;
    *_reductible_arg_count -= redundancy_count;
}


/*
    Removes the redundancies found from _non_reductible_args and applies the changes to _reductible_args
    If any of the arguments is uninitialized (NULL) will do nothing
*/
void remove_redundancies(
    const char* _non_reductible_args, const uint64_t* _non_reductible_arg_count, 
    char* _reductible_args, uint64_t* _reductible_arg_count
) {
    if(_non_reductible_args == NULL || _reductible_args == NULL) return;
    uint32_t redundancy_count = 0;
    _cargs_queue* q = NULL;
    _cargs_queue* q_pos = NULL;

    //Get first redundance
    uint32_t i = 0, j;
    while(i < *_non_reductible_arg_count && redundancy_count == 0)
    {
        j = 0;
        while(j < *_reductible_arg_count)
        {
            if(_non_reductible_args[i] == _reductible_args[j])
            {
                redundancy_count++;
                _push_node(q, j);
                q_pos = q;
                break;
            }
            j++;
        }
        i++;
    }
    if(redundancy_count == 0) return;

    //Obtains the argument redundancies
    while(i < *_non_reductible_arg_count)
    {
        j=0;
        while(j < *_reductible_arg_count)
        {
            if(_non_reductible_args[i] == _reductible_args[j])
            {
                redundancy_count++;
                _push_node(q_pos, j);
            }
            j++;
        }
        i++;
    }

    _transform_redundant_str(q, _reductible_args, _reductible_arg_count, redundancy_count);
}