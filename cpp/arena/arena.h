
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

// Platform specific functions
void MemDecommit(void *ptr, u64 size);
u64 MemCommit(void *ptr, u64 size);
void* MemReserve(u64 size);

// Arena API functions
Arena* ArenaAlloc(u64 size);
void ArenaDestroy(Arena *arena);
void* ArenaPush(Arena *arena, u64 size);
void ArenaPopTo(Arena* arena, u64 pos);
void ArenaPop(Arena* arena, u64 size);

#define ArenaPushStruct(A, T) (ArenaPush((A), sizeof(T)))
#define ArenaPushArray(A, T, C) ((T *) (ArenaPush((A), sizeof(T) * (C))))

