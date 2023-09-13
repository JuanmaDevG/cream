echo off
setlocal
rem -----------------------------------------------------------
rem Windows build file that removes unnecessary object binaries
rem -----------------------------------------------------------


if -%1-==-clean- (
    del /q build\result\*
    del .ninja_log
    exit /b
)

ninja -f build\build-win.ninja
del build\result\*.obj

endlocal
echo on