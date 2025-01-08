@echo off

mkdir build
pushd build

set LIBS=onecore.lib
set CFLAGS=/std:c++20 /Zi /EHsc
set INC=/I..\

cl %CFLAGS% %INC% ..\binary-tree\main.cpp %LIBS%

popd
