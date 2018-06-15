#include "script.h"
#include "bricks.h"

enum class TokenType: i32 {
    _INVALID = 0,

    OPEN_PARENTHESIS,  // (
    CLOSE_PARENTHESIS, // )
    OPEN_BRACKET,      // [
    CLOSE_BRACKET,     // ]
    OPEN_BRACE,        // }
    CLOSE_BRACE,       // {
    COMMA,             // ,
    SEMICOLON,         // ;
    SLASH,             // /

    NAME,
    STRING,
    INTEGER,

    KW_STRUCT,

    END_OF_FILE
};

constexpr char* TokenTypeStr[] = {
    "_INVALID",

    "OPEN_PARENTHESIS",  // (
    "CLOSE_PARENTHESIS", // )
    "OPEN_BRACKET",      // [
    "CLOSE_BRACKET",     // ]
    "OPEN_BRACE",        // }
    "CLOSE_BRACE",       // {
    "COMMA",             // ,
    "SEMICOLON",         // ;
    "SLASH",             // /

    "NAME",
    "STRING",
    "INTEGER",

    "KW_STRUCT",

    "END_OF_FILE"
};

struct Token
{
    const char* start;
    i32 len;
    TokenType type = TokenType::_INVALID;
};

struct Cursor
{
    const char* ptr;
    i32 line = 1;
    Token lastToken;
};

inline bool strComp(const char* src, const char* cmp) {
    while(*cmp) {
        if(*src != *cmp) {
            return false;
        }
        src++;
        cmp++;
    }
    return true;
}

inline bool isWhitespace(char c)
{
    return c == ' ' || c == '\t' ||
           c == '\r' || c == '\n';
}

inline bool isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

inline bool isNumber(char c)
{
    return (c >= '0' && c <= '9');
}

inline bool isNameChar(char c)
{
    return isAlpha(c) || isNumber(c) || c == '_';
}

inline void skipComments(Cursor* pCursor)
{
    Cursor& cursor = *pCursor;

    if(cursor.ptr[0] == '/' && cursor.ptr[1] == '/') {
        cursor.ptr += 2;
        while(*cursor.ptr && *cursor.ptr != '\n') {
           cursor.ptr++;
        }
    }
}

void skipWhiteSpaceAndComments(Cursor* pCursor)
{
    Cursor& cursor = *pCursor;
    skipComments(pCursor);

    while(*cursor.ptr && isWhitespace(*cursor.ptr)) {
        if(*cursor.ptr == '\n') {
            cursor.line++;
        }
        cursor.ptr++;
        skipComments(pCursor);
    }
}

Token getToken(Cursor* pCursor)
{
    Cursor& cursor = *pCursor;
    Token token;

    skipWhiteSpaceAndComments(pCursor);

    if(!*cursor.ptr) {
        token.type = TokenType::END_OF_FILE;
        cursor.lastToken = token;
        return token;
    }

    token.start = cursor.ptr;
    token.len = 1;

    switch(cursor.ptr[0]) {
        case 0:   token.type = TokenType::END_OF_FILE; break;
        case '(': token.type = TokenType::OPEN_PARENTHESIS; break;
        case ')': token.type = TokenType::CLOSE_PARENTHESIS; break;
        case '[': token.type = TokenType::OPEN_BRACKET; break;
        case ']': token.type = TokenType::CLOSE_BRACKET; break;
        case '{': token.type = TokenType::OPEN_BRACE; break;
        case '}': token.type = TokenType::CLOSE_BRACE; break;
        case ',': token.type = TokenType::COMMA; break;
        case ';': token.type = TokenType::SEMICOLON; break;

        case 's': {
            if(strComp(cursor.ptr, "struct") && !isNameChar(cursor.ptr[6])) {
                token.type = TokenType::KW_STRUCT;
                token.len = 6;
                break;
            }
            // fall through
        }

        default: {
            if(isNumber(cursor.ptr[0])) {
                token.type = TokenType::INTEGER;
                while(isNumber(cursor.ptr[token.len])) {
                    token.len++;
                }
            }
            else if(isAlpha(cursor.ptr[0])) {
                token.type = TokenType::NAME;
                while(isNameChar(cursor.ptr[token.len])) {
                    token.len++;
                }
            }
        }
    }

    cursor.ptr += token.len;
    cursor.lastToken = token;

    return token;
}

