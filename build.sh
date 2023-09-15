# ---------------------------------------------------------
# Linux build file that removes unnecessary object binaries
# ---------------------------------------------------------

if [ "$1" = "clean" ]
then
    rm -f build/result/*
    exit
fi

ninja -f build/build.ninja
rm -rf build/result/*.obj