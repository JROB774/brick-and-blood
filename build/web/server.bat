@echo off

call config.bat
pushd ..\..\%OutputPath%
start python -m http.server
popd
