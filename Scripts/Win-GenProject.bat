@echo off

pushd ..\

call Vendor\premake\premake5.exe vs2022

popd

pause