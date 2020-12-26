@echo off

call config.bat

C:\

pushd %EMSDKRootDir%
emsdk activate latest
emsdk_env.bat
popd
