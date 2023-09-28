# cargs

The best **library** that **solves the command-line argument managing time waste** in your programming projects and works perfectly with C/C++ and 
any programming language that can load DLLs (Dynamic Link Libraries)

## What is cargs?

cargs is essentially an entire library that offers a very lightweigh runtime that can be used for any argument parsing purpose.
It provides a set of functions that let you manage every little detail about the argument parsing in your program.

It is **NOT JUST command line arguments**, it supports multiple argument loads, integrated _argument rule creation_, has a little **error system** 
that will be updated with time, and integrated automatic self memory management default set of functions (no need to free or manage pointer ownership).

It is a reasonably "big" project in terms of functionality to explain right into this file, and I wanted to take it as an user manual, so for more details, 
you can check out the code (I'll make an implementation explanation if I have time).

## How do I get cargs to work?

cargs is intended to have the easiest syntax as possible for the user (including begginers) so for experienced programmers some steps will seem obvious 
for a project with this lack of user-side complexity, I decided to make it available for everyone because when I was just newbie, I would wanted to have 
some GitHub project that I can easily access to...

### The easy way, download binaries

I will post the releases link here, just let me some time

### Wanna build yourself? Here the tools that you need

- **clang** compiler: https://github.com/llvm/llvm-project/releases
- **Ninja** build system: https://github.com/ninja-build/ninja/releases

> In linux distributions you can simply do this (yes, is that simple):
> $ sudo apt install clang llvm ninja

**DO NOT EXACTLY COPY THE FOLLOWING BUILD INSTRUCTIONS, you'll understand why while reading...**

First (Windows cmd or Linux bash is valid) clone the repo:
'''
    git clone https://github.com/JuanmaDevG/cargs
    cd cargs\
'''

Windows build instructions:
'''
    build
    cd build\result\
    move cargs.lib \wherever\you\wanna\place\the\library\
    move cargs.dll \wherever\you\wanna\place\the\library\
    cd ..\\..\include\
    move cargs.h \wherever\you\wanna\place\the\header_file/
'''

Linux build instructions:
'''
    ./build.sh
    cd build/result/
    mv libcargs.a /wherever/you/want/to/place/the/library/
    mv libcargs.so /wherever/you/want/to/place/the/library/
    cd ../../include
    mv cargs.h /wherever/you/wanna/place/the/header_file/
'''