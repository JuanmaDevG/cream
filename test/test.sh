# ************************************
# Test script for Linux based machines
# ************************************

#Devnote:
# Allways when comparing strins IS MANDATORY to surround variables in double quotes "$var" and double square brackets [[ check_ops ]] 
# while using conditional sencences and square brackets must be separated from commands by spaces or at least by the same amount 
# of characters
# DON'T EVER FORGET THE DOUBLE QUOTES TO WORK WITH STRINGS, I DID SPEND MANY HOURS IN RESEARCHING
#devnote2:
# Assigniation operation to an actually declared variable must be without dollar sign because if the dollar sign is set, will be 
# interpreted as a command or instruction:
# BAD: $my_var="something" #Will be interpreted as instruction
# GOOD: my_var="something" #Will be interpreted as a variable value set, not as an access

compiler=clang

options="-Wall -Wextra -Werror -Wno-undefined-inline"
include_dir="../include/internal/"
source_dir="../src/"
test_file="tmp_test"

function compile_and_run
{
    $compiler $options -I $include_dir -o $test_file $1 $sources
    ./$test_file
}

function run_test_files
{
    test_package="src/"
    if [ -n "$1" ] #-n means not-empty string
    then
        $test_package="$test_package$1"
    fi

    for file in $test_package*
    do
        if [[ -f "$file" && "${source_file##.*}" != "h" ]]
        then
            compile_and_run $file
        fi
    done

    rm -f $test_file
}


# ---------------------------------------------------
#                      MAIN
# ---------------------------------------------------

# Reading source directories
sources=""
for source_file in $source_dir*
do
    # Actual source file is not a directory
    if [[ -f "$source_file" && "${source_file##.*}" != "h" ]]
    then
        sources="$sources $source_file"
    fi
done

#No gdb debug compile option for the moment...

if [[ "$1" == "clean" ]] #Same space character identation, otherwise won't work
then
    rm -f tmp_test.*
    return
fi

run_test_files "$1"
exit


