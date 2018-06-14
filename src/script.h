#pragma once

#include "utils.h"

enum class InstType: i32 {
    _INVALID = 0,
    PLACE_BRICK,
    PLACE_BRICK_STRUCT,
    BRICK_STRUCT_BEGIN,
    BRICK_STRUCT_END,
    BRICK_STRUCT_ADD_MEMBER,
};

struct Instruction
{
    InstType type = InstType::_INVALID;
    intptr_t args[6];
};

struct Script
{
    FileBuffer file;
    u8* bytecodeData = nullptr;
    u32 bytecodeDataCur = 0;
    u32 bytecodeDataSize = 0;
    Array<Instruction> bytecode;

    ~Script() { release(); }
    void release();

    bool openAndCompile(const char* path);
    bool execute(struct BrickWall* wall);

    u32 _pushBytecodeData(const void* data, u32 dataSize);
};
