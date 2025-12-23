#include "bnkalloc.h"

#include <assert.h>
#include <string.h>

Arena* NewArena(uint64_t size) {
    Arena* a = malloc(sizeof(Arena));
    a->data = malloc(size);
    a->cap = size;
    a->pos = 0;
    return a;
}

Arena ArenaCreate(u64 size) {
    return (Arena){
        .data = malloc(size),
        .cap = size,
        .pos = 0
    }; 
}

void ArenaDelete(Arena* arena) {
    if (arena->data)
        free(arena->data);
}

void* ArenaPush(Arena* arena, u64 size, u64 alignment) {
    assert(arena->pos + size <= arena->cap);

    void* ptr = arena->data + arena->pos;
    arena->pos += size;
    return ptr;
}

void* ArenaPushZero(Arena* arena, u64 size, u64 alignment) {
    assert(arena->pos + size <= arena->cap);

    void* ptr = arena->data + arena->pos; 
    memset(ptr, 0, size);
    arena->pos += size;
    return ptr;
}

void ArenaClear(Arena* arena) {
    arena->pos = 0;
}
