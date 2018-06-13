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

    inline const Brick* getBrick(intptr_t offset) const {
        assert(_size);
        offset = offset % _size;
        const i32 count = bricks.count();
        i64 cur = 0;
        for(i32 i = 0; i < count; ++i) {
            const Brick& b1 = bricks[i];
            cur += b1.size;
            if(offset < cur) {
                return &b1;
            }
        }
        return nullptr;
    }
};

struct BrickWall
{
    Array<BrickStruct> structs;
    Array<Brick> bricks;

    struct TypeInfo {
        Str32 name;
        i64 size;
    };
    Array<TypeInfo> typeCache;

    BrickWall();
    void _rebuildTypeCache();

    bool insertBrick(Brick b);
    bool insertBrickStruct(const char* name, intptr_t where, i32 count, const BrickStruct& bstruct);
    const Brick* getBrick(intptr_t offset);

    BrickStruct* newStructDef(const char* name, u32 color);
};

void ui_brickPopup(const char* popupId, intptr_t selStart, i64 selLength, BrickWall* wall);
void ui_brickStructList(BrickWall* brickWall);
void ui_brickWall(BrickWall* brickWall, const u8* fileData);
