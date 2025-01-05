// Platoform headers
#include <Windows.h>
#include <Memoryapi.h>

#include <stdio.h>
#include <assert.h>

// CPP Libs
#include <string>
#include <iostream>

// My headers
#include "types.h"
#include "math.h"
#include "arena.h"
#include "arena.cpp"

int main()
{
    Arena *arena = ArenaAlloc(Gigabytes(1));
    std::string str = "Hello World\nFOO";
    char *str_copied = ArenaPushArray(arena, char, str.length());
    memcpy(str_copied, str.data(), str.length());
    puts(str_copied);
}
