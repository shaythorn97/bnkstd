#include "bnkalloc.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct Entity Entity;
struct Entity {
    Entity* next;
    int id;
    bool isActive;
    struct { float x, y; } pos;
};

typedef struct FreeList FreeList;
struct FreeList {
    Arena* arena;
    Entity* nextFree;
};

#define MemoryZeroStruct(v) memset((v), 0, sizeof(*(v)))

Entity* EntityAlloc(FreeList* list) {
    Entity* res = list->nextFree;
    if (res) {
        list->nextFree = list->nextFree->next;
        MemoryZeroStruct(res);
    } else {
        res = ArenaPushStructZero(list->arena, Entity);
    }

    return res;
}

void EntityRelease(FreeList* list, Entity* entity) {
    entity->next = list->nextFree;
    list->nextFree = entity;
}

int main() {
    Arena scene = ArenaCreate(sizeof(Entity) * 32);
    FreeList list = {
        .arena = &scene,
        .nextFree = NULL
    };

    int id = 1;

    Entity* gunner = EntityAlloc(&list);
    gunner->pos.x = 1;
    gunner->pos.y = 1;

    Entity* beg = (Entity*)list.arena->data;
    Entity* end = (Entity*)(list.arena->data + list.arena->pos);
    for (Entity* it = beg; it < end; it++) {
        if (it->isActive) {
            printf("%i, %f, %f\n", it->id, it->pos.x, it->pos.y);
        }
    }

    ArenaDelete(&scene);
}
