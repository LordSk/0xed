#pragma once
#include "base.h"
#include "utils.h"

enum class BrickType: i32
{
    CHAR = 0,
    WIDE_CHAR,

    INT8,
    UINT8,
    INT16,
    UINT16,
    INT32,
    UINT32,
    INT64,
    UINT64,

    FLOAT32,
    FLOAT64,

    OFFSET32,
    OFFSET64,

    USER_STRUCT,
    _COUNT
};

struct Str32
{
    char str[32];
    i32 len = 0;

    void set(const char* _str) {
        len = strlen(_str);
        assert(len < 32);
        memmove(str, _str, len);
        str[len] = 0;
    }
};

struct Brick
{
    struct BrickStruct* userStruct = nullptr;
    intptr_t start = 0;
    i64 size = 0;
    u32 color;
    BrickType type = BrickType::CHAR;
    Str32 name;
};

struct BrickStruct
{
    Str32 name;
    i64 _size = 0;
    u32 color;
    Array<Brick> bricks;

    inline void computeSize() {
        _size = 0;
        const i32 brickCount = bricks.count();
        for(i32 i = 0; i < brickCount; ++i) {
            _size += bricks[i].size;
        }
    }
};

struct BrickWall
{
    Array<BrickStruct> structs;
    Array<Brick> bricks;

    bool addBrick(Brick b);
    const Brick* getBrick(intptr_t offset);

    BrickStruct* addStruct(const char* name, u32 color);
};

void ui_brickPopup(const char* popupId, intptr_t selStart, i64 selLength, BrickWall* wall);
void ui_brickStructList(BrickWall* brickWall);
