@echo off
echo Building GameObjects...

set "path=../../bin/GameObjects.bin"
del "%path%" 2>nul

C:\Windows\py.exe ../Scripts/gen_objects_library.py

if exist "%path%" (
    echo Successfully generated GameObjects.bin
) else (
    echo Generation failed.
)