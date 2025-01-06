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
// Returns in bytes the ammount of memory commited.
u64 MemCommit(void *ptr, u64 size)
{    
    u32 page_size = PageSize();

    // Allocate in pages
    u64 upper_bound = size + page_size - 1;
    u64 excess = upper_bound % page_size;
    u64 pages_bytes = upper_bound - excess;
    
    VirtualAlloc(ptr, pages_bytes, MEM_COMMIT, PAGE_READWRITE);
    return pages_bytes;
}

// Decomits all pages contained in the interval [ptr, ptr + size].
void MemDecommit(void *ptr, u64 size)
{
    // No need to find nearest page. This is done automatically
    VirtualFree(ptr, size, MEM_DECOMMIT);
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

// TODO: Alignment
void* ArenaPush(Arena *arena, u64 size)
{
    void *result = nullptr;
    // In case an allocation is made that exceeds the arena size.
    assert(arena->pos + size < arena->size);

    result = arena->base + arena->pos;
    arena->pos += size;
    // In case we need to commmit more pages to satisfy the allocation.
    if (arena->commit_pos < arena->pos)
    {
        // Bytes needed to be committed to satisfy the allocation.        
        u64 to_commit = arena->pos - arena->commit_pos;        
        u64 commited_bytes = MemCommit(arena->base + arena->commit_pos,
                                       to_commit);
        assert(commited_bytes); // Always commit some pages.
        arena->commit_pos += commited_bytes;
    }
    
    return result;
}

/*
  |       c'  p'    p   c
 */
void ArenaPopTo(Arena* arena, u64 pos)
{
    assert(pos <= arena->pos);
    
    u64 new_pos = max(sizeof(Arena), pos);
    arena->pos = new_pos;
    u64 page_size = PageSize();
    u64 commit_pos_upper_bound = arena->pos + (page_size - 1);
    u64 commit_pos_excess = commit_pos_upper_bound % page_size;    
    u64 commit_pos_aligned_to_page = commit_pos_upper_bound - commit_pos_excess;

    // If the new pos falls into a new page, decommit the old pages.
    // TODO: Should have a threshold to avoid thrashing.
    if (commit_pos_aligned_to_page < arena->commit_pos)
    {
        u64 decommit_size = arena->commit_pos - commit_pos_aligned_to_page;
        MemDecommit(arena->base + commit_pos_aligned_to_page, decommit_size);
        arena->commit_pos -= decommit_size;
    }    
}

void ArenaPop(Arena* arena, u64 size)
{
    // In case you try to pop more than the arena size, pop back to the first
    // availble memory.
    u64 size_to_pop = min(arena->pos, size);   
    u64 new_pos = arena->pos - size_to_pop;
    // We first place the Arena in the memory of the arena.
    u64 min_pos = sizeof(Arena);
    new_pos = max(min_pos, new_pos);
    ArenaPopTo(arena, new_pos);
}

    


