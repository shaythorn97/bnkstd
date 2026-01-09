#include "bnkalloc.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MemoryZeroStruct(v) memset((v), 0, sizeof(*(v)))

int main() {
    Arena scene = ArenaCreate(sizeof(Entity) * 32);

    ArenaDelete(&scene);
}
