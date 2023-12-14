@ECHO OFF
REM Clean everything

ECHO "Cleaning everything..."

REM Testbed
make -f "makefile.testbed.mak" clean
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

REM Models
make -f "makefile.models.mak" clean
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)


ECHO "All assemblies cleaned successfully."