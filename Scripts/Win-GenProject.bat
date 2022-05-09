@echo off

pushd ..\

call Vendor\premake5\Binaries\premake5.exe vs2022

popd

pause