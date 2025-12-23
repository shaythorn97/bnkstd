#pragma once

#include <stdbool.h>

#include "bnkalloc.h"

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
