#include <stdio.h>
#include <stdlib.h>

#define Array(T) struct Array_##T { T* data; size_t len; size_t cap; }

#define new_array(arr, n) (__typeof__(arr)){ .data = malloc(sizeof(arr) * (n)), .cap = (n) }
#define array_append(arr, v) do { (arr)->data[(arr)->len++] = (v); } while (0)

Array(int) nums() {
    Array(int) ret = new_array(ret, 256);
    array_append(&ret, 0);
    return ret;
}

typedef struct {
    Array(int) textures;
} TextureManager;

int main() {
    TextureManager tm = {0};
    tm.textures = new_array(tm.textures, 8);
    foo(&tm.textures);

    for (int i = 0; i < tm.textures.len; i++) {
        printf("%i,", tm.textures.data[i]);
    }

    return 0;
}
