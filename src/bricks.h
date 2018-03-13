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

    _COUNT
};

struct Brick
{
    BrickType type = BrickType::CHAR;
    intptr_t start = 0;
    i64 length = 0;
    u32 color;
};

struct BrickWall
{
    Array<Brick> bricks;

    bool addBrick(Brick b);
    const Brick* getBrick(intptr_t offset);
};

void ui_brickPopup(const char* popupId, intptr_t selStart, i64 selLength, BrickWall* wall);
