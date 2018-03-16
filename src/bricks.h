#pragma once
#include "base.h"
#include "utils.h"

enum BrickType: i32
{
    BrickType_CHAR = 0,
    BrickType_WIDE_CHAR,

    BrickType_INT8,
    BrickType_UINT8,
    BrickType_INT16,
    BrickType_UINT16,
    BrickType_INT32,
    BrickType_UINT32,
    BrickType_INT64,
    BrickType_UINT64,

    BrickType_FLOAT32,
    BrickType_FLOAT64,

    BrickType_OFFSET32,
    BrickType_OFFSET64,

    BrickType_USER_STRUCT,
    BrickType__COUNT
};

struct Str32
{
    char str[32];
    i32 len = 0;

    Str32() = default;

    Str32(const char* _str) {
        set(_str);
    }

    void set(const char* _str) {
        len = strlen(_str);
        assert(len < 32);
        memmove(str, _str, len);
        str[len] = 0;
    }
};

struct Brick
{
    const struct BrickStruct* userStruct = nullptr;
    intptr_t start = 0;
    i64 size = 0;
    u32 color;
    BrickType type = BrickType_CHAR;
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

    struct TypeCache {
        Str32 name;
        i64 size;
    };
    Array<TypeCache> typeCache;

    BrickWall();
    void _rebuildTypeCache();

    bool insertBrick(Brick b);
    bool insertBrickStruct(const char* name, intptr_t where, i32 count, const BrickStruct& bstruct);
    const Brick* getBrick(intptr_t offset);

    BrickStruct* newStructDef(const char* name, u32 color);
};

void ui_brickPopup(const char* popupId, intptr_t selStart, i64 selLength, BrickWall* wall);
void ui_brickStructList(BrickWall* brickWall);
void ui_brickWall(BrickWall* brickWall);
