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

const char* BaseTypeStr[] = {
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

struct ScriptVar
{
    BaseType baseType;
    u32 structType;
    u32 nameHash;
    i32 line; // of declaraction;
    char name[64];
};

struct ScriptStruct
{
    char name[64];
    u32 nameHash;
    ScriptVar members[128];
};

Array<ScriptStruct> g_userDefinedStructs;

bool parseVariableDeclaration(Script& script, Cursor* pCursor)
{
    Token token = getToken(pCursor);

    if(token.type != TokenType::NAME) {
        LOG("ParsingError> expected variable type (line: %d)", pCursor->line);
        return false;
    }

    char varType[128];
    char varName[128];
    memmove(varType, token.start, token.len);
    varType[token.len] = 0;

    // validate type
    bool found = false;
    for(i32 i = 1; i < (i32)BaseType::_COUNT; ++i) {
        if(strComp(varType, BaseTypeStr[i])) {
            found = true;
            break;
        }
    }

    if(!found) {
        const u32 typeHash = hash32(varType, token.len);
        const i32 count = g_userDefinedStructs.count();

        for(i32 i = 0; i < count; ++i) {
            if(typeHash == g_userDefinedStructs[i].nameHash) {
                found = true;
                break;
            }
        }

        if(!found) {
            LOG("ParsingError> '%s' undefined type (line: %d)", varType, pCursor->line);
        }
    }

    token = getToken(pCursor);

    if(token.type == TokenType::OPEN_BRACKET) {
        token = getToken(pCursor);

        // TODO: evaluate expression (ex: header.entryCount)
        if(token.type != TokenType::INTEGER) {
            LOG("ParsingError> expected integer after '[' (line: %d)", pCursor->line);
            return false;
        }

        token = getToken(pCursor);
        if(token.type != TokenType::CLOSE_BRACKET) {
            LOG("ParsingError> expected ']' (line: %d)", pCursor->line);
            return false;
        }

        token = getToken(pCursor);
    }

    if(token.type != TokenType::NAME) {
        LOG("ParsingError> expected either '[' or a variable name after type '%s' (line: %d)",
            varType, pCursor->line);
        return false;

    }

    // variable name
    memmove(varName, token.start, token.len);
    varName[token.len] = 0;

    token = getToken(pCursor);

    // TODO: parse initialization
    if(token.type != TokenType::SEMICOLON) {
        LOG("ParsingError> expected ';' to end variable declaration (%s) (line: %d)",
            varName, pCursor->line);
        return false;
    }

    //LOG("%s %s;", varType, varName);

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
    while(parseVariableDeclaration(script, pCursor)) {
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
        return parseVariableDeclaration(script, pCursor);
    }
    else if(token.type == TokenType::END_OF_FILE) {
        *pCursor = curCopy;
        return true;
    }

    return false;
}

bool Script::openAndCompile(const char* path)
{
    if(!openFileReadAll(path, &file)) {
        return false;
    }
    defer(free(file.data));

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
