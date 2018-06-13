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
    0,

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
    u32 structType;
    u32 nameHash;
    i32 line; // of declaraction;
    i32 arrayCount;
    i32 nameLen;
    char name[64];
};

struct ScriptStruct
{
    char name[64];
    u32 nameHash;
    i32 size;
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
            var->structType = 0;
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
                var->structType = typeHash;
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

bool parseStructDeclaration(Script& script, Cursor* pCursor)
{
    ScriptStruct ss;
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
    memmove(ss.name, token.start, token.len);
    ss.name[token.len] = 0;
    ss.nameHash = hash32(ss.name, token.len);

    token = getToken(pCursor);

    if(token.type != TokenType::OPEN_BRACE) {
        LOG("ParsingError> expected { after 'struct %s' (line: %d)", ss.name, pCursor->line);
        return false;
    }

    // TODO: handle empty struct (throw error)

    // parse members
    ScriptVar member;
    while(parseVariableDeclaration(&member, pCursor)) {
        Cursor curCopy = *pCursor;
        token = getToken(&curCopy);

        if(token.type == TokenType::CLOSE_BRACE) {
            *pCursor = curCopy;
            break;
        }
    }

    if(pCursor->lastToken.type != TokenType::CLOSE_BRACE) {
        LOG("ParsingError> expected } after 'struct %s {...' (line: %d)", ss.name, pCursor->line);
        return false;
    }

    g_userDefinedStructs.push(ss);

    return true;
}

bool parseExpression(Script& script, Cursor* pCursor)
{
    Cursor curCopy = *pCursor;
    Token token = getToken(&curCopy);

    if(token.type == TokenType::KW_STRUCT) {
        return parseStructDeclaration(script, pCursor);
    }
    else if(token.type == TokenType::NAME) {
        ScriptVar var;
        if(parseVariableDeclaration(&var, pCursor)) {
            // base type brick
            if(var.baseType != BaseType::_INVALID) {
                Instruction inst;
                inst.type = InstType::PLACE_BRICK;

                i32 size = BaseTypeSize[(i32)var.baseType] * var.arrayCount;
                i32 typeId = BaseTypeBrickType[(i32)var.baseType];

                inst.args[0] = size;
                inst.args[1] = script._pushBytecodeData(var.name, var.nameLen);
                inst.args[2] = var.nameLen;
                inst.args[3] = typeId;
                inst.args[4] = 0xffffffff;

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

    for(i32 i = 0; i < instCount; ++i) {
        const Instruction& inst = bytecode[i];

        switch(inst.type) {
            case InstType::PLACE_BRICK: {
                LOG("Inst> PLACE_BRICK(%d, %.*s, %d, %x)",
                        (i32)inst.args[0],
                        (i32)inst.args[2],
                        (const char*)bytecodeData + (i32)inst.args[1],
                        (i32)inst.args[3],
                        (i32)inst.args[4]);

                Brick b;
                b.start = bufferOffset;
                b.size = (i64)inst.args[0];
                b.type = (BrickType)inst.args[3];
                assert(b.type >= BrickType_CHAR && b.type < BrickType__COUNT);
                b.name.set((const char*)bytecodeData + (i32)inst.args[1], (i32)inst.args[2]);
                b.userStruct = nullptr;
                bufferOffset += b.size;

                if(!wall->insertBrick(b)) {
                    return false;
                }
            } break;
        }
    }

    return true;
}

u32 Script::_pushBytecodeData(void* data, u32 dataSize)
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
