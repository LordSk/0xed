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

    inline void pop() {
        std::vector<T>::pop_back();
    }

    inline void insert(i32 where, T elem) {
        std::vector<T>::insert(begin() + where, elem);
    }

    inline i32 count() const {
        return size();
    }

    inline T& last() {
        return back();
    }
};

/**
 *  List
 *  - growable array
 *  - can't access all elements sequentially (for now) due to buckets
 *  - pointers are not invalidated on resize (grow)
 */
template<typename T, i32 BUCKET_ELT_COUNT=1024>
struct List
{
    struct Bucket
    {
        T* buffer;
        u32 count;
        u32 capacity;
    };

    T stackData[BUCKET_ELT_COUNT];
    Array<Bucket> buckets;

    List() {
        Bucket buck;
        buck.capacity = BUCKET_ELT_COUNT;
        buck.buffer = stackData;
        buck.count = 0;
        buckets.push(buck);
    }

    ~List() {
        const i32 bucketCount = buckets.count();
        for(i32 i = 1; i < bucketCount; ++i) {
            free(buckets[i].buffer);
        }
        buckets.clear();
    }

    inline void _allocNewBucket() {
        const i32 shift = max(0, buckets.count()-1);
        Bucket buck;
        buck.capacity = BUCKET_ELT_COUNT << shift;
        buck.buffer = (T*)malloc(sizeof(T) * buck.capacity);
        buck.count = 0;
        buckets.push(buck);
    }

    inline T& push(T elem) {
        Bucket* buck = &buckets[buckets.count()-1];
        if(buck->count+1 >= buck->capacity) {
            _allocNewBucket();
        }
        buck = &buckets[buckets.count()-1];
        return (buck->buffer[buck->count++] = elem);
    }

    inline void clear() {
        // clear all but the first
        const i32 bucketCount = buckets.count();
        for(i32 i = 1; i < bucketCount; ++i) {
            free(buckets[i].buffer);
        }
        Bucket first = buckets[0];
        first.count = 0;
        buckets.clear();
        buckets.push(first);
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

inline u32 hash32(const void* data, const u32 dataLen)
{
    assert(dataLen > 0);
    u32 h = 0x811c9dc5;
    for(u32 i = 0; i < dataLen; ++i) {
        h = h ^ ((u8*)data)[i];
        h = h * 16777619;
    }
    return h;
}

template<i32 STR_LEN>
struct StrT
{
    char str[STR_LEN];
    i32 len = 0;

    StrT() = default;

    StrT(const char* _str) {
        set(_str);
    }

    void set(const char* _str) {
        len = strlen(_str);
        assert(len < STR_LEN);
        memmove(str, _str, len);
        str[len] = 0;
    }

    void set(const char* _str, i32 _len) {
        assert(_len < STR_LEN);
        memmove(str, _str, _len);
        str[_len] = 0;
        len = _len;
    }
};

typedef StrT<32> Str32;
typedef StrT<64> Str64;
