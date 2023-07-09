#include "utils.h"


inline _redundant_queue* _create_redundant_queue()
{
    _redundant_queue* q = (_redundant_queue*)malloc(sizeof(_redundant_queue));
    q->next = NULL; q->position = 0;
    return q;
}

void remove_redundancies(
    char* _non_reductible_args, uint64_t* _non_red_arg_count, 
    char* _reductible_args, uint64_t* _reductible_arg_count
) {
    //Ensuring both values
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

    //Reserves the needed space for the new argument string and copies the values
    char* new_args = (char*)malloc((*_reductible_arg_count) - redundancy_count +1);
    //...
}