#include <assert.h>
#include <stdalign.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define KB(x) (1024 * (x))
#define MB(x) ((1024 * 1024) * (x))
#define GB(x) ((1024 * 1024 * 1024) * (x))

typedef struct Arena Arena;
struct Arena {
    uint8_t* data; 
    size_t cap;
    size_t pos;
    size_t committedSize;
};

static size_t GetPageSize() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwPageSize;
}

Arena NewArena(size_t size) {
    size = (size + GetPageSize() - 1) / GetPageSize() * GetPageSize();

    return (Arena){ 
        .data = VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_NOACCESS),
        .cap = size
    };
}

void ArenaReset(Arena* a) {
    a->pos = 0;
}

void ArenaDelete(Arena* a) {
    VirtualFree(a->data, 0, MEM_RELEASE); 
}

// we need to pad this
void* ArenaPush(Arena* a, size_t size, size_t count) {
    size_t newOffset = a->pos + (size * count);
    assert(newOffset <= a->cap);

    if (newOffset > a->committedSize) {
        size_t commitTarget = (newOffset + GetPageSize() - 1) / GetPageSize() * GetPageSize();

        if (commitTarget > a->cap) {
            commitTarget = a->cap;
        }

        size_t commitSize = commitTarget - a->committedSize;
        void* commitStartAddr = a->data + a->committedSize;

        if (!VirtualAlloc(commitStartAddr, commitSize, MEM_COMMIT, PAGE_READWRITE)) {
            return NULL;
        }

        a->committedSize = commitTarget;
    }

    void* ptr = a->data + a->pos;
    a->pos = newOffset;

    return ptr;
}

void* ArenaGetPos(Arena* a) {
    return a->data + a->pos;
}

#define new(a, t) ArenaPush(a, sizeof(t), 1)
#define new_array(a, t, n) ArenaPush(a, sizeof(t), (n))

typedef struct Particle Particle;
struct Particle {
    struct { float x, y; } pos;
    struct { uint32_t w, h; } size;
    float lifetime;
};

int main() {
    Arena a = NewArena(MB(8));
    printf("Address: %p\n", a.data);

    int* i = new(&a, int);
    *i = 10;
    printf("Address: %p\n", i);
    printf("Value: %i\n", *i);

    double* d = new(&a, double);
    *d = 99.0f;
    printf("Address: %p\n", d);
    printf("Value: %f\n", *d);

    Particle* particles = new_array(&a, Particle, 10000);
    printf("Address: %p\n", particles);

    ArenaDelete(&a);
}
