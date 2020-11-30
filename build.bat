@echo off

call config.bat

if not exist bin\win32\%Architecture% mkdir bin\win32\%Architecture%
tcc -I src -L lib\win32 -lSDL2 -lSDL2_mixer -bench %ExtraFlags% -o bin\win32\%Architecture%\%OutputName% src\main_win32.c
