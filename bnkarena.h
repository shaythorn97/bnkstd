#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// include this
// #define BNK_ARENA_IMPL

#define KB(x) ((x) * 1024)
#define MB(x) ((x) * (1024 * 1024))
#define GB(x) ((uint64_t)(x) * (1024 * 1024 * 1024))

typedef struct {
    uint8_t* data;
    uint64_t offset;
    uint64_t cap;
} Arena;

Arena new_arena(uint64_t size);
void arena_free(Arena* a);
void arena_reset(Arena* a);

// we need to slightly change this so we are committing in blocks of 64kb?
void* arena_push(Arena* a, uint64_t size, uint64_t align, bool zero);

#define arena_push_array(a, t, n) arena_push((a), sizeof(t) * (n), _Alignof(t), false)
#define arena_push_struct(a, t) arena_push_array((a), t, 1)
#define arena_push_array_zero(a, t, n) arena_push((a), sizeof(t) * (n), _Alignof(t), true)
#define arena_push_struct_zero(a, t) arena_push_array_zero((a), t, 1)

#ifdef BNK_ARENA_IMPL

// we need to make sure we are handling linux shite
#ifdef _WIN32
    #define WIN32(r) __declspec(dllimport) r __stdcall

    WIN32(void*) VirtualAlloc(void*, uint64_t, uint32_t, uint32_t);
    WIN32(int32_t) VirtualFree(void*, uint64_t, uint32_t);

    #define MEM_COMMIT 0x1000
    #define MEM_RESERVE 0x2000
    #define MEM_RESET 0x8000
    #define PAGE_READWRITE 0x04
#endif

// need to implement VirtualAlloc and mmap instead of pants malloc
Arena new_arena(uint64_t size) {
    return (Arena){
        .data = (uint8_t*)VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_READWRITE),
        .cap = size
    };
}

void arena_free(Arena* a) {
    if (a->data) {
        VirtualFree(a->data, 0, PAGE_READWRITE);
    }
}

void arena_reset(Arena* a) {
    a->offset = 0;
}

void* arena_push(Arena* a, uint64_t size, uint64_t align, bool zero) {
    uint64_t padding = -a->offset & (align - 1);
    if (a->offset + size + padding > a->cap) return NULL;

    void* ptr = a->data + a->offset + padding;
    a->offset += size + padding;

    if (zero) {
        memset(a->data, 0, size);
    }

    return ptr;
}

#endif