// PARSING

enum class BaseType: i32 {
    _INVALID = 0,

    CHAR,
    WCHAR,

    INT8,
    INT16,
    INT32,
    INT64,

    UINT8,
    UINT16,
    UINT32,
    UINT64,

    FLOAT32,
    FLOAT64,

    _COUNT
};

const char* BaseTypeStr[(i32)BaseType::_COUNT] = {
    "",

    "char",
    "wchar",

    "int8",
    "int16",
    "int32",
    "int64",

    "uint8",
    "uint16",
    "uint32",
    "uint64",

    "float32",
    "float64"
};

const i32 BaseTypeSize[(i32)BaseType::_COUNT] = {
    0,

    1,
    2,

    1,
    2,
    4,
    8,

    1,
    2,
    4,
    8,

    4,
    8
};

const i32 BaseTypeBrickType[(i32)BaseType::_COUNT] = {
    -1,

    BrickType_CHAR,
    BrickType_WIDE_CHAR,

    BrickType_INT8,
    BrickType_INT16,
    BrickType_INT32,
    BrickType_INT64,

    BrickType_UINT8,
    BrickType_UINT16,
    BrickType_UINT32,
    BrickType_UINT64,

    BrickType_FLOAT32,
    BrickType_FLOAT64
};

struct ScriptVar
{
    BaseType baseType;
    u32 structTypeHash;
    u32 nameHash;
    i32 line; // of declaraction;
    i32 arrayCount;
    i32 nameLen;
    char name[64];
};

struct ScriptStruct
{
    char name[64];
    i32 nameLen;
    u32 nameHash;
    i32 size;
    i32 memberCount;
    ScriptVar members[128];
};

Array<ScriptStruct> g_userDefinedStructs;

bool parseVariableDeclaration(ScriptVar* var, Cursor* pCursor)
{
    // TODO: handle 'local' case

    *var = {};

    Token token = getToken(pCursor);

    if(token.type != TokenType::NAME) {
        LOG("ParsingError> expected variable type (line: %d)", pCursor->line);
        return false;
    }

    assert(token.len < 64);

    var->arrayCount = 1;
    var->line = pCursor->line;

    char varType[64];
    char varName[64];
    memmove(varType, token.start, token.len);
    varType[token.len] = 0;

    // validate type
    bool found = false;
    for(i32 i = 1; i < (i32)BaseType::_COUNT; ++i) {
        if(strComp(varType, BaseTypeStr[i])) {
            found = true;
            var->baseType = (BaseType)i;
            var->structTypeHash = 0;
            break;
        }
    }

    if(!found) {
        const u32 typeHash = hash32(varType, token.len);
        const i32 count = g_userDefinedStructs.count();

        for(i32 i = 0; i < count; ++i) {
            if(typeHash == g_userDefinedStructs[i].nameHash) {
                found = true;
                var->baseType = BaseType::_INVALID;
                var->structTypeHash = typeHash;
                break;
            }
        }

        if(!found) {
            LOG("ParsingError> '%s' undefined type (line: %d)", varType, pCursor->line);
        }
    }

    token = getToken(pCursor);

    i32 varArrayCount = 1;

    if(token.type == TokenType::OPEN_BRACKET) {
        token = getToken(pCursor);

        // TODO: evaluate expression (ex: header.entryCount)
        if(token.type != TokenType::INTEGER) {
            LOG("ParsingError> expected integer after '[' (line: %d)", pCursor->line);
            return false;
        }

        assert(token.len < 32);
        char intStr[32];
        memmove(intStr, token.start, token.len);
        intStr[token.len] = 0;

        token = getToken(pCursor);
        if(token.type != TokenType::CLOSE_BRACKET) {
            LOG("ParsingError> expected ']' (line: %d)", pCursor->line);
            return false;
        }

        sscanf(intStr, "%d", &varArrayCount);

        token = getToken(pCursor);
    }

    if(token.type != TokenType::NAME) {
        LOG("ParsingError> expected either '[' or a variable name after type '%s' (line: %d)",
            varType, pCursor->line);
        return false;

    }

    assert(token.len < 64);

    // variable name
    i32 varNameLen = token.len;
    memmove(varName, token.start, token.len);
    varName[token.len] = 0;

    token = getToken(pCursor);

    // TODO: parse initialization
    if(token.type != TokenType::SEMICOLON) {
        LOG("ParsingError> expected ';' to end variable declaration (%s) (line: %d)",
            varName, pCursor->line);
        return false;
    }

    LOG("%s[%d] %s;", varType, varArrayCount, varName);

    memmove(var->name, varName, varNameLen);
    var->arrayCount = varArrayCount;
    var->nameHash = hash32(varName, varNameLen);
    var->nameLen = varNameLen;

    return true;
}

