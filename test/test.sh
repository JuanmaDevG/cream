!/bin/bash

# ************************************
# Test script for Linux based machines
# ************************************

compiler=clang

options="-Wall -Wextra -Werror -Wno-undefined-inline"
inlcude_dir="../include/internal/"
source_dir="../src/"
test_file="tmp_test"

# Reading source directories
sources=""
for source_file in $source_dir 
do
    # Actual source file is not a directory
    if [ -f "$source_file" ]
    then
        $sources=$sources" $source_dir$source_file"
    fi
done


#This file does not have gdb debug compile option because I don't know how to use it from now

if [ $1 = "clean"]
then
    rm -f tmp_test.*
    return
fi




# Functions

run_test_files()
{
    test_package="src/"
    if [ $1 != ""]
    then
        $test_package=$test_package$1*
    else
        $test_package=$test_package*
    fi

    for file in $test_package
    do
        compile_and_run $file
    done

    rm -f $test_file
}

compile_and_run()
{
    #tomorrow more commands
}