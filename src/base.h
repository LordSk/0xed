#pragma once
#include <stdint.h>

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
