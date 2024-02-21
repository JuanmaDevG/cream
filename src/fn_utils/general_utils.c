#include "general_utils.h"


extern inline bool _cream_streq(const char* _str1, const char* _str2)
{
    if(!(_str1 && _str2)) return false;

    uint32_t i=0;
    while(_str1[i] == _str2[i])
    {
        if(
            _str1[i+1] == '\0' && 
            _str2[i+1] == '\0'
        ) {
            return true;
        }
        i++;
    }
    return false;
}