bool parseStructDeclaration(ScriptStruct* ss, Cursor* pCursor)
{
    *ss = {};

    Token token = getToken(pCursor);

    if(token.type != TokenType::KW_STRUCT) {
        LOG("ParsingError> expected 'struct' (line: %d)", pCursor->line);
        return false;
    }

    token = getToken(pCursor);

    if(token.type != TokenType::NAME) {
        LOG("ParsingError> expected a name after 'struct' (line: %d)", pCursor->line);
        return false;
    }

    assert(token.len < 64);
    memmove(ss->name, token.start, token.len);
    ss->name[token.len] = 0;
    ss->nameLen = token.len;
    ss->nameHash = hash32(ss->name, token.len);

    token = getToken(pCursor);

    if(token.type != TokenType::OPEN_BRACE) {
        LOG("ParsingError> expected { after 'struct %s' (line: %d)", ss->name, pCursor->line);
        return false;
    }

    // TODO: handle empty struct (throw error)

    // parse members
    ss->memberCount = 0;
    ScriptVar member;
    while(parseVariableDeclaration(&member, pCursor)) {
        ss->members[ss->memberCount++] = member;

        Cursor curCopy = *pCursor;
        token = getToken(&curCopy);

        if(token.type == TokenType::CLOSE_BRACE) {
            *pCursor = curCopy;
            break;
        }
    }

    if(pCursor->lastToken.type != TokenType::CLOSE_BRACE) {
        LOG("ParsingError> expected } after 'struct %s {...' (line: %d)", ss->name, pCursor->line);
        return false;
    }

    return true;
}

#if 0
void pushCreateBrickStructInstructions(const ScriptStruct& ss, Script& script)
{
    Instruction inst;
    inst.type = InstType::BRICK_STRUCT_BEGIN;
    inst.args[0] = script._pushBytecodeData(ss.name, ss.nameLen);
    inst.args[1] = ss.nameLen;

    script.bytecode.push(inst);

    // TODO: place_brick on members (recursively in user structs cases)
    const i32 memberCount = ss.memberCount;
    for(i32 m = 0; m < memberCount; ++m) {
        const ScriptVar& var = ss.members[m];

        if(var.baseType != BaseType::_INVALID) {
            Instruction inst;
            inst.type = InstType::PLACE_BRICK;

            i32 size = BaseTypeSize[(i32)var.baseType] * var.arrayCount;
            i32 typeId = BaseTypeBrickType[(i32)var.baseType];

            inst.args[0] = size;
            inst.args[1] = script._pushBytecodeData(var.name, var.nameLen);
            inst.args[2] = var.nameLen;
            inst.args[3] = typeId;
            inst.args[4] = 0xff0000ff;

            script.bytecode.push(inst);
        }
        else {
            assert(var.structType);
            ScriptStruct* memSs = nullptr;

            const i32 userDefinedStructCount = g_userDefinedStructs.count();
            for(i32 s = 0; s < userDefinedStructCount; ++s) {
                if(g_userDefinedStructs[s].nameHash == var.structType) {
                    memSs = &g_userDefinedStructs[s];
                    break;
                }
            }

            assert(memSs);
            pushBrickStructAddMembersInstructions(*memSs, script);
        }
    }

    inst = {};
    inst.type = InstType::BRICK_STRUCT_END;
    script.bytecode.push(inst);
}

