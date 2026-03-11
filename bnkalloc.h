#pragma once

#include <stdint.h>

typedef struct Allocator {
    void* (*alloc)(void* ctx, uint64_t size, uint64_t align);
    void (*free)(void* ctx, void* ptr);
    void* ctx;
} Allocator;

#define allocator_alloc_array(a, t, n) ((a)->alloc((a)->ctx, sizeof(t) * (n), _Alignof(t)))
#define allocator_alloc_type(a, t) allocator_alloc_array(a, t, 1)

// maybe helper funcs?
