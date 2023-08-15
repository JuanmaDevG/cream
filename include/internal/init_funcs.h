#pragma once

#include "utils.h"
#include "shared_data.h"


/*
    Sets the identificator character for arguments. If this function is 
    not called, by default is '-' like in unix-like systems.
*/
void cargs_set_identificator(const char new_identificator);

/*
    The first argument is the boolean argument string.
    The second one is the data argument string

    For example the call:
    cargs_set_args("abcd", "efgh");

    Will set the command line arguments:
    Boolean (existent or not):      {-a , -b , -c , -d}
    Data (require data to work):    {-e , -f , -g , -h}

    WARNING:
    Data arguments are allowed to be empty in the program input, to allways 
    require data when the program starts is recommended to call 
    cargs_set_minimum_data() after this function.

    Each time this function is called, it calls cargs_clean() so every data 
    or argument buffer declared previously will be deleted for security 
    and argument consistency reasons.
*/
void cargs_set_args(const char* boolean_arguments, const char* data_arguments);

/*
    Cleans up all the cargs metadata, buffers and whatever is allocated.

    After this function call, cargs can be reused with whatever purpose 
    with any kind of arguments.

    Will return false is there is nothing to clean.
*/
bool cargs_clean();

/*
    Associates an extended version of an argument letter to make the argument 
    parsing more verbose.

    An example of use:
    cargs_associate_extended("acf", "abort", "copy", "file")
    -a = --abort
    -c = --copy
    -f = --file

    WARNING:
    This function is more efficient for the library to receive the arg_characters string ordered as given  
    in cargs_set_args function. Otherwise the loop will be much longer.

    In this function call, if any argument char does not exist, cargs will simply ignore it.
*/
void cargs_associate_extended(const char* arg_characters, ...);

/*
    Makes the characters given as parameter mandatory to use

    This makes cargs to throw an error code while trying to execute 
    cargs_load_args function and the specified arguments are not found

    If a char argument does not exist, cargs will just ignore it

    WARNING:
    This function is more efficient for the library to receive the arg_characters string ordered as given  
    in cargs_set_args function. Otherwise the loop will be much longer.
*/
void cargs_make_mandatory(const char* arg_characters);

/*
    Sets the given argument characters to require a minimum number of arguments.
    For example:
    cargs_set_minimum_data("abc", 1, 2, 3);
    -a will require at least one argument
    -b will require at least two arguments
    ...

    WARNING:
    This function is more efficient for the library to receive the arg_characters string ordered as given  
    in cargs_set_args function. Otherwise the loop will be much longer.

    The arguments that are not found will just be ignored, no errors will be added to cargs_error_code
*/
void cargs_set_minimum_data(const char* data_arg_string, ...);

/*
    Sets the given argument characters to allow a maximum number of arguments.
    For example:
    cargs_set_maximum_data("abc", 3, 2, 4)
    -a will allow a maximum of three arguments
    -b will alloc a maximum of two arguments
    ...

    WARNING:
    This function is more efficient for the library to receive the arg_characters string ordered as given  
    in cargs_set_args function. Otherwise the loop will be much longer.

    The arguments that are not found will just be ignored, no errors will be added to cargs_error_code
*/
void cargs_set_maximum_data(const char* data_arg_string, ...);

/*
    This function will treat the arguments that are not related to any data argument, as errors.

    EXAMPLE:
    If the program my_program has:
    -f argument that allows just 1 data string
    -a argument that allows just 2 data strings

    The call to the program:
    > my_program some-data-here -a 1 2 potatoe -f file.txt foo
    
    Will treat as errors: "some-data-here", "potatoe", and "foo"
*/
extern inline void cargs_treat_anonymous_args_as_errors(const bool value);

/*
    After the call of this function to true, the repetition of any argument option 
    in the program input will be treated as an error and loaded to cargs_error_code.

    EXAMPLE:
    > program_name -f file.txt -f another_file.txt (WILL STORE ERROR)
*/
extern inline void cargs_treat_repeated_args_as_errors(const bool value);

/*
    Loads the program arguments, checks them and sets the data pointers ready to use.
    After correctly using this function, you can use get functions.

    If any argument error is given, it will stop and store the error code in 
    cargs_error_code

    WARNING:
    This function must be called when the argument setters are called, otherwise
    will have no effect (but of course you can leave boolean or data arguments empty) 
*/
void cargs_load_args(const int argc, const char** argv);

/*
    Gets the pointer to a a loaded error string

    If the pointer is NULL, there are no errors
*/
const char* cargs_get_error();
