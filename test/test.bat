@echo off
rem **************************************
rem Test script for Windows based machines
rem **************************************

rem (Note:) @echo something (disables the command view for that specific echo)
rem (Note:) do not use space when declaring a variable because print functions can be buggy
rem     Avoid: set my_var = Some value
rem     Prefer: set my_var=Some value
rem (Note:) use %var% calls for variables that were created with set, otherwise use %%var calls with loop local volatile variables


rem This can be changed to gcc if you prefer to use MinGW compiler interface, should not but might produce any error
set compiler=clang

set options=-Wall -Wextra -Werror -O3
set include_dir=..\include\internal\
set source_dir=..\src\

rem There here will be more source files with more updates TODO: make a cleaner syntax
set sources=%source_dir%shared_data.c %source_dir%utils.c

for %%f in (.\src\*) do (
    %compiler% %options% -I%include_dir% %%f -o test.exe
)