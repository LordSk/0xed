#pragma once
#include "base.h"
#include <vector>

// TODO: replace this with our own "lsk array"
template<typename T>
struct Array: public std::vector<T>
{
    inline void push(T elem) {
        push_back(elem);
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
