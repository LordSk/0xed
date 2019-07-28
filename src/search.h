#pragma once
#include "base.h"
#include "utils.h"

struct SearchDataType
{
    enum Enum: i32 {
        ASCII_String,
        Integer,
        Float,
    };
};

struct SearchParams
{
    char str[64] = {0};
    i64 vint;
    u64 vuint;
    f32 vf32;
    f32 vf64;

    u8 dataSize = 0;
    bool8 intSigned = true;
    SearchDataType::Enum dataType = SearchDataType::ASCII_String;

    enum Stride: i32 {
        Full=0,
        Twice,
        Even
    };
    Stride strideKind = Full;
};

struct SearchResult
{
	i64 offset;
    i32 len;
    SearchDataType::Enum type;
};

bool searchStartThread();
void searchTerminateThread();
void searchSetNewFileBuffer(BufferSlice nfb);
void searchNewRequest(const SearchParams& params, ArrayTS<SearchResult>* results);
