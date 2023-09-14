# ---------------------------------------------------------
# Linux build file that removes unnecessary object binaries
# ---------------------------------------------------------

!/bin/bash

if [ "$1" = clean ]
then
    rm -f build/result/*
fi

ninja -f build/build.ninja
rm -rf build/internal/*.o