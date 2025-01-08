
template <class T>
struct Array
{    
    u64 size;                   // In Elements
    u64 capacity;               // In elements
    T *base;                    // Not really needed. Maybe use Arena.
    Arena *arena;    
};

template<class T>
Array<T> *ArrayAlloc(Arena *arena, u64 capacity);

template<class T>
void Append(Array<T> *array, T elm);

template<class T>
T Get(Array<T> *array, u64 index);

template<class T>
void RemoveLast(Array<T> *array);


#define INCREMENT_FACTOR
#define SHRINK_FACTOR
