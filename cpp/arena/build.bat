@echo off

mkdir build
pushd build

set LIBS=onecore.lib
set CFLAGS=/std:c++20 /Zi

cl %CFLAGS% ..\arena.cpp %LIBS%

popd
