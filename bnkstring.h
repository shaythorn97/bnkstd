#pragma once

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define BNKSTRINGDEF

// TODO
// - this should be replaced with an allocator, the reason being is so that I can use some other memory allocation method if I want
// - change the type from int to something like int64_t
typedef struct Arena Arena;

typedef struct {
    const char* data;
    int64_t len;
} bnk_String;

#define S(s) (bnk_String){ s, sizeof(s) - 1 }

/* this gives us a string that is from the index we want to the end of the string */
bnk_String bnk_string_from(bnk_String s, int64_t from);
bnk_String bnk_string_to(bnk_String s, int64_t to);
bnk_String bnk_string_from_to(bnk_String s, int64_t from, int64_t to);
bool bnk_string_compare(bnk_String s1, bnk_String s2);
bool bnk_string_contains(bnk_String s, bnk_String substr);
int64_t bnk_string_find(bnk_String s, bnk_String substr);

bnk_String bnk_string_copy(Arena* a, bnk_String s);
bnk_String bnk_string_append(Arena* a, bnk_String s1, bnk_String s2);

char* bnk_string_cstr(Arena* a, bnk_String s);
size_t bnk_string_cstr_size(bnk_String s);

#ifdef BNK_STRING_IMPLEMENTATION

BNKSTRINGDEF bnk_String bnk_string_from(bnk_String s, int64_t from) {
    // we need to do some checks in here

    return (bnk_String){
        .data = s.data + from,
        .len = s.len - from
    };
}

BNKSTRINGDEF bnk_String bnk_string_to(bnk_String s, int64_t to) {
    return (bnk_String){
        .data = s.data,
        .len = to
    };
}

BNKSTRINGDEF bnk_String bnk_string_from_to(bnk_String s, int64_t from, int64_t to) {
    return (bnk_String){
        .data = s.data + from,
        .len = to - from
    };
}

BNKSTRINGDEF bool bnk_string_compare(bnk_String s1, bnk_String s2) {
    return s1.len == s2.len && !memcmp(s1.data, s2.data, s1.len);
}

bool bnk_string_contains(bnk_String s, bnk_String substr) {
    for (int64_t i = 0; i < s.len; i++) {
        if (substr.len > s.len - i) break;
        if (!memcmp(s.data + i, substr.data, substr.len)) return true;
    }

    return false;
}

int64_t bnk_string_find(bnk_String s, bnk_String substr) {
    for (int64_t i = 0; i < s.len; i++) {
        if (substr.len > s.len - i) break;
        if (!memcmp(s.data + i, substr.data, substr.len)) return i;
    } 

    return -1;
}

#endif
