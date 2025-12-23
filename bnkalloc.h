#pragma once

#include <stdlib.h>

#include "bnktypes.h"

#define KB(x) ((x) * 1024)
#define MB(x) ((x) * (1024 * 1024))
#define GB(x) ((u64)(x) * (1024 * 1024 * 1024))

typedef struct Arena Arena;
struct Arena {
    char* data;
    u64 cap;
    u64 pos;
};

Arena ArenaCreate(u64 size);
void ArenaDelete(Arena* a);

void* ArenaPush(Arena* a, u64 size, u64 alignment);
void* ArenaPushZero(Arena* arena, u64 size, u64 alignment);

#define ArenaPushArray(a, t, n) ArenaPush((a), sizeof(t) * (n), _Alignof(t))
#define ArenaPushArrayZero(a, t, n) ArenaPushZero((a), sizeof(t) * (n), _Alignof(t))
#define ArenaPushStruct(a, t) ArenaPushArray((a), t, 1)
#define ArenaPushStructZero(a, t) ArenaPushArrayZero((a), t, 1)

void ArenaReset(Arena* arena);
