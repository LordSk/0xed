#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

// http://www.gingerbill.org/article/defer-in-cpp.html
template<typename F>
struct __privDefer {
    F f;
    __privDefer(F f) : f(f) {}
    ~__privDefer() { f(); }
};

template<typename F>
__privDefer<F> __defer_func(F f) {
    return __privDefer<F>(f);
}

#define DO_GLUE(x, y) x##y
#define GLUE(x, y) DO_GLUE(x, y)
#define DEFER_VAR(x) GLUE(x, __COUNTER__)
#define defer(code) auto DEFER_VAR(_defer_) = __defer_func([&](){code;})
// ----------------------------------------------------

#define LOG(fmt, ...) (printf(fmt"\n", __VA_ARGS__), fflush(stdout))

#ifndef min
    #define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
    #define max(a, b) ((a) > (b) ? (a) : (b))
#endif

template<typename T>
T clamp(T val, T valmin, T valmax)
{
    if(val < valmin) return valmin;
    if(val > valmax) return valmax;
    return val;
}

// TODO: maybe move this somewhere elese
struct FileBuffer
{
    u8* data = nullptr;
    i64 size = 0;
};

inline bool openFileReadAll(const char* path, FileBuffer* out_fb)
{
    FILE* file = fopen(path, "rb");
    if(!file) {
        LOG("ERROR: Can not open file %s", path);
        return false;
    }

    if(out_fb->data) {
        free(out_fb->data);
        out_fb->data = nullptr;
        out_fb->size = 0;
    }

    FileBuffer fb;

    i64 start = ftell(file);
    fseek(file, 0, SEEK_END);
    i64 len = ftell(file) - start;
    fseek(file, start, SEEK_SET);

    fb.data = (u8*)malloc(len + 1);
    assert(fb.data);
    fb.size = len;

    // read
    fread(fb.data, 1, len, file);
    fb.data[len] = 0;

    fclose(file);
    *out_fb = fb;

    LOG("file loaded path=%s size=%llu", path, fb.size);
    return true;
}
