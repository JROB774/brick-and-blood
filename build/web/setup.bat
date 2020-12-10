@echo off
setlocal

call config.bat

pushd %EMSDKRootDir%
emsdk activate latest
emsdk_env.bat
popd

endlocal
