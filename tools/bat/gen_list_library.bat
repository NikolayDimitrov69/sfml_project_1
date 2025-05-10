@echo off
echo Running enum generator...

set "path=../../sfml-project2/GeneratedEnums.h"
del "%path%" 2>nul

C:\Windows\py.exe ../Scripts/gen_list_library.py

if exist "%path%" (
    echo Successfully generated GeneratedEnums.h
) else (
    echo Generation failed.
)