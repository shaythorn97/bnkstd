#pragma once

#include <stdbool.h>

#define BNKSTRINGDEF static inline

typedef struct Arena Arena;

typedef struct {
    const char* data;
    size_t length;
} String;

#define S(s) (String){ s, sizeof(s) - 1 }

String StringFrom(String s, int from);
String StringTo(String s, int to);
String StringFromTo(String s, int from, int to);

bool StringCompare(String s1, String s2);

String StringCopy(Arena* buf, String s);
String StringAppend(Arena* buf, String s1, String s2);

char* StringToCString(Arena* arena, String s);
size_t StringCStrSize(String s);

#ifdef BNK_STRING_IMPLEMENTATION

BNKSTRINGDEF String StringFrom(String s, int from) { return (String){0}; }
BNKSTRINGDEF String StringTo(String s, int to) { return (String){0}; }
BNKSTRINGDEF String StringFromTo(String s, int to) {return (String){0}; }

#endif
