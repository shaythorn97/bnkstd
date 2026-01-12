#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define bnk_KB(x) ((x) * 1024)
#define bnk_MB(x) ((x) * (1024 * 1024))
#define bnk_GB(x) ((uint64_t)(x) * (1024 * 1024 * 1024))

typedef struct {
    uint8_t* data;
    uint64_t offset;
    uint64_t cap;
} bnk_Arena;

bnk_Arena bnk_new_arena(uint64_t size);
void bnk_arena_delete(bnk_Arena* a);
void bnk_arena_reset(bnk_Arena* a);

void* bnk_arena_push(bnk_Arena* a, uint64_t size, uint64_t align);
void* bnk_arena_push_zero(bnk_Arena* a, uint64_t size, uint64_t align);

#define bnk_arena_push_array(a, t, n) bnk_arena_push((a), sizeof(t) * (n), _Alignof(t))
#define bnk_arena_push_struct(a, t) bnk_arena_push_array((a), t, 1)
#define bnk_arena_push_array_zero(a, t, n) bnk_arena_push_zero((a), sizeof(t) * (n), _Alignof(t))
#define bnk_arena_push_struct_zero(a, t) bnk_arena_push_array_zero((a), t, 1)

#ifdef BNK_ALLOC_IMPLEMENTATION

// need to implement VirtualAlloc and mmap instead of pants malloc
bnk_Arena bnk_new_arena(uint64_t size) {
    return (bnk_Arena){
        .data = malloc(size),
        .cap = size
    };
}

void bnk_arena_delete(bnk_Arena* a) {
    if (a->data) free(a->data);
}

void bnk_arena_reset(bnk_Arena* a) {
    a->offset = 0; 
}

void* bnk_arena_push(bnk_Arena* a, uint64_t size, uint64_t align) {
    uint64_t padding = -a->offset & (align - 1); 
    if (a->offset + size + padding > a->cap) return NULL;

    void* ptr = a->data + a->offset + padding;
    a->offset += size + padding;

    return ptr;
}

void* bnk_arena_push_zero(bnk_Arena* a, uint64_t size, uint64_t align) {
    uint64_t padding = -a->offset & (align - 1); 
    if (a->offset + size + padding > a->cap) return NULL;

    void* ptr = a->data + a->offset + padding;
    a->offset += size + padding;

    memset(ptr, 0, size);
    return ptr;
}

#endif
