
struct Arena
{
    // Position in the arena. This can never exceed the size.
    u64 pos;
    // Position of the memory that has been commited
    u64 commit_pos;
    // Size of the entire arena in reserved virtual memory.
    u64 size;
    // TODO: Maybe make an alignment requirement.
    
    // The actual arena is allocated within the arena. This is the
    // initial pointer to the memory where the arena is stored.
    u8 *base;
};    

void MemDecommit(void *ptr, u64 size);
void MemCommit(void *ptr, u64 size);
void* MemReserve(u64 size);
