#pragma once

#include "utils.h"

enum class InstType: i32 {
    _INVALID = 0,
    PLACE_BRICK,
    STRUCT_BEGIN,
    STRUCT_END,
    STRUCT_ADD_BRICK,
};

struct Instruction
{
    InstType type = InstType::_INVALID;
    void* args[2];
};

struct Script
{
    FileBuffer file;
    Array<Instruction> code;

    bool openAndCompile(const char* path);
};
