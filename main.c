#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BNK_ARENA_IMPLEMENTATION
#include "bnkarena.h"

#define BNK_STRING_IMPLEMENTATION
#include "bnkstring.h"

#include "bnkalloc.h"

// some examples of using bnkstring.h and using bnkarena.h

void* allocator_stdlib_alloc(void* ctx, uint64_t size, uint64_t align) {
    return malloc(size);
}

void allocator_stdlib_free(void* ctx, void* ptr) {
    free(ptr);
}

void* allocator_arena_alloc(void* ctx, uint64_t size, uint64_t align) {
    Arena* a = (Arena*)ctx;
    return arena_push(a, size, align);
}

void allocator_arena_free(void* ctx, void* ptr) {
    Arena* a = (Arena*)ctx;
    arena_reset(a);
}

int main() {
    Allocator std = {
        .alloc = allocator_stdlib_alloc,
        .free = allocator_stdlib_free,
    };

    Arena persistent = new_arena(1024);

    Allocator arena = {
        .alloc = allocator_arena_alloc,
        .free = allocator_arena_free,
        .ctx = &persistent,
    };

    float* f = allocator_alloc_type(&std, float);
}
