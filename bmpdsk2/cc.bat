@echo off

if "%1"=="" goto nasi
if "%2"=="" goto ikko
if "%3"=="" goto niko
if "%4"=="" goto sanko
goto endbat

:nasi
    echo.
    echo   [Usage]
    echo.
    echo      cc SourceFile(.c/.cc/.cpp is necessary)
    echo         (Default execution file is a.exe)
    echo.
    echo    or
    echo.
    echo      cc SourceFile(.c/.cc/.cpp is necessary)  ExecutionFile(.exe is Unnecessary)
    echo.
    goto endbat

:ikko
    if exist %1 (
        gcc -Wunused -Wformat -Wuninitialized -Wunused-variable %1 -o a.exe
        goto endbat ) else (
        echo Specified file "%1" cannot be opened.
        goto endbat )
 
:niko
    if exist %1 (
        gcc  -Wunused -Wformat -Wuninitialized -Wunused-variable %1 -o %2
        goto endbat ) else (
        echo Specified file "%1" cannot be opened.
        goto endbat )

:sanko
    echo.
    echo Too many parameters for cc.bat file !
    echo.
:endbat
