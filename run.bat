@echo off

call config.bat

pushd bin\win32\%Architecture%
%OutputName%
popd
