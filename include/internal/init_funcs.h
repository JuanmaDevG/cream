#pragma once

#include "utils.h"
#include "shared_data.h"


/*
    Sets the identificator character for argument options. If this function is 
    not called, by default is '-' like in unix-like systems.
*/
void cream_set_identificator(const char new_identificator);

/*
    The first argument is the boolean argument string.
    The second one is the data argument string

    For example the call:
    cream_set_args(4, "abcd", 3, "efgh");

    Will set the command line arguments:
    Boolean (existent or not):      {-a , -b , -c , -d}
    Data (require data to work):    {-e , -f , -g}

    WARNING:
    Data arguments are allowed to be empty in the program input, to allways 
    require data when the program starts is recommended to call 
    cream_set_minimum_data() after this function.

    Each time this function is called, it calls cream_clean() so every data 
    or argument buffer declared previously will be deleted for security 
    and argument consistency reasons.
*/
void cream_set_args(const uint32_t bool_options_count, const char* bool_options, const uint32_t data_options_count, const char* data_options);

/*
    Cleans up all the cream metadata, buffers and whatever data is passed to cream.

    After this function call, cream can be reused with whatever purpose 
    with any kind of arguments.

    Will return false if there is nothing to clean.

    WARNING:
    This function DOES NOT CHANGE the state of:
    - Treating repeated arguments as errors: cream_treat_repeated_args_as_errors
    - Treating anonymous arguments as errors: cream_treat_anonymous_args_as_errors
    - Including the argument zero when call cream_load_args: cream_include_argument_zero
*/
void cream_clean();

/*
    Associates an extended version of an argument letter to make the argument 
    parsing more verbose.

    An example of use:
    cream_associate_extended("acf", "abort", "copy", "file")
    -a = --abort
    -c = --copy
    -f = --file

    WARNING:
    This function is more efficient for the library to receive the arg_characters string ordered as given  
    in cream_set_args function. Otherwise the loop will be much longer.

    In this function call, if any argument char does not exist, cream will simply ignore it.
*/
void cream_associate_extended(const char* arg_characters, ...);

/*
    Makes the characters given as parameter mandatory to use

    This configures cream-error-system to write an error code in cream_error_code while trying to execute 
    cream_load_args function and the specified arguments are not found in the program input while the argument load.

    If a char argument does not exist, cream will just ignore it

    WARNING:
    This function is more efficient for the library to receive the arg_characters string ordered as given  
    in cream_set_args function. Otherwise the loop will be much longer.
*/
void cream_make_mandatory(const char* arg_characters);

/*
    Sets the given argument characters to require a minimum number of arguments.
    For example:
    cream_set_minimum_data("abc", 1, 2, 3);
    -a will require at least one argument
    -b will require at least two arguments
    ...

    WARNING:
    This function is more efficient for the library to receive the arg_characters string ordered as given  
    in cream_set_args function. Otherwise the loop will be much longer.

    The arguments that are not found will just be ignored, no errors will be added to cream_error_code
*/
void cream_set_minimum_data(const char* data_arg_string, ...);

/*
    Sets the given argument characters to allow a maximum number of arguments.
    For example:
    cream_set_maximum_data("abc", 3, 2, 4)
    -a will allow a maximum of three arguments
    -b will alloc a maximum of two arguments
    ...

    WARNING:
    This function is more efficient for the library to receive the arg_characters string ordered as given  
    in cream_set_args function. Otherwise the loop will be much longer.

    The arguments that are not found will just be ignored, no errors will be added to cream_error_code
*/
void cream_set_maximum_data(const char* data_arg_string, ...);

/*
    This function will treat the arguments that are not related to any argument option
    (previously declared with cream_set_args), as errors.

    EXAMPLE:
    If the program my_program has:
    -f argument that allows just 1 data string
    -a argument that allows just 2 data strings

    The call to the program:
    > my_program some-data-here -a 1 2 potatoe -f file.txt foo
    
    Will treat as errors: "some-data-here", "potatoe", and "foo"
*/
void cream_treat_anonymous_args_as_errors(const bool value);

/*
    After the call of this function, the argument option chars given as parameters won't be allowed to be 
    repeated in the program input.

    EXAMPLE:
    #After you call cream_disable_options_repetition("fabc"); 
    #Prohibited to repeat in the program input: {-f, -a, -b , -c}
    > program_name -f file.txt -f another_file.txt (THE ERROR SYSTEM WILL STORE AN ERROR)
*/
void cream_treat_repeated_options_as_errors(const char* arg_options);

/*
    Includes the absolute first argument (like the program name in case of argv)
*/
void cream_include_argument_zero(const bool value);

/*
    Loads the program arguments, checks them and sets the data pointers ready to use.
    After correctly using this function, you can use get functions.

    If any argument error is given, it will stop and store the error code in 
    cream_error_code

    Argument loads are accumulative, so two argument loads will store all the 
    needed information per argument

    WARNING:
    This function must be called when the argument setters are called, otherwise
    will have no effect (but of course you can leave boolean or data arguments empty) 
*/
void cream_load_args(const int argc, const char** argv);

/*
    Cancels all argument loads previously made and so frees all
    argument caching structures and buffers, BUT NOT the 
    previous constraints that the user of this library has set

    This will let the pointers cleaned up to make another 
    cream_load_args like nothing has happened.
*/
void cream_cancel_argument_loads();

/*
    Gets the pointer to the error string generated after the current last argument load.

    If the pointer is NULL, there are no errors, so cream_error_code == CREAM_NO_ERROR
*/
const char* cream_get_errors();