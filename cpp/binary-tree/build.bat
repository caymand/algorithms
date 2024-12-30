@echo off

mkdir build
pushd build

set CFLAGS=/std:c++20

cl /Zi %CFLAGS% ..\main.cpp

popd
