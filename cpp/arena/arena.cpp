// Platoform headers
#include <Windows.h>
#include <Memoryapi.h>

#include <stdio.h>
#include <assert.h>

// My headers
#include "types.h"
#include "math.h"
#include "arena.h"

// OS Specific allocation primitives.

// Reserve some memory. Rounds to nearest gigabyte.
void* MemReserve(u64 size)
{
    // Round to nearest gigabytes
    u64 upper_bound = size + Gigabytes(1) - 1;
    u64 excess = upper_bound%Gigabytes(1);
    u64 nearest_gb_size = upper_bound - excess;
    
    void *ptr = VirtualAlloc(nullptr,
                             nearest_gb_size,
                             MEM_RESERVE,
                             PAGE_NOACCESS);
    assert(ptr);
    return ptr;
}

u64 PageSize()
{
    // Find page size for this system
    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);
    
    return sys_info.dwPageSize;
}


// Commits pages of virtual memory.
void MemCommit(void *ptr, u64 size)
{    
    u32 page_size = PageSize();

    // Allocate in pages
    u64 upper_bound = size + page_size - 1;
    u64 excess = upper_bound % page_size;
    u64 pages_bytes = upper_bound - excess;
    
    VirtualAlloc(ptr, pages_bytes, MEM_COMMIT, PAGE_READWRITE);
    return;
}

// Decomits all pages contained in the interval [ptr, ptr + size].
void MemDecommit(void *ptr, u64 size)
{
    // No need to find nearest page. This is done automatically
    VirtualFree(ptr, size, MEM_DECOMMIT);
}

void* ArenaPush(Arena *arena, u64 size)
{
    return nullptr;
}

// Allocate an arena with size reserved virtual memory.
Arena* ArenaAlloc(u64 size)
{
    void *reserved_block = MemReserve(size);
    // Commit the very first page to store the arena.
    u64 initial_commit_size = PageSize();
    assert(sizeof(Arena) <= initial_commit_size);
    MemCommit(reserved_block, initial_commit_size);

    Arena *arena = (Arena*) reserved_block;
    arena->pos = sizeof(Arena);
    arena->commit_pos = initial_commit_size;
    arena->size = size;
    // Not really needed since it will be the same as arena*
    arena->base = (u8*) arena;
    
    return arena;
}

void ArenaDestroy(Arena *arena)
{
    // Decommit all the pages that could be used by the arena.
    // TODO: Maybe better to use the commit_pos?
    MemDecommit(arena, arena->size);
}


int main()
{
    Arena *arena = ArenaAlloc(Gigabytes(1));
    char *str = (char *)arena->base + arena->pos;
    char hello_world[] = "Hello, World!";
    memcpy(str, hello_world, strlen(hello_world));
    puts(str);
}
