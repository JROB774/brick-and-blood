@echo off

:: Change this to whatever your EMSDK root directory is for this script to work.
set EMSDKRootDir=C:\.emsdk

set AssetPath=assets
set IncludeDirs=-I source\depends\libgon -I source\depends\libstb
set Libraries=-s USE_SDL=2 -s USE_SDL_MIXER=2
set InputSource=source\main_web.cpp
set OutputPath=binary\web\
set OutputName=%OutputPath%brick.html
