/* NOTE: This is more like a temp array */

template<class T>
Array<T> *ArrayAlloc(Arena *arena, u64 capacity)
{
    Array *array = ArenaPushStruct(arena, Array);
    array->base = arena->base + arena->pos;
    array->capacity = capacity;
    array->size = 0;

    // Make room for the array elements on the Arena.
    u64 array_capacity_size = typeof(T) * capacity;
    ArenaPush(arena, array_capacity_size);

    return array;
}

template<class T>
void Append(Array<T> *array, T elm)
{
    u64 new_size = array->size + 1;    
    if (array->capacity < new_size)
    {
        // Double the array capacity.
        u64 increment_size = sizeof(T) * array->capacity;
        ArenaPush(array->arena, increment_size);
        array->capacity += array->capacity;
    }
    // TODO: Find a better alternative to just copying.
    T* next_free_elm = array->base + array->size;
    *next_free_elm = elm;

    array->size++;
}

template<class T>
T* Get(Array<T> *array, u64 index)
{
    assert(index < array->size && index >= 0);
    return array->base + index;
}

template<class T>
void RemoveLast(Array<T> *array)
{
    ArenaPopStruct(array->arena, T);
}

