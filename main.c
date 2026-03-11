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

Allocator new_allocator(Arena* arena) {
    return (Allocator){
        .alloc = allocator_arena_alloc,
        .free = allocator_arena_free,
        .ctx = arena,
    };
}

typedef struct {
    String8 name;
    float balance;
} User;

User* new_user(Allocator* allocator, String8 name, float balance) {
    User* user = allocator_alloc_type(allocator, User);
    *user = (User){
        .name = name,
        .balance = balance,
    };  

    return user;
}

int main() {
    Arena persistent = new_arena(1024);
    Allocator aa = new_allocator(&persistent);

    User* user = new_user(&aa, S("Barry"), 100.0f);
    printf("User: %.*s, %f\n", (int)user->name.len, user->name.data, user->balance);
}
