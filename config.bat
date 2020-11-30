@echo off

:: can be either "x86" or "x64"
set Architecture=x64
:: can be either "debug" or "release"
set BuildMode=debug

set ExtraFlags=

if %Architecture% == x86   set ExtraFlags=%ExtraFlags% -m32 -D BUILD_32BIT
if %Architecture% == x64   set ExtraFlags=%ExtraFlags% -m64 -D BUILD_64BIT
if %BuildMode%    == debug set ExtraFlags=%ExtraFlags% -D BUILD_DEBUG -g

set OutputName=SANTA.exe
