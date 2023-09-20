@ECHO OFF
REM Building Everything

@REM ECHO "Building Everything..."

REM Testbed
make -f "makefile" all
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

ECHO "All assemblies built successfully."