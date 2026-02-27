#pragma once

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdalign.h>

typedef struct {
    const char* data;
    int64_t len;
} String8;

#define S(s) (String8){ s, sizeof(s) - 1 }

String8 string_from(String8 s, int64_t from);
String8 string_to(String8 s, int64_t to);
String8 string_from_to(String8 s, int64_t from, int64_t to);
bool string_compare(String8 s1, String8 s2);
bool string_contains(String8 s, String8 contains);
int64_t string_find(String8 s, String8 find);

String8 string_copy(char* buf, uint64_t bufSize, String8 s);
String8 string_append(char* buf, uint64_t, String8 s, String8 app);

char* string_to_cstr(char* buf, uint64_t bufSize, String8 s);

// #ifdef BNK_STRING_IMPLEMENTATION

String8 string_from(String8 s, int64_t from) {
    // we need to do some checks in here

    return (String8){
        .data = s.data + from,
        .len = s.len - from
    };
}

String8 string_to(String8 s, int64_t to) {
    return (String8){
        .data = s.data,
        .len = to
    };
}

 String8 string_from_to(String8 s, int64_t from, int64_t to) {
    return (String8){
        .data = s.data + from,
        .len = to - from
    };
}

bool string_compare(String8 s1, String8 s2) {
    return s1.len == s2.len && !memcmp(s1.data, s2.data, s1.len);
}

bool string_contains(String8 s, String8 contains) {
    for (int64_t i = 0; i < s.len; i++) {
        if (contains.len > s.len - i) break;
        if (!memcmp(s.data + i, contains.data, contains.len)) return true;
    }

    return false;
}

int64_t string_find(String8 s, String8 find) {
    for (int64_t i = 0; i < s.len; i++) {
        if (find.len > s.len - i) break;
        if (!memcmp(s.data + i, find.data, find.len)) return i;
    } 

    return -1;
}

char* string_to_cstr(char* buf, uint64_t bufSize, String8 s) {
    if (bufSize < s.len + 1) return NULL;

    memcpy(buf, s.data, s.len);
    buf[s.len] = '\0';

    return buf;
}

#endif
