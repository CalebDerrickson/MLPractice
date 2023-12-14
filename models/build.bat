@ECHO off
REM Build script for models
SetLocal EnableDelayedExpansion

REM Get a list of all the .c files
SET cFilenames=
FOR /R %%f in (*.c) do (
    SET cFilenames=!cFilenames! %%f
)

REM echo "Files:" %cFilenames%

SET assembly=models
SET compilerFlags=-g -shared -Wvarargs -Wall -Werror
REM -Wall -Werror
SET includeFlags=-Isrc
SET linkerFlags=-luser32
SET defines=-D_DEBUG -DLEXPORT -D_CRT_SECURE_NO_WARNINGS

ECHO "Building %assembly%%..."
clang %cFilenames% %compilerFlags% -o ../bin/%assembly%.lib %defines% %includeFlags% %linkerFlags%