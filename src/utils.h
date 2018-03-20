#pragma once
#include "base.h"
#include <vector>

// TODO: replace this with our own "lsk array"
template<typename T>
struct Array: public std::vector<T>
{
    inline T& push(T elem) {
        push_back(elem);
        return *(end()-1);
    }

    inline void insert(i32 where, T elem) {
        std::vector<T>::insert(begin() + where, elem);
    }

    inline i32 count() const {
        return size();
    }
};

struct FileBuffer
{
    u8* data = nullptr;
    i64 size = 0;
};

bool openFileReadAll(const char* path, FileBuffer* out_fb);

inline u32 colorAddAllChannels(u32 color, i32 add)
{
    i32 hoverR = color & 0xff;
    i32 hoverG = (color & 0xff00) >> 8;
    i32 hoverB = (color & 0xff0000) >> 16;
    hoverR = clamp(hoverR + add, 0, 255);
    hoverG = clamp(hoverG + add, 0, 255);
    hoverB = clamp(hoverB + add, 0, 255);
    return (0xff000000 | (hoverB << 16) | (hoverG << 8) | hoverR);
}

inline f32 colorAvgChannel(u32 color)
{
    i32 hoverR = color & 0xff;
    i32 hoverG = (color & 0xff00) >> 8;
    i32 hoverB = (color & 0xff0000) >> 16;
    f32 total = hoverR + hoverG + hoverB;
    return total / 3.0f;
}

inline u32 colorOne(u8 chan1)
{
    return 0xff000000 | (chan1 << 16) | (chan1 << 8) | chan1;
}
