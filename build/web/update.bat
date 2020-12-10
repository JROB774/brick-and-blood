@echo off
setlocal

call config.bat

pushd %EMSDKRootDir%
git pull
emsdk install latest
popd

endlocal
