# cargs

The best **library** that **solves the command-line argument managing time waste** in your programming projects and works perfectly with C/C++ and 
any programming language that can load DLLs (Dynamic Link Libraries)

## What is cargs?

cargs is essentially an entire library that offers a very lightweigh runtime that can be used for any argument parsing purpose.
It provides a set of functions that let you manage every little detail about the argument parsing in your program **THE EASIEST WAY POSSIBLE**.

It is **NOT JUST command line arguments**, it supports multiple argument loads, integrated _argument rule creation_, has a little **error system** 
that will be updated with time, and integrated automatic self memory management default set of functions (no need to free or manage pointer ownership).

It is a reasonably "big" project in terms of functionality to explain right into this file, and I wanted to take it as an user manual, so for more details, 
you can check out the code (I'll make a proper extended documentation if I have time).

## How do I get cargs to work?

cargs is intended to have the easiest syntax as possible for the user (including begginers) so for experienced programmers some steps will seem obvious 
for a project with this lack of user-side complexity, I decided to make it available for everyone because when I was just a newbie, I would wanted to have 
some GitHub project that I can easily access to. It took some months to understand about linking, libraries, build systems, package managers (and the lack 
of them) for dependencies, naming conventions, and so on...

### The easy way, download binaries

This will be a link to the release page... just let me some time to write the user manual...

### Wanna build yourself? Here the tools that you need

- **clang** compiler: https://github.com/llvm/llvm-project/releases
- **Ninja** build system: https://github.com/ninja-build/ninja/releases

> In linux distributions you can simply do this (yes, is that simple):
> $ sudo apt install clang llvm ninja

**DO NOT EXACTLY COPY THE FOLLOWING BUILD INSTRUCTIONS, you'll understand why while reading...**

First (Windows cmd or Linux bash is valid) clone the repo:
```bash
    git clone https://github.com/JuanmaDevG/cargs
    cd cargs
```

Windows build instructions:
```cmd
    build
    cd build\result\
    move cargs.lib \wherever\you\wanna\place\the\library\
    move cargs.dll \wherever\you\wanna\place\the\library\
    cd ..\..\include\
    move cargs.h \wherever\you\wanna\place\the\header_file/
```

Linux build instructions:
```bash
    ./build.sh
    cd build/result/
    mv libcargs.a /wherever/you/want/to/place/the/library/
    mv libcargs.so /wherever/you/want/to/place/the/library/
    cd ../../include
    mv cargs.h /wherever/you/wanna/place/the/header_file/
```

## How to LINK cargs to your programs? (for newbies like I was)

Once you've obtained the library binaries or you've built the library yourself like a true sigma. 
Have you ever used an external library? If not, you're lucky because this section is written as an introduction to the usage of external libraries. To be the simplest 
as possible, an external library is a piece of code that contains functions to call, or classes to instantiate (c++) and use in your program's code.

If you're using the Dynamic Link Library (.dll for Windows) or Shared Object (.so for Linux), you would tipically need to use the runtime linker, BUT you're lucky to 
have the header file ``cargs.h`` that will make the linkage much more simple. In the case you're using the Static Library (.lib for Windows / .a for Windows) the 
header file is MANDATORY.

You have some **OPTIONS** to locate the header file (cargs.h) and the cargs.lib/libcargs.a (or for dynamic linking cargs.dll/libcargs.so) file:

- **Integrating the library** into your project directories (less recommended because your project will depend on a binary file and that's generally a bad practice).
- **Creating a general directory** to store your libraries (a bit more reccommended, you would have your own directories for your projects where to find the libraries).
- **Write command line files** that automatically build external repositories or import binaries from the official download sites.
- **Make your own installer** with a tool that lets you package your project and automatically manage the installation of dependencies.

> For simplicity and to be able to use the library today, I would recommend the first or second option XD

Now that you've placed the header file and the library file (must be one, DO NOT place Dynamic Link Library and Static Library into the same dir please), you must know 
that the compiler does not know WHERE are your libraries, so now that we've barely scratched the surface of dependency management for linkage, let's learn with an example.

You have a project with a source file called ``my_app.c``, a project dir called ``dependencies/``, in there you have the cargs header file ``include/cargs.h`` and the 
library file ``lib/cargs.lib`` (or libcargs.a), each with it's own directory. Your project's source file contains the ``#include <cargs.h>`` directive and uses some 
functions.

To finally get your executable to life, the compile command would be:

> Pro tip, use backslash on Windows for directories AND is also a Windows convention to name the output files as whatever.exe

```bash
    clang -I dependencies/include/ -L dependencies/lib/ -lcargs -o my_app  my_app.c
```

It is intuitive that the -I option is for the preprocessor to match the #include directive and the -L option is to tell the c/c++ linker WHERE are those precious binary 
files known as libraries, because the compiler only knows where the standard library dirs are, but not your's.

The linker is not going to link found file into the library directory, YOU MUST SELECT each library to be linked, in this case, just cargs with ``-lcargs``. Then 
declare the ouput file to generate with ``-o`` option and introduce the source code files.

**And you're done!! Now you can use any external library from GitHub!!**

## The official cargs user manual

Actually writing... but the cargs.h header file is really self descriptive...