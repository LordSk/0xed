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
    struct { char buff[64] = {0}; u8 len = 0; } str;
    struct { i64 val; } vint;
    struct { u64 val; } vuint;
    struct { f64 val; } vfloat;

    SearchDataType::Enum dataType = SearchDataType::ASCII_String;
    i32 stride = 1;
};

struct SearchResult
{
    u64 offset;
    i32 len;
    SearchDataType::Enum type;
};

bool searchStartThread();
void searchData(const SearchParams& params, Array<SearchResult>* results);
