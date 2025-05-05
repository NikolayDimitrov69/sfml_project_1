@echo off
echo Running enum generator...

REM Make sure python is on PATH and ListLibrary.xml is in same folder
python ../Scripts/gen_enums.py

if exist ../sfml-project2/GeneratedEnums.h (
    echo Successfully generated GeneratedEnums.h
) else (
    echo Generation failed.
)
pause