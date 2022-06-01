@echo off
rem Compilation...

set PATH=..\_tools;..\_gcc\bin;%PATH%

if exist program.uf2 del program.uf2
make all -j 8
if errorlevel 1 goto err
if not exist program.uf2 goto err
echo.
type program.siz
goto end

:err
pause ERROR!
:end
