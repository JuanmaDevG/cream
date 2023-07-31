@echo off
rem **************************************
rem Test script for Windows based machines
rem **************************************

rem Development notes (what I've learned and I some advice)
rem ------------------------------------------------------------------------------------------------------------------------------

rem (Note:) @echo something (disables the command view for that specific echo)
rem (Note:) do not use space when declaring a variable because print functions can be buggy
rem     Avoid: set my_var = Some value
rem     Prefer: set my_var=Some value
rem (Note:) use %var% calls for variables that were created with set, otherwise use %%var calls with loop local volatile variables
rem (Note:) is very important to respect the compiler arguments order
rem     LIKE: compiler_call [config_options] [source_files] [header_files_dirs and lib_dirs] [lib_activation_options (-lglfw)]
rem (Note:) VERY IMPORTANT
rem     inline functions must be marked as extern whenever they're not implemented on the header file, this is 
rem     to tell the linker that the function is implemented in another file.
rem     This thing is still not very useful when -Werror flag is declared, because the compiler will bug and 
rem     print an undefined inline warning, to avoid the warning you simply tell the compiler not to show it
rem     (-Wno-undefined-inline) and do not worry because the inline functions will be pasted in your code.
rem     This happens because the compiler does not know the implementation of an inline function and is very common to put 
rem     inline functions implementation within their definition header file.

rem ------------------------------------------------------------------------------------------------------------------------------

rem This can be changed to gcc if you prefer to use MinGW compiler interface, should not but might produce any error
set compiler=clang

set options=-Wall -Wextra -Werror -Wno-undefined-inline -O3
set include_dir=..\include\internal\
set source_dir=..\src\
set test_sources=.\src\*

rem There here will be more source files with more updates TODO: make a cleaner syntax
set sources=%source_dir%error_str.c %source_dir%shared_data.c %source_dir%utils.c
set test_file=tmp_test.exe

rem Compile object files
for %%f in (%test_sources%) do (
    rem Avoiding the compilation of header files because of lack of entry point
    if /I not %%~xf == .h (
        %compiler% %options% -o %test_file% -I%include_dir% %%f %sources%
        %test_file%
    )
)
del %test_file%
