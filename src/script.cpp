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
    MINUS,             // -
    PLUS,              // +
    STAR,              // *
    POINT,             // .
    EQUAL,             // =
    LESS_THAN,         // <
    MORE_THAN,         // >

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
    "MINUS",             // -
    "PLUS",              // +
    "STAR",              // *
    "POINT",             // .
    "EQUAL",             // =
    "LESS_THAN",         // <
    "MORE_THAN",         // >

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
    i32 line;
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
    token.line = cursor.line;

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
        case '-': token.type = TokenType::MINUS; break;
        case '+': token.type = TokenType::PLUS; break;
        case '*': token.type = TokenType::STAR; break;
        case '.': token.type = TokenType::POINT; break;
        case '<': token.type = TokenType::LESS_THAN; break;
        case '>': token.type = TokenType::MORE_THAN; break;

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
            else {
                assert(0);
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


enum class ASTNodeType: i32 {
    _INVALID = 0,

    STRUCT_DECL,
    VAR_DECL,
    NAME,
    TYPE,
    INT_LITERAL,

    BLOCK,
    BLOCK_END,
    ARRAY_ACCESS,
    STRUCT_ACCESS,
    OP_MATH,
    SCRIPT_END,
};

// STRUCT_DECL: 1 param
// VAR_DECL: 2 params (type, var)
// STRUCT_ACCESS: 2 params (struct, memberName)
// ARRAY_ACCESS: 2 params (var, index)
// OP_MATH: 2 params (left, right)

const char* ASTNodeTypeStr[] = {
    "_INVALID",

    "StructDecl",
    "VarDecl",
    "Name",
    "Type",
    "IntLitteral",

    "Block",
    "BLOCK_END",
    "ArrayAccess",
    "StructAccess",
    "OpMath",
    "ScriptEnd",
};

struct ASTNode
{
    Token token;
    ASTNodeType type;
    ASTNode* next;
    ASTNode* param1;
    ASTNode* param2;
};

Array<ScriptStruct> g_userDefinedStructs;
List<ASTNode> g_ast;
ASTNode* g_firstNode;
ASTNode* g_scopeLast;

inline ASTNode* newNodeNext(Token token_, ASTNodeType type, ASTNode* parent)
{
    ASTNode n = { token_, type, nullptr, nullptr, nullptr };
    ASTNode* r = &(g_ast.push(n));
    if(parent) {
        parent->next = r;
    }
    return r;
}

inline ASTNode* newNode(Token token_, ASTNodeType type)
{
    ASTNode n = { token_, type, nullptr, nullptr, nullptr };
    ASTNode* r = &(g_ast.push(n));
    return r;
}

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

void printNode(ASTNode* node)
{
    LOG_NNL(KMAG "%s" KNRM "(%.*s) {", ASTNodeTypeStr[(i32)node->type], node->token.len, node->token.start);

    if(node->param1) {
        LOG_NNL(KYEL "1[" KNRM);
        printNode(node->param1);
        LOG_NNL(KYEL "] " KNRM);
    }

    if(node->param2) {
        LOG_NNL(KCYN "2[" KNRM);
        printNode(node->param2);
        LOG_NNL(KCYN "] " KNRM);
    }

    LOG_NNL("}");

    if(node->next) {
        LOG_NNL(" ->\n");
        printNode(node->next);
    }
}

ASTNode* astStructDeclaration(Token token, Cursor* pCursor);

ASTNode* astExpression(Cursor* pCursor, TokenType expectedEndType = TokenType::SEMICOLON)
{
    Token token = getToken(pCursor);
    if(token.type == TokenType::END_OF_FILE) {
        ASTNode* nodeExpr = newNodeNext(token, ASTNodeType::SCRIPT_END, nullptr);
        return nodeExpr;
    }
    if(token.type == expectedEndType) {
        LOG("ParsingError> empty expression (line: %d)", pCursor->line);
        return nullptr;
    }
    if(token.type == TokenType::KW_STRUCT) {
        return astStructDeclaration(token, pCursor);
    }

    ASTNode* firstNode = nullptr;
    ASTNode* lastNode = nullptr;

    while(token.type != expectedEndType) {
        switch(token.type) {
            case TokenType::OPEN_BRACKET: {
                ASTNode* node = newNode(token, ASTNodeType::ARRAY_ACCESS);
                if(lastNode) {
                    lastNode->next = node;
                }
                lastNode = node;

                ASTNode* expr = astExpression(pCursor, TokenType::CLOSE_BRACKET);
                if(!expr) {
                    LOG("ParsingError> error parsing expression (line: %d)", pCursor->line);
                    return nullptr;
                }
                lastNode->param2 = expr;
            } break;

            case TokenType::SEMICOLON: {
                LOG("ParsingError> unexpected ';' (line: %d)", pCursor->line);
                return nullptr;
            } break;

            case TokenType::END_OF_FILE: {
                return firstNode;
            } break;

            case TokenType::CLOSE_BRACE: {
                LOG("ParsingError> empty block or unexpected '}' (line: %d)", pCursor->line);
                return nullptr;
            } break;

            case TokenType::OPEN_BRACE: {
                ASTNode* block = newNode(token, ASTNodeType::BLOCK);
                ASTNode* firstExpr = astExpression(pCursor);
                if(!firstExpr) {
                    LOG("ParsingError> error parsing expression (line: %d)", pCursor->line);
                    return nullptr;
                }
                block->param1 = firstExpr;

                bool endBlock = false;

                Cursor copy = *pCursor;
                Token t = getToken(&copy);
                if(t.type == TokenType::CLOSE_BRACE) {
                    endBlock = true; // CLOSE_BRACE token gets "eaten" by getToken below
                    expectedEndType = TokenType::CLOSE_BRACE;
                }
                else if(t.type == TokenType::END_OF_FILE) {
                    LOG("ParsingError> block (line: %d) not closed", block->token.line);
                    return nullptr;
                }

                ASTNode* curExpr = firstExpr;
                while(!endBlock) {
                    ASTNode* newExpr = astExpression(pCursor);
                    if(!newExpr) {
                        LOG("ParsingError> error parsing expression (line: %d)", pCursor->line);
                        return nullptr;
                    }
                    curExpr->next = newExpr;
                    while(newExpr->next) newExpr = newExpr->next;
                    curExpr = newExpr;

                    Cursor copy = *pCursor;
                    Token t = getToken(&copy);
                    if(t.type == TokenType::CLOSE_BRACE) {
                        endBlock = true; // CLOSE_BRACE token gets "eaten" by getToken below
                        expectedEndType = TokenType::CLOSE_BRACE;
                    }
                    else if(t.type == TokenType::END_OF_FILE) {
                        LOG("ParsingError> block (line: %d) not closed", block->token.line);
                        return nullptr;
                    }
                }

                lastNode = block;
            } break;

            case TokenType::NAME: {
                ASTNode* node = newNode(token, ASTNodeType::NAME);
                if(lastNode) {
                    lastNode->next = node;
                }
                lastNode = node;
            } break;

            case TokenType::INTEGER: {
                ASTNode* node = newNode(token, ASTNodeType::INT_LITERAL);
                if(lastNode) {
                    lastNode->next = node;
                }
                lastNode = node;
            } break;

            case TokenType::POINT: {
                ASTNode* node = newNode(token, ASTNodeType::STRUCT_ACCESS);
                if(lastNode) {
                    lastNode->next = node;
                }
                lastNode = node;
            } break;

            case TokenType::PLUS:
            case TokenType::MINUS:
            case TokenType::STAR:
            case TokenType::SLASH: {
                ASTNode* node = newNode(token, ASTNodeType::OP_MATH);
                if(lastNode) {
                    lastNode->next = node;
                }
                lastNode = node;
            } break;

            default: {
                ASTNode* node = newNode(token, ASTNodeType::_INVALID);
                if(lastNode) {
                    lastNode->next = node;
                }
                lastNode = node;
            } break;
        }

        token = getToken(pCursor);

        if(!firstNode) {
            firstNode = lastNode;
        }
    }

    bool sorting = true;

    while(sorting) {
        sorting = false;

        // sort by precedence
        ASTNode* mpNode = nullptr, *mpPrevNode = nullptr; // max precedence node, prev...
        i32 maxPrec = 0;
        ASTNode* n = firstNode;
        ASTNode* prevNode = nullptr;

        while(n) {
            i32 nodePrecedence = -1;
            if(n->param1) {
                prevNode = n;
                n = n->next;
                continue;
            }

            switch(n->type) {
                case ASTNodeType::ARRAY_ACCESS:
                    nodePrecedence = 11;
                    break;

                case ASTNodeType::STRUCT_ACCESS:
                    nodePrecedence = 10;
                    break;
                case ASTNodeType::OP_MATH: {
                    switch(n->token.type) {
                        // multiply / divide
                        case TokenType::STAR:
                        case TokenType::SLASH:
                            nodePrecedence = 9;
                            break;

                        // plus / minus
                        case TokenType::PLUS:
                        case TokenType::MINUS:
                            nodePrecedence = 8;
                            break;
                    }
                } break;
            }

            if(nodePrecedence > maxPrec) {
                mpNode = n;
                mpPrevNode = prevNode;
                maxPrec = nodePrecedence;
            }

            prevNode = n;
            n = n->next;
        }

        if(mpNode) {
            assert(mpPrevNode);
            sorting = true;

            switch(mpNode->type) {
                case ASTNodeType::ARRAY_ACCESS: {
                    ASTNode* varArrAccess = mpPrevNode;

                    LOG("PrecedenceSort> ARRAY_ACCESS prev:[%s: '%.*s']",
                        ASTNodeTypeStr[(i32)mpPrevNode->type], mpPrevNode->token.len,
                        mpPrevNode->token.start);

                    swap(&varArrAccess->token,  &mpNode->token);
                    swap(&varArrAccess->type,   &mpNode->type);
                    swap(&varArrAccess->param1, &mpNode->param1);
                    swap(&varArrAccess->param2, &mpNode->param2);

                    varArrAccess->param1 = mpNode;
                    varArrAccess->next = mpNode->next;
                    mpNode->next = nullptr;
                } break;

                case ASTNodeType::STRUCT_ACCESS: {
                    ASTNode* varStruct = mpNode;
                    ASTNode* varMember = mpNode->next;
                    assert(varMember);

                    LOG("PrecedenceSort> STRUCT_ACCESS prev:[%s: '%.*s'] -> next:[%s: '%.*s']",
                        ASTNodeTypeStr[(i32)mpPrevNode->type], mpPrevNode->token.len, mpPrevNode->token.start,
                        ASTNodeTypeStr[(i32)varMember->type], varMember->token.len, varMember->token.start);

                    swap(&varStruct->token,  &mpPrevNode->token);
                    swap(&varStruct->type,   &mpPrevNode->type);
                    swap(&varStruct->param1, &mpPrevNode->param1);
                    swap(&varStruct->param2, &mpPrevNode->param2);

                    mpPrevNode->param1 = varStruct;
                    mpPrevNode->param2 = varMember;
                    mpPrevNode->next = varMember->next;

                    varStruct->next = nullptr;
                    varMember->next = nullptr;
                } break;

                case ASTNodeType::OP_MATH: {
                    ASTNode* varStruct = mpNode;
                    ASTNode* varMember = mpNode->next;
                    assert(varMember);

                    LOG("PrecedenceSort> OP_MATH prev:[%s: '%.*s'] -> next:[%s: '%.*s']",
                        ASTNodeTypeStr[(i32)mpPrevNode->type], mpPrevNode->token.len, mpPrevNode->token.start,
                        ASTNodeTypeStr[(i32)varMember->type], varMember->token.len, varMember->token.start);

                    swap(&varStruct->token,  &mpPrevNode->token);
                    swap(&varStruct->type,   &mpPrevNode->type);
                    swap(&varStruct->param1, &mpPrevNode->param1);
                    swap(&varStruct->param2, &mpPrevNode->param2);

                    mpPrevNode->param1 = varStruct;
                    mpPrevNode->param2 = varMember;
                    mpPrevNode->next = varMember->next;

                    varStruct->next = nullptr;
                    varMember->next = nullptr;
                } break;
            }
        }
    }

    ASTNode* secNode = firstNode->next;
    if(!secNode) {
        return firstNode;
    }

    // detect variable declarations patterns
    if((firstNode->type == ASTNodeType::NAME && secNode->type == ASTNodeType::NAME) ||
        firstNode->type == ASTNodeType::ARRAY_ACCESS && secNode->type == ASTNodeType::NAME) {
        // variable declaration
        ASTNode* varDecl = newNode(firstNode->token, ASTNodeType::VAR_DECL);
        varDecl->param1 = firstNode;
        varDecl->param2 = secNode;
        varDecl->next = secNode->next;
        firstNode->next = nullptr;
        secNode->next = nullptr;
        firstNode = varDecl;
    }

    return firstNode;
}

ASTNode* astStructDeclaration(Token structKwToken, Cursor* pCursor)
{
    ASTNode* structNode = &g_ast.push({structKwToken, ASTNodeType::STRUCT_DECL, nullptr, nullptr});

    Token token = getToken(pCursor);

    if(token.type != TokenType::NAME) {
        LOG("ParsingError> expected a name after 'struct' (line: %d)", pCursor->line);
        return nullptr;
    }

    Token structName = token;
    structNode->param1 = newNode(token, ASTNodeType::NAME);

    token = getToken(pCursor);

    if(token.type != TokenType::OPEN_BRACE) {
        LOG("ParsingError> expected { after 'struct %.*s' (line: %d)",
            structName.len, structName.start, pCursor->line);
        return nullptr;
    }

    ASTNode* memberNode = astExpression(pCursor);
    if(!memberNode) {
        LOG("ParsingError> struct %.*s declaration is empty or malformed (line: %d)",
            structName.len, structName.start, pCursor->line);
        return nullptr;
    }

    structNode->param2 = memberNode;
    while(memberNode->next) memberNode = memberNode->next;
    ASTNode* last = memberNode;

    // TODO: handle empty struct (throw error)

    // parse members
    while(memberNode = astExpression(pCursor)) {
        last->next = memberNode;
        while(memberNode->next) memberNode = memberNode->next;
        last = memberNode;

        Cursor curCopy = *pCursor;
        token = getToken(&curCopy);

        if(token.type == TokenType::CLOSE_BRACE) {
            *pCursor = curCopy;
            break;
        }
    }

    if(pCursor->lastToken.type != TokenType::CLOSE_BRACE) {
        LOG("ParsingError> expected } after 'struct %.*s {...' (line: %d)",
            structName.len, structName.start, pCursor->line);
        return nullptr;
    }

    return structNode;
}

#if 0
// make AST
bool parseExpression(Script& script, Cursor* pCursor)
{
    Array<ASTNode*> scopeStack;
    scopeStack.push(g_firstNode);
    g_scopeLast = scopeStack.last();

    bool parsing = true;

    while(parsing) {
        Token token = getToken(pCursor);

        switch(token.type) {
            case TokenType::KW_STRUCT: {
                g_scopeLast = newNodeNext(token, ASTNodeType::STRUCT, g_scopeLast);
                astStructDeclaration(pCursor);
            } break;

            case TokenType::OPEN_BRACE: {
                g_scopeLast = newNodeNext(token, ASTNodeType::BLOCK_BEGIN, g_scopeLast);
                scopeStack.push(g_scopeLast);
                g_scopeLast = scopeStack.last();
            } break;

            case TokenType::CLOSE_BRACE: {
                assert(scopeStack.count() > 1);
                g_scopeLast = newNodeNext(token, ASTNodeType::BLOCK_END, g_scopeLast);
                scopeStack.pop();
                g_scopeLast = scopeStack.last();
            } break;

            case TokenType::END_OF_FILE: {
                parsing = false;
            } break;

            default: {
                g_scopeLast->next = astExpression(pCursor, TokenType::SEMICOLON);
                g_scopeLast = g_scopeLast->next;
            } break;
        }
    }

    return true;
}
#endif

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
    g_ast.clear();
    g_firstNode = newNodeNext(Token{}, ASTNodeType::_INVALID, nullptr); // first node;
    g_scopeLast = g_firstNode;

    bool parsing = true;
    Cursor cursor = { (char*)file.data };

    while(parsing) {
        ASTNode* node;
        if(!(node = astExpression(&cursor))) {
            LOG("ERROR> could not parse script properly");
            return false;
        }
        else {
            g_scopeLast->next = node; // first
            while(node->next) {
                node = node->next;
            }
            g_scopeLast = node; // last
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

    LOG("AST ----------");
    printNode(g_firstNode->next);
    LOG("\n--------------");
    return true;
}

bool Script::execute(BrickWall* wall)
{
    return false;
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
                                          (BrickStruct*)((intptr_t)(found - brickStructs)),
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
