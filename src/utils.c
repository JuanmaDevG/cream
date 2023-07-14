#include "utils.h"


inline _redundant_queue* _create_redundant_queue()
{
    _redundant_queue* q = (_redundant_queue*)malloc(sizeof(_redundant_queue));
    q->next = NULL; q->position = 0;
    return q;
}

/*
    Reserve the needed space for the new argument string and copies the values ignoring redundant ones
*/
void _transform_redundant_str(
    _redundant_queue* q, char* _reductible_args, uint64_t* _reductible_arg_count, const uint32_t redundancy_count
) {
    char* new_args = (char*)malloc((*_reductible_arg_count) - redundancy_count +1);
    char* copy_point = new_args, *read_point = _reductible_args;
    uint32_t index = 0, offset = 0, diff = 0;
    _redundant_queue* q_pos = q->next;

    while(index < redundancy_count)
    {
        diff = q->position - offset;
        memcpy(copy_point, read_point, diff);
        read_point += diff +1;
        copy_point += diff;

        offset = q->position +1;
        free(q);
        q = q_pos;

        if(index < redundancy_count -1) q_pos = q_pos->next;
        index++;
    }
    q = NULL;
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
    const char* _non_reductible_args, const uint64_t* _non_red_arg_count, 
    char* _reductible_args, uint64_t* _reductible_arg_count
) {
    if(_non_reductible_args == NULL || _reductible_args == NULL) return;
    uint32_t redundancy_count = 0;
    _redundant_queue* q = _create_redundant_queue();
    _redundant_queue* q_pos = q;

    //Obtains the argument redundancies
    for(uint32_t i=0; i < *_non_red_arg_count; i++)
    {
        for(uint32_t j=0; j < *_reductible_arg_count; j++)
            if(_non_reductible_args[i] == _reductible_args[j])
            {
                redundancy_count++;
                q_pos->position = j;
                q_pos->next = _create_redundant_queue();
                q_pos = q->next;
            }
    }
    
    if(redundancy_count > 0) free(q_pos);
    else 
    {
        free(q);
        return;
    }

    _transform_redundant_str(q, _reductible_args, _reductible_arg_count, redundancy_count);
}