#pragma once
#include <stdint.h>

typedef int8_t i8;
typedef uint8_t u8;
typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;

#define qstr_cstr(str) str.toLocal8Bit().constData()

#ifndef min
    #define min(a, b) ((a) < (b) ? (a) : (b))
#endif
