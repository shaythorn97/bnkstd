#pragma once

#include <cstdint>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdalign.h>

// include this to use!!!
#define BNK_STRING_IMPL

typedef struct Allocator Allocator;

typedef struct {
    uint8_t* data;
    int64_t len;
} String8;

#define S(s) (String8){ s, sizeof(s) - 1 }

String8 string_from(String8 s, int64_t from);
String8 string_to(String8 s, int64_t to);
String8 string_from_to(String8 s, int64_t from, int64_t to);
bool string_compare(String8 s1, String8 s2);
bool string_contains(String8 s, String8 contains);
int64_t string_find(String8 s, String8 find);

String8 string_copy(uint8_t* buf, uint64_t bufSize, String8 s);
String8 string_append(uint8_t* buf, uint64_t bufSize, String8 s, String8 app);
uint8_t* string_to_cstr(uint8_t* buf, uint64_t bufSize, String8 s);

// with new custom allocator
String8 string_copy_alloc(Allocator allocator, String8 s);
String8 string_append_alloc(Allocator allocator, String8 s, String8 app);
uint8_t* string_to_cstr_alloc(Allocator allocator, String8 s);

typedef struct {
    String8 str;
    int64_t cap;
} StringBuilder;

void sb_append_string(StringBuilder* sb, String8 app);
void sb_append_u8(StringBuilder* sb, uint8_t app);

#ifdef BNK_STRING_IMPL

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

String8 string_copy(uint8_t* buf, uint64_t bufSize, String8 s) {
    if (!buf || bufSize < s.len) return (String8){0};

    memcpy(buf, s.data, s.len);

    return (String8){
        .data = buf,
        .len = s.len,
    };
}

String8 string_append(uint8_t* buf, uint64_t bufSize, String8 s, String8 app) {
    if (!buf || bufSize < s.len + app.len) return (String8){0};

    memcpy(buf, s.data, s.len);
    memcpy(buf + s.len, app.data, app.len);

    return (String8){
        .data = buf,
        .len = s.len + app.len,
    };
}

uint8_t* string_to_cstr(uint8_t* buf, uint64_t bufSize, String8 s) {
    if (bufSize < s.len + 1) return NULL;

    memcpy(buf, s.data, s.len);
    buf[s.len] = '\0';

    return buf;
}

void sb_append_string(StringBuilder *sb, String8 app) {
    // if the capacity is big enough then we can add it on
    if (sb->str.len + app.len > sb->cap) return;

    memcpy(sb->str.data + sb->str.len, app.data, app.len);
}

void sb_append_uint8_t(StringBuilder *sb, uint8_t app) {
    if (sb->str.len + 1 > sb->cap) return;

    sb->str.data[sb->str.len++] = app;
}

#endif
