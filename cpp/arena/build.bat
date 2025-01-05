
@echo off

mkdir build
pushd build

set LIBS=onecore.lib
set CFLAGS=/std:c++20 /Zi /EHsc

cl %CFLAGS% ..\main.cpp %LIBS%

popd