void pushBrickStructAddMembersInstructions(const ScriptStruct& ss, Script& script)
{
    const i32 memberCount = ss.memberCount;
    for(i32 m = 0; m < memberCount; ++m) {
        const ScriptVar& var = ss.members[m];

        if(var.baseType != BaseType::_INVALID) {
            Instruction inst;
            inst.type = InstType::PLACE_BRICK;

            i32 size = BaseTypeSize[(i32)var.baseType] * var.arrayCount;
            i32 typeId = BaseTypeBrickType[(i32)var.baseType];

            inst.args[0] = size;
            inst.args[1] = script._pushBytecodeData(var.name, var.nameLen);
            inst.args[2] = var.nameLen;
            inst.args[3] = typeId;
            inst.args[4] = 0xff0000ff;

            script.bytecode.push(inst);
        }
        else {
            assert(var.structType);
            ScriptStruct* memSs = nullptr;

            const i32 userDefinedStructCount = g_userDefinedStructs.count();
            for(i32 s = 0; s < userDefinedStructCount; ++s) {
                if(g_userDefinedStructs[s].nameHash == var.structType) {
                    memSs = &g_userDefinedStructs[s];
                    break;
                }
            }

            assert(memSs);
            pushBrickStructAddMembersInstructions(*memSs, script);
        }
    }
}
#endif
bool parseExpression(Script& script, Cursor* pCursor)
{
    Cursor curCopy = *pCursor;
    Token token = getToken(&curCopy);

    if(token.type == TokenType::KW_STRUCT) {
        ScriptStruct ss;

        if(parseStructDeclaration(&ss, pCursor)) {
            g_userDefinedStructs.push(ss);

            Instruction inst;
            inst.type = InstType::BRICK_STRUCT_BEGIN;
            inst.args[0] = script._pushBytecodeData(ss.name, ss.nameLen);
            inst.args[1] = ss.nameLen;
            inst.args[2] = 0xff000000 | (ss.nameHash & 0x00ffffff); // TODO: support user colors

            script.bytecode.push(inst);

            // TODO: members
            const i32 memberCount = ss.memberCount;
            for(i32 m = 0; m < memberCount; ++m) {
                const ScriptVar& member = ss.members[m];

                inst = {};
                inst.type = InstType::BRICK_STRUCT_ADD_MEMBER;

                inst.args[0] = script._pushBytecodeData(member.name, member.nameLen);
                inst.args[1] = member.nameLen;
                inst.args[2] = member.arrayCount;
                inst.args[3] = BaseTypeBrickType[(i32)member.baseType];
                inst.args[4] = member.structTypeHash;
                inst.args[5] = 0xff000000 | (member.nameHash & 0x00ffffff); // TODO: support user colors

                script.bytecode.push(inst);
            }

            inst = {};
            inst.type = InstType::BRICK_STRUCT_END;

            script.bytecode.push(inst);

            return true;
        }
        return false;
    }
    else if(token.type == TokenType::NAME) {
        ScriptVar var;

        if(parseVariableDeclaration(&var, pCursor)) {
            // base type brick
            if(var.baseType != BaseType::_INVALID) {
                Instruction inst;
                inst.type = InstType::PLACE_BRICK;

                i32 typeId = BaseTypeBrickType[(i32)var.baseType];

                inst.args[0] = var.arrayCount;
                inst.args[1] = script._pushBytecodeData(var.name, var.nameLen);
                inst.args[2] = var.nameLen;
                inst.args[3] = typeId;
                inst.args[4] = 0xff000000 | (var.nameHash & 0x00ffffff); // TODO: support user colors

                script.bytecode.push(inst);
            }
            else {
                assert(var.structTypeHash);

                Instruction inst;
                inst.type = InstType::PLACE_BRICK_STRUCT;

                inst.args[0] = var.arrayCount;
                inst.args[1] = script._pushBytecodeData(var.name, var.nameLen);
                inst.args[2] = var.nameLen;
                inst.args[3] = var.structTypeHash;
                inst.args[4] = 0xff000000 | (var.nameHash & 0x00ffffff); // TODO: support user colors

                script.bytecode.push(inst);
            }

            return true;
        }
        return false;
    }
    else if(token.type == TokenType::END_OF_FILE) {
        *pCursor = curCopy;
        return true;
    }

    return false;
}

