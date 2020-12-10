@echo off
setlocal

pushd ..\..

call build\win32\utility\findvsdev.bat
call build\win32\utility\buildvars.bat

call %VSDevPath% -no_logo -arch=%Architecture%

if not exist %OutputPath% mkdir %OutputPath%

if %BuildMode%==Release rc -nologo -i %ResourcePath% %ResourceFile%

call build\win32\utility\buildtime.bat "cl %IncludeDirs% %Defines% %CompilerFlags% %CompilerWarnings% -Fe%OutputExecutable% %InputSource% -link %LinkerFlags% %LinkerWarnings% %LibraryDirs% %Libraries% %InputResource%"
copy source\depends\libsdl2\library\%Architecture%\*.dll %OutputPath%

pushd %OutputPath%
if %BuildMode%==Release del %ResourcePath%*.res
del *.ilk *.res *.obj *.exp *.lib
popd
del *.ilk *.res *.obj *.exp *.lib

popd

endlocal
