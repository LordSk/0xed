#pragma once

#include "utils.h"

enum class InstType: i32 {
    _INVALID = 0,
    PLACE_BRICK,
    BRICK_STRUCT_BEGIN,
    BRICK_STRUCT_END
};

struct Instruction
{
    InstType type = InstType::_INVALID;
    intptr_t args[5];
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

    u32 _pushBytecodeData(void* data, u32 dataSize);
};