void Script::release()
{
    bytecodeDataSize = 0;
    if(bytecodeData) {
        free(bytecodeData);
        bytecodeData = nullptr;
    }
}

bool Script::openAndCompile(const char* path)
{
    release();

    if(!openFileReadAll(path, &file)) {
        return false;
    }
    defer(free(file.data));

    bytecode.clear();
    bytecodeDataSize = 2048; // 2 Ko
    bytecodeData = (u8*)malloc(bytecodeDataSize);
    bytecodeDataCur = 0;

    g_userDefinedStructs.clear();

    bool parsing = true;
    Cursor cursor = { (char*)file.data };

    while(parsing) {
        if(!parseExpression(*this, &cursor)) {
            LOG("ERROR> could not parse script properly");
            parsing = false;
        }
        if(cursor.lastToken.type == TokenType::END_OF_FILE) {
            parsing = false;
        }
        /*else {
            LOG("token: type=%s, len=%d, str='%.*s'",
                TokenTypeStr[(i32)token.type], token.len, token.len, token.start);
        }*/
    }

    LOG("File parsed and compiled (%s)", path);
    return true;
}

bool Script::execute(BrickWall* wall)
{
    *wall = {}; // reset wall

    intptr_t bufferOffset = 0;
    const i32 instCount = bytecode.count();

    BrickStruct currentBrickStruct;
    bool insideStructDeclaration = false;

    for(i32 i = 0; i < instCount; ++i) {
        const Instruction& inst = bytecode[i];

        switch(inst.type) {
            case InstType::PLACE_BRICK: {
                const char* name = (const char*)bytecodeData + (i32)(i32)inst.args[1];
                const i32 nameLen = (i32)inst.args[2];
                const i32 arrayCount = inst.args[0];
                const BrickType brickType = (BrickType)inst.args[3];
                const u32 color = (u32)inst.args[4];

                LOG("Inst> PLACE_BRICK(%.*s, type=%d, arrayCount=%d, color=%x)",
                        nameLen,
                        name,
                        brickType,
                        arrayCount,
                        color);

                Brick b = makeBrickBasic(name, nameLen, brickType, arrayCount, color);
                b.start = bufferOffset;
                bufferOffset += b.size;

                if(!wall->insertBrick(b)) {
                    LOG("Script> could not insert '%.*s'", nameLen, name);
                    return false;
                }
            } break;

            case InstType::PLACE_BRICK_STRUCT: {
                const char* name = (const char*)bytecodeData + (i32)(i32)inst.args[1];
                const i32 nameLen = (i32)inst.args[2];
                const i32 arrayCount = inst.args[0];
                const u32 structHash = (u32)inst.args[3];
                const u32 color = (u32)inst.args[4];

                LOG("Inst> PLACE_BRICK_STRUCT(%.*s, arrayCount=%d, struct=%x, color=%x)",
                        nameLen,
                        name,
                        arrayCount,
                        structHash,
                        color);

                const i32 structCount = wall->structs.count();
                const BrickStruct* brickStructs = wall->structs.data();
                const BrickStruct* found = nullptr;

                for(i32 b = 0; b < structCount; ++b) {
                    if(brickStructs[b].nameHash == structHash) {
                        found = &brickStructs[b];
                        break;
                    }
                }
                assert(found); // tried to place a non existing struct
                assert(found->_size > 0); // did not finalize struct?
                bool r = wall->insertBrickStruct(name, nameLen, bufferOffset, arrayCount, *found);
                if(!r) {
                    LOG("Script> could not insert '%s %.*s'", found->name.str, nameLen, name);
                    return false;
                }
                bufferOffset += found->_size * arrayCount;

            } break;

            case InstType::BRICK_STRUCT_BEGIN: {
                assert(!insideStructDeclaration); // cannot declare structs inside of structs

                currentBrickStruct = {};
                insideStructDeclaration = true;

                const char* name = (const char*)bytecodeData + (i32)(i32)inst.args[0];
                const i32 nameLen = (i32)inst.args[1];
                const u32 color = (u32)inst.args[2];

                LOG("Inst> BRICK_STRUCT_BEGIN(%.*s, color=%x)",
                        nameLen,
                        name,
                        color);

                currentBrickStruct.name.set(name, nameLen);
                currentBrickStruct.color = color;

            } break;

            case InstType::BRICK_STRUCT_END: {
                assert(insideStructDeclaration); // END without BEGIN

                LOG("Inst> BRICK_STRUCT_END()");
                insideStructDeclaration = false;

                currentBrickStruct.finalize();
                wall->structs.push(currentBrickStruct);
                wall->_rebuildTypeCache();
            } break;

            case InstType::BRICK_STRUCT_ADD_MEMBER: {
                assert(insideStructDeclaration); // add member during struct declaration only

                const char* name = (const char*)bytecodeData + (i32)(i32)inst.args[0];
                const i32 nameLen = (i32)inst.args[1];
                const i32 arrayCount = inst.args[2];
                const BrickType brickType = (BrickType)inst.args[3];
                const u32 structHash = (u32)inst.args[4];
                const u32 color = (u32)inst.args[5];

                LOG("Inst> BRICK_STRUCT_ADD_MEMBER(%.*s, arrayCount=%d, baseType=%d, struct=%x, color=%x)",
                        nameLen,
                        name,
                        arrayCount,
                        brickType,
                        structHash,
                        color);

                Brick b;
                if(structHash == 0) {
                    b = makeBrickBasic(name, nameLen, brickType, arrayCount, color);
                }
                else {
                    const i32 structCount = wall->structs.count();
                    const BrickStruct* brickStructs = wall->structs.data();
                    const BrickStruct* found = nullptr;

                    for(i32 b = 0; b < structCount; ++b) {
                        if(brickStructs[b].nameHash == structHash) {
                            found = &brickStructs[b];
                            break;
                        }
                    }
                    assert(found); // tried to place a non existing struct
                    assert(found->_size > 0); // did not finalize struct?

                    b = makeBrickOfStruct(name, nameLen,
                                          (BrickStruct*)((intptr_t)(found - brickStructs) + 1),
                                          found->_size, arrayCount, color);
                }

                currentBrickStruct.bricks.push(b);

            } break;
        }
    }

    wall->finalize();
    return true;
}

u32 Script::_pushBytecodeData(const void* data, u32 dataSize)
{
    if(bytecodeDataCur + dataSize >= bytecodeDataSize) {
        bytecodeDataSize = max(bytecodeDataSize * 2, bytecodeDataSize + dataSize);
        bytecodeData = (u8*)realloc(bytecodeData, bytecodeDataSize);
    }

    memmove(bytecodeData + bytecodeDataCur, data, dataSize);
    u32 offset = bytecodeDataCur;
    bytecodeDataCur += dataSize;
    return offset;
}
