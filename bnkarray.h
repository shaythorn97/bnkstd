#pragma once

#include <stdlib.h>

#define bnk_Array(T) struct { T* data; int len; int cap; }
#define bnk_append(arr, el) arr.data[arr.len++] = el

#define bnk_new_array(arr, N) (__typeof__(arr)){ .data = malloc(N * sizeof(*arr.data)), .cap = N }
