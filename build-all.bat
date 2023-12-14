@ECHO OFF
REM Building Everything

@REM ECHO "Building Everything..."


REM Models
make -f "makefile.models.mak" all
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)


REM Testbed
make -f "makefile.testbed.mak" all
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

ECHO "All assemblies built successfully."