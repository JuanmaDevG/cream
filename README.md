# cream

The cream library stands for **C Readable and Expandable Argument Manager**, as you see, it is self descriptive.

Cream aims to **solve the command-line argument parsing time waste problem** in your programming projects and works perfectly with C/C++ and 
any programming language that can load DLLs (Dynamic Link Libraries)


## What is cream?

cream is essentially an entire library that offers a very lightweigh runtime that can be used for any argument parsing purpose. 
Yes, it is sintactically the best command line argument parser for the C and C++ languages.
It provides a set of functions that let you manage every little detail about the argument parsing in your program **THE EASIEST WAY POSSIBLE** 
and with no effort.

The motivation to create this library comes from the need of a library that makes all the work of parsing the command line arguments, and to finally 
develop command line programs where the input arguments are completely organized and the syntax is not a cumbersome 
(like the whole rest of the libraries).

It is **NOT JUST command line arguments**, it supports multiple argument loads, integrated _argument rule creation_, has a little **error system** 
that will be updated with time, and integrated automatic self memory management default set of functions (no need to free or manage pointer ownership).

It is a reasonably "big" project in terms of functionality to explain right into this file, and I wanted to take it as an user manual, so for more details, 
you can check out the code (I'll make a proper extended documentation if I have time).

## How do I get cream to work?

cream is intended to have the easiest syntax as possible for the user (including begginers) so for experienced programmers some steps will seem obvious 
for a project with this lack of user-side complexity, but I decided to make it available for everyone because when I was just a newbie, I would wanted to have 
some GitHub project that I can easily access to. It took some months to understand about linking, libraries, build systems, package managers (and the lack 
of them by default like windows...) for dependencies, naming conventions, and so on...

### The easy way, download binaries

Literally https://github.com/JuanmaDevG/cream/releases and download the x86_64 linux or windows version

### Wanna build yourself?

CMake is strictly necessary to build the project. **DONT FORGET** to check the option *add cmake to the PATH* 
if you want to use cmake for windows with the command line.

 - **CMake** build system: https://cmake.org/download/

Tools I **highly** recommend to install for windows compilation (and so If you do not want to use Visual Studio)

- **clang** compiler: https://github.com/llvm/llvm-project/releases
- **Ninja** build system: https://github.com/ninja-build/ninja/releases
- **Windows SDK**: https://developer.microsoft.com/es-es/windows/downloads/windows-sdk/

> In linux distributions you can simply do this (yes, is that simple):
> $ sudo apt install cmake llvm clang ninja

For building yourself the library, it is recommended to have some knowledge of cmake to configure the build to your taste, but in case you just want to try and 
build it yourself without more configurations, you'll minimally need a **Linux distribution + any c compiler + any build system(Ninja, GNU make, ...)** or the 
**Windows SDK + Ninja + clang** or the default c++ **Visual Studio installation** to let cmake generate the build files.

The commands to build it like I would are:

```bash
    cmake -D CMAKE_C_COMPILER=clang -G Ninja build
    cd build
    ninja #Or: make
```

Or the default cmake build command to set everything default:

```bash
    cmake your/whatever/build/dir
    cmake --build your/whatever/build/directory
```

## How to LINK cream to your programs? (for newbies like I was)

Once you've obtained the library binaries or you've built the library yourself like a true sigma. 
Have you ever used an external library? If not, you're lucky because this section is written as an introduction to the usage of external libraries. To be the simplest 
as possible, an external library is a piece of code that contains functions to call, or classes to instantiate (c++) and use in your program's code.

If you're using the Dynamic Link Library (.dll for Windows) or Shared Object (.so for Linux), you would tipically need to use the runtime linker, BUT you're lucky to 
have the header file ``cream.h`` that will make the linkage much more simple. In the case you're using the Static Library (.lib for Windows / .a for Linux) the 
header file is MANDATORY.

You have some **OPTIONS** to locate the header file (cream.h) and the cream.lib/libcream.a (or for dynamic linking cream.dll/libcream.so) file:

- **Integrating the library** into your project directories (less recommended because your project will depend on a binary file and that's generally a bad practice).
- **Creating a general directory** to store your libraries (a bit more reccommended, you would have your own directories for your projects where to find the libraries).
- **Write command line files** that automatically build external repositories or import binaries from the official download sites.
- **Make your own installer** with a tool that lets you package your project and automatically manage the installation of dependencies.

> For simplicity and to be able to use the library today, I would recommend the first or second option XD

Now that you've placed the header file and the library file (must be one, DO NOT place Dynamic Link Library and Static Library into the same dir please), you must know 
that the compiler does not know WHERE are your libraries, so now that we've barely scratched the surface of dependency management for linkage, let's learn with an example.

You have a project with a source file called ``my_app.c``, a project dir called ``dependencies/``, in there you have the cream header file ``include/cream.h`` and the 
library file ``lib/cream.lib`` (or libcream.a), each with it's own directory. Your project's source file contains the ``#include <cream.h>`` directive and uses some 
functions.

To finally get your executable to life, the compile command would be:

> Pro tip, use backslash on Windows for directories AND is also a Windows convention to name the output files as whatever.exe

```bash
    clang -I dependencies/include/ -L dependencies/lib/ -lcream -o my_app  my_app.c
```

It is intuitive that the -I option is for the preprocessor to match the #include directive and the -L option is to tell the c/c++ linker WHERE are those precious binary 
files known as libraries, because the compiler only knows where the standard library dirs are, but not your's.

The linker is not going to link found file into the library directory, YOU MUST SELECT each library to be linked, in this case, just cream with ``-lcream``. Then 
declare the ouput file to generate with ``-o`` option and introduce the source code files.

**And you're done!! Now you can use any external library from GitHub!!**

## The official cream user manual

Let's start by learning about two concepts, command line argument options can be **boolean arguments** (existent or not in the argument input) or **data arguments** 
(those that can have additional parameters to work). For example, the program:

``$ clang -c -o program.o source.c``

As you can see, an argument option is allways preceded by an unique identifier in Unix like programs, the option identifier is '-' and it has become a standard.

Focusing on the example, the compiler gets a boolean argument option (-c) that means just to compile without performing the linking phase, and a 
data argument option (-o) wich **needs data**, that data is the output filename, and last but not least, the source code filename. 

What happens to the source filename? It is not linked to any argument option (neither -o), that's what cream calls **anonymous argument**, it's simply an argument that 
is just data itself, interesting data for the program.

This library is capable to clone the same behaviour with a few functions:

```c
    #include <stdio.h>
    #include <cream.h>

    /*
        The enumerated types are just numbers that determine the position of each argument character
        For efficiency, the data needs to be accessed at by the argument character index.
    */
    enum BOOL_ARGS {ARG_C}
    enum DATA_ARGS {ARG_O, ARG_L_UPPER, ARG_I_UPPER}

    int main(int argc, char** argv)
    {
        //You will almost never use this function because the default argument identifier set is '-'
        cream_set_identificator('-');

        // Boolean arguments: { -c }    Data arguments: { -o , -L , -I }
        cream_set_args("c", "oLI");

        /*
            As you can appreciate, the argument options from clang compiler can handle minimum 1 piece of data and also maximum one, 
            you cannot leave -o option empty.

            cream is desiged to support up to 255 maximum and minimum options, and assigning zero means no limits (this comes by default)
        */
        cream_set_minimum_data("oLI", 1, 1, 1); //Each argument option character must have a minimum of one piece of data (like a filename)
        cream_set_maximum_data("oLI", 1, 1, 1); //And so, each argument option character can handle a maximum of one pice of data

        //Yes, you can treat anonymous arguments as errors
        cream_treat_anonymous_args_as_errors(false); //By default this is false but is better you know this function exists.

        //Because in clang you can do it like: clang -L some/dir/ -L some/another/dir/ -I some/include/dir -I some/other/ ...
        //cream by default allows repeated argument options, but you will know whenever you want to configure this to true
        cream_treat_repeated_args_as_errors(false);

        //This is also false by default but yeah, you can take the absolute first argument with the argument load (in this case the program name is useless)
        //The real life utility case of this function will be explained later
        cream_include_argument_zero(false);

        //After previous cream configuration, cream-state-machine is ready to load the command line arguments
        cream_load_args(argc, argv);

        //Was there any error? Let's check out the cream error system
        if(cream_error_code != cream_NO_ERROR)
        {
            printf("%s\n", cream_get_error());
            cream_clean();  //Frees all the memory needed by the library, avoiding memory leaks
            return 0;
        }

        //And finally, after the argument load that follows the previously set configuration, we can obtain our data
        if(cream_check_bool(ARG_C)) printf("The argument option c has been set");
        
        if(cream_check_data(ARG_O))
        {
            unsigned int data_count = cream_get_data_count(ARG_O);
            char** actual_data = cream_get_data(ARG_O);

            for(unsigned int i=0; i < data_count; i++)
            {
                printf("Data piece number %u is %s\n", i, actual_data[i]);
            }
        }

        //Don't forget that you can get the anonymous arguments
        unsigned int anon_count = cream_get_anonymous_arg_count();
        char** anonymous_args = cream_get_anonymous_args();

        //And so, iterate and do whatever you want with them

        return 0;
    }
```

This has been a short and not really complete example about what cream is able to do, but you may get the idea.

## The cream complete functions user manual 

### cream_set_identificator(const char new_identificator)

Sets the identificator character for argument options. If this function is not called, by default is '-' like in unix-like systems. For example:

```c
    //Will allow argument options like: /a /b /c /f /h ...
    cream_set_identificator('/');
```


### cream_set_args(const char* boolean_arguments, const char* data_arguments)

The first argument is the boolean argument string.
The second one is the data argument string.

For example the call:
```c
    cream_set_args("abcd", "efgh");
```

Will set the command line arguments:
- **Boolean options** (existent or not):      {-a , -b , -c , -d}
- **Data options** (require data to work):    {-e , -f , -g , -h}

**WARNING:**

Data arguments are allowed to be empty in the program input, to allways 
require data when the program starts is recommended to call 
cream_set_minimum_data() after this function.

Each time this function is called, it calls cream_clean() so every data 
or argument buffer declared previously will be deleted for security 
and argument consistency reasons.


### cream_associate_extended(const char* arg_characters, ...)

Associates an extended version of an argument letter to make the argument 
parsing more verbose.

An example of use:
```c
    cream_associate_extended("acf", "abort", "copy", "file")
```
- -a = --abort
- -c = --copy
- -f = --file

**WARNING:**

This function is more efficient for the library to receive the arg_characters string ordered as given  
in cream_set_args function. Otherwise the loop will be much longer.

In this function call, if any argument char does not exist, cream will simply ignore it.


### cream_make_mandatory(const char* arg_characters)

Makes the characters given as parameter mandatory to use

This configures cream-error-system to write an error code in cream_error_code while trying to execute 
cream_load_args function and the specified arguments are not found in the program input while the argument load.

```c
    cream_make_mandatory("abc"); // Mandatory: { -a, -b, -c }
```

If a char argument does not exist, cream will just ignore it

**WARNING:**

This function is more efficient for the library to receive the arg_characters string ordered as given  
in cream_set_args function. Otherwise the loop will be much longer.


###  cream_set_minimum_data(const char* data_arg_string, ...)


Sets the given argument characters to require a minimum number of arguments.
For example:
```c
    cream_set_minimum_data("abc", 1, 2, 3);
```
- -a will require at least one argument
- -b will require at least two arguments
- and so on...

**WARNING:**

This function is more efficient for the library to receive the arg_characters string ordered as given  
in cream_set_args function. Otherwise the loop will be much longer.

The arguments that are not found will just be ignored, no errors will be added to cream_error_code


### cream_set_maximum_data(const char* data_arg_string, ...)

Sets the given argument characters to allow a maximum number of arguments.
For example:
```c
    cream_set_maximum_data("abc", 3, 2, 4)
```
- -a will allow a maximum of three arguments
- -b will alloc a maximum of two arguments
- And so on...

**WARNING:**

This function is more efficient for the library to receive the arg_characters string ordered as given  
in cream_set_args function. Otherwise the loop will be much longer.

The arguments that are not found will just be ignored, no errors will be added to cream_error_code


### cream_treat_anonymous_args_as_errors(const bool value)

This function will treat the arguments that are not linked to any argument option (previously declared with cream_set_args), 
as errors.

By defautl, this value is set to false.

EXAMPLE:
If the program my_program has:
- -f argument that allows just 1 data piece string
- -a argument that allows just 2 data piece strings

The call to the program:
```bash
    my_program some-data-here -a 1 2 potatoe -f file.txt foo
```

Will treat as errors: "some-data-here", "potatoe", and "foo"


### cream_treat_repeated_args_as_errors(const bool value)

After the call of this function to true, the repetition of any argument option 
in the program input will be treated as an error and loaded to cream_error_code.

by default this value is set to false.

EXAMPLE:
```bash
    program_name -f file.txt -f another_file.txt
```
Will write an error in the _cream-error-system_ and stop the current argument load


### cream_include_argument_zero(const bool value)

Includes the absolute first argument (like the program name in case of argv).

By default, this value is set to false.

This function is oriented to be used when there is more than one argument load. It is possible to **change the state machine** between different 
argument loads to accumulate argument data.


### cream_load_args(const int argc, const char** argv)

Loads the program arguments, checks them and sets the data pointers ready to use.
After correctly using this function, you can use gettter functions to obtain your data.

If any argument error is given, it will stop and store the error code in 
*cream_error_code*, to extract the error string, you can do it so by *cream_get_error()* 

Argument loads are accumulative, so two argument loads will store all the 
needed information per argument. cream is so flexible that allows you to change the *cream-state-machine* 
between different argument loads.

**WARNING:**

This function must be called when the argument setters are called and so cream is configured, otherwise
will have no effect (but of course you can leave boolean or data arguments empty, just don't leave both empty)


### cream_cancel_argument_loads()

Cancels all argument loads previously made and so frees all
argument caching structures and buffers, **BUT NOT** the 
previous constraints that the user of this library has set (with configuration functions).

This will let the cream internal argument caching structures cleaned up to make another 
cream_load_args like nothing has happened.


### cream_clean()

Cleans up all the cream metadata, buffers and whatever data is passed to cream.

After this function call, cream can be reused with whatever purpose 
with any kind of arguments.

Will return false if there is nothing to clean.

**WARNING**, this function **DOES NOT CHANGE** the state of:
- Treating repeated arguments as errors: cream_treat_repeated_args_as_errors
- Treating anonymous arguments as errors: cream_treat_anonymous_args_as_errors
- Including the argument zero when call cream_load_args: cream_include_argument_zero


### cream_get_error()

Gets the pointer to the error string generated after the current last argument load.

If the pointer is NULL, there are no errors, so cream_error_code == cream_NO_ERROR


### cream_check_bool(const uint32_t __arg_index)

Returns a boolean value checking the existence of the bool argument option in the argument input 
using it's boolean argument index.

If you set:
```c
    cream_set_args("abc", NULL); //And then cream_load_args(......);
    cream_load_args(argc, argv);
```
*Don't forget to load the args*

The existence of 'b' option is checked with:
```c
    // 0 -> -a     1 -> -b     2 -> -c
    cream_check_bool_arg(1);
```


### cream_check_data(const uint32_t __arg_index)

Returns a boolean value checking the existence of the data argument option in the argument input 
using it's data argument index.

If you set:
```c
    cream_set_args(NULL, "abc"); //And then cream_load_args(...whatever);
    cream_load_args(argc, argv);
```

The existence of 'c' option is checked with:
```c
    cream_check_data_arg(2); //0 -> -a       1 -> -b      2 -> -c
```


### cream_get_data_count(const uint32_t arg_index)

Returns the number of elements associated to the given data argument 
index


### cream_get_data(const uint32_t arg_index)

Returns a char pointer vector with all the strings that the 
argument option has cached after the argument load.

**WARNING:**

If there has been no associated data to an option in the argument 
load (the_returned_ptr == NULL) does not mean that the option has not 
been checked, the only ways to check options existence are the 
cream_check_... functions


### cream_get_anonymous_arg_count()

Returns the number of anonymous arguments cached by cream after the 
last argument load (or loads).


### cream_get_anonymous_args()

Returns a dual char pointer that contains all the pointed 
argument strings cached by cream after the last argument loads.