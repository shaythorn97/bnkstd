#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define KB(x) ((x) * 1024)
#define MB(x) ((x) * (1024 * 1024))
#define GB(x) ((uint64_t)(x) * (1024 * 1024 * 1024))

typedef struct {
    uint8_t* data;
    uint64_t offset;
    uint64_t cap;
} Arena;

Arena new_arena(uint64_t size);
void arena_delete(Arena* a);
void arena_reset(Arena* a);

void* arena_push(Arena* a, uint64_t size, uint64_t align);
void* arena_push_zero(Arena* a, uint64_t size, uint64_t align);

#define arena_push_array(a, t, n) arena_push((a), sizeof(t) * (n), _Alignof(t))
#define arena_push_struct(a, t) arena_push_array((a), t, 1)
#define arena_push_array_zero(a, t, n) arena_push_zero((a), sizeof(t) * (n), _Alignof(t))
#define arena_push_struct_zero(a, t) arena_push_array_zero((a), t, 1)

#ifdef BNK_ARENA_IMPLEMENTATION

// we need to make sure we are handling linux shite
#ifdef _WIN32
    #define WIN32(r) __declspec(dllimport) r __stdcall
    WIN32(void*) VirtualAlloc(void*, uint64_t, uint32_t, uint32_t);
    WIN32(uint32_t) VirtualFree(void*, uint64_t, uint32_t);
#endif

// need to implement VirtualAlloc and mmap instead of pants malloc
Arena new_arena(uint64_t size) {
    return (Arena){
        .data = (uint8_t*)VirtualAlloc(NULL, size, 0x1000 | 0x2000, 0x04),
        .cap = size
    };
}

void arena_delete(Arena* a) {
    if (a->data) {
        VirtualFree(a->data, 0, 0x8000);
    }
}

void arena_reset(Arena* a) {
    a->offset = 0; 
}

void* arena_push(Arena* a, uint64_t size, uint64_t align) {
    uint64_t padding = -a->offset & (align - 1);
    if (a->offset + size + padding > a->cap) return NULL;

    void* ptr = a->data + a->offset + padding;
    a->offset += size + padding;

    return ptr;
}

void* arena_push_zero(Arena* a, uint64_t size, uint64_t align) {
    uint64_t padding = -a->offset & (align - 1); 
    if (a->offset + size + padding > a->cap) return NULL;

    void* ptr = a->data + a->offset + padding;
    a->offset += size + padding;

    memset(ptr, 0, size);
    return ptr;
}

#endif
