#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BNK_ARENA_IMPL
#include "bnkarena.h"

#define BNK_STRING_IMPL
#include "bnkstring.h"

int main() {
    String8 str = S8("Hello, World!");

    String8 from = string_from(str, 7);
    printf("%.*s\n", (int)from.len, from.data);

    String8 to = string_to(str, 5);
    printf("%.*s\n", (int)to.len, to.data);

    String8 fromTo = string_from_to(str, 3, 9);
    printf("%.*s\n", (int)fromTo.len, fromTo.data);

    bool compareTrue = string_compare(to, S8("Hello"));
    printf("Compare: %d\n", compareTrue);

    bool compareFalse = string_compare(to, S8("World!"));
    printf("Compare: %d\n", compareFalse);

    bool containsTrue = string_contains(str, S8("!"));
    printf("Contains: %d\n", containsTrue);

    bool containsFalse = string_contains(str, S8("-"));
    printf("Contains: %d\n", containsFalse);

    int64_t find = string_find(str, S8(","));
    printf("Found at index: %lld\n", find);
}
