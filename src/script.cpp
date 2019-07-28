#include "script.h"
#include "bricks.h"
#include "imgui_extended.h"

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
    KW_IF,
    KW_ELSE,
    KW_WHILE,

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
    "KW_IF",
    "KW_ELSE",
    "KW_WHILE",

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
        case '=': token.type = TokenType::EQUAL; break;
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

        case 'i': {
            if(strComp(cursor.ptr, "if") && !isNameChar(cursor.ptr[2])) {
                token.type = TokenType::KW_IF;
                token.len = 2;
                break;
            }
            // fall through
        }

        case 'e': {
            if(strComp(cursor.ptr, "else") && !isNameChar(cursor.ptr[5])) {
                token.type = TokenType::KW_ELSE;
                token.len = 4;
                break;
            }
            // fall through
        }

        case 'w': {
            if(strComp(cursor.ptr, "while") && !isNameChar(cursor.ptr[6])) {
                token.type = TokenType::KW_WHILE;
                token.len = 5;
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

// STRUCT_DECL: 1 param
// VAR_DECL: 2 params (type, var)
// STRUCT_ACCESS: 2 params (struct, memberName)
// ARRAY_ACCESS: 2 params (var, index)
// OP_MATH: 2 params (left, right)

const char* ASTNodeTypeStr[] = {
    "_INVALID",

    "If",
    "Else",
    "While",
    "StructDecl",
    "VarDecl",
    "Name",
    "Type",
    "IntLitteral",

    "Block",
    "BLOCK_END",
    "ArrayAccess",
    "ParenthesisExpr",
    "StructAccess",
    "OpMath",
    "OpCompare",
    "OpAssign",
    "ScriptEnd",
};

struct ASTNodeType
{
    enum Enum: i32 {
        _INVALID = 0,

        IF,
        ELSE,
        WHILE,
        STRUCT_DECL,
        VAR_DECL,
        NAME,
        TYPE,
        INT_LITERAL,

        BLOCK,
        BLOCK_END,
        ARRAY_ACCESS,
        PARENTHESIS_EXPR,
        STRUCT_ACCESS,
        OP_MATH,
        OP_COMPARE,
        OP_ASSIGN,
        SCRIPT_END,
    };
};

struct ASTNode
{
    Token token;
    ASTNodeType::Enum type;
    ASTNode* next;
    ASTNode* param1;
    ASTNode* param2;
};

struct ExecStr
{
    i32 len;
    char buff[1];
};

template<typename T>
struct ExecArr
{
    i32 count;
    T data[1];
};

constexpr char* ExecNodeTypeStr[] =
{
    "_INVALID",
    "STRUCT_DECL",
    "VAR_DECL",
    "BLOCK",
    "IF_ELSE",

    "STRUCT_ACCESS",
    "ARRAY_ACCESS",

    "LITTERAL_INT",
    "LITTERAL_FLOAT",

    "OP_ADD",
    "OP_SUB",
    "OP_MUL",
    "OP_DIV",
};

static List<ASTNode> g_ast;

inline ASTNode* newNodeNext(Token token_, ASTNodeType::Enum type, ASTNode* parent)
{
    ASTNode n = { token_, type, nullptr, nullptr, nullptr };
    ASTNode* r = &(g_ast.push(n));
    if(parent) {
        parent->next = r;
    }
    return r;
}

inline ASTNode* newNode(Token token_, ASTNodeType::Enum type)
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

void printAstNode(ASTNode* node)
{
    LOG_NNL(KMAG "%s" KNRM "(%.*s)" KMAG " {" KNRM, ASTNodeTypeStr[(i32)node->type],
            node->token.len, node->token.start);

    if(node->param1) {
        LOG_NNL(KYEL "1[" KNRM);
        printAstNode(node->param1);
        LOG_NNL(KYEL "] " KNRM);
    }

    if(node->param2) {
        LOG_NNL(KCYN "2[" KNRM);
        printAstNode(node->param2);
        LOG_NNL(KCYN "] " KNRM);
    }

    LOG_NNL(KGRN "} %s" KNRM, ASTNodeTypeStr[(i32)node->type]);

    if(node->next) {
        LOG_NNL(" ->\n");
        printAstNode(node->next);
    }
}

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

                while(firstExpr->next) firstExpr = firstExpr->next;
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

                if(lastNode) {
                    lastNode->next = block;
                }
                lastNode = block;
            } break;

            case TokenType::OPEN_PARENTHESIS: {
                ASTNode* paren = newNode(token, ASTNodeType::PARENTHESIS_EXPR);
                if(lastNode) {
                    lastNode->next = paren;
                }
                lastNode = paren;

                ASTNode* newExpr = astExpression(pCursor, TokenType::CLOSE_PARENTHESIS);
                if(!newExpr) {
                    LOG("ParsingError> error parsing expression (line: %d)", pCursor->line);
                    return nullptr;
                }

                paren->param1 = newExpr;
            } break;

            case TokenType::KW_STRUCT: {
                ASTNode* structNode = newNode(token, ASTNodeType::STRUCT_DECL);

                Cursor copy = *pCursor;
                Token tokenName = getToken(&copy);
                if(tokenName.type != TokenType::NAME) {
                    LOG("ParsingError> expected name after 'struct' (line: %d)", pCursor->line);
                    return nullptr;
                }

                *pCursor = copy;
                ASTNode* name = newNode(tokenName, ASTNodeType::NAME);
                structNode->param1 = name;

                if(lastNode) {
                    lastNode->next = structNode;
                }
                lastNode = structNode;
            } break;

            case TokenType::KW_IF: {
                ASTNode* node = newNode(token, ASTNodeType::IF);
                if(lastNode) {
                    lastNode->next = node;
                }
                lastNode = node;
            } break;

            case TokenType::KW_ELSE: {
                ASTNode* node = newNode(token, ASTNodeType::ELSE);
                if(lastNode) {
                    lastNode->next = node;
                }
                lastNode = node;
            } break;

            case TokenType::KW_WHILE: {
                ASTNode* node = newNode(token, ASTNodeType::WHILE);
                if(lastNode) {
                    lastNode->next = node;
                }
                lastNode = node;
            } break;

            case TokenType::EQUAL: {
                Cursor copy = *pCursor;
                Token t = getToken(&copy);
                if(t.type == TokenType::EQUAL) {
                    *pCursor = copy;

                    ASTNode* node = newNode(token, ASTNodeType::OP_COMPARE);
                    if(lastNode) {
                        lastNode->next = node;
                    }
                    lastNode = node;
                }
                else {
                    ASTNode* node = newNode(token, ASTNodeType::OP_ASSIGN);
                    if(lastNode) {
                        lastNode->next = node;
                    }
                    lastNode = node;
                }
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
                case ASTNodeType::STRUCT_ACCESS:
                    nodePrecedence = 12;
                    break;

                case ASTNodeType::ARRAY_ACCESS:
                    nodePrecedence = 11;
                    break;

                case ASTNodeType::OP_ASSIGN: {
                    nodePrecedence = 10;
                } break;

                case ASTNodeType::OP_COMPARE: {
                    nodePrecedence = 9;
                } break;

                case ASTNodeType::OP_MATH: {
                    switch(n->token.type) {
                        // multiply / divide
                        case TokenType::STAR:
                        case TokenType::SLASH:
                            nodePrecedence = 8;
                            break;

                        // plus / minus
                        case TokenType::PLUS:
                        case TokenType::MINUS:
                            nodePrecedence = 7;
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

                case ASTNodeType::OP_ASSIGN: {
                    ASTNode* leftExpr = mpNode;
                    ASTNode* rightExpr = mpNode->next;
                    assert(rightExpr);

                    LOG("PrecedenceSort> OP_ASSIGN prev:[%s: '%.*s'] -> next:[%s: '%.*s']",
                        ASTNodeTypeStr[(i32)mpPrevNode->type], mpPrevNode->token.len, mpPrevNode->token.start,
                        ASTNodeTypeStr[(i32)rightExpr->type], rightExpr->token.len, rightExpr->token.start);

                    swap(&leftExpr->token,  &mpPrevNode->token);
                    swap(&leftExpr->type,   &mpPrevNode->type);
                    swap(&leftExpr->param1, &mpPrevNode->param1);
                    swap(&leftExpr->param2, &mpPrevNode->param2);

                    mpPrevNode->param1 = leftExpr;
                    mpPrevNode->param2 = rightExpr;
                    mpPrevNode->next = rightExpr->next;

                    leftExpr->next = nullptr;
                    rightExpr->next = nullptr;
                } break;

                case ASTNodeType::OP_COMPARE: {
                    ASTNode* leftExpr = mpNode;
                    ASTNode* rightExpr = mpNode->next;
                    assert(rightExpr);

                    LOG("PrecedenceSort> OP_COMPARE prev:[%s: '%.*s'] -> next:[%s: '%.*s']",
                        ASTNodeTypeStr[(i32)mpPrevNode->type], mpPrevNode->token.len, mpPrevNode->token.start,
                        ASTNodeTypeStr[(i32)rightExpr->type], rightExpr->token.len, rightExpr->token.start);

                    swap(&leftExpr->token,  &mpPrevNode->token);
                    swap(&leftExpr->type,   &mpPrevNode->type);
                    swap(&leftExpr->param1, &mpPrevNode->param1);
                    swap(&leftExpr->param2, &mpPrevNode->param2);

                    mpPrevNode->param1 = leftExpr;
                    mpPrevNode->param2 = rightExpr;
                    mpPrevNode->next = rightExpr->next;

                    leftExpr->next = nullptr;
                    rightExpr->next = nullptr;
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

    return firstNode;
}

void printExecNode(ExecNode* node)
{
    LOG_NNL(KMAG "%s {" KNRM, ExecNodeTypeStr[(i32)node->type]);

    switch(node->type) {
        case ExecNodeType::BLOCK: {
            ExecNode* expr = (ExecNode*)node->params[0]; // first expression
            printExecNode(expr);
        } break;

        case ExecNodeType::VAR_DECL: {
            const ExecStr& type = *(ExecStr*)node->params[0];
            ExecNode* arrayCountexpr = (ExecNode*)node->params[1];
            const ExecStr& name = *(ExecStr*)node->params[2];

            LOG_NNL("%.*s", type.len, type.buff);

            if(arrayCountexpr) {
                LOG_NNL("[");
                printExecNode(arrayCountexpr);
                LOG_NNL("]");
            }

            LOG_NNL(" %.*s", name.len, name.buff);
        } break;

        case ExecNodeType::STRUCT_DECL: {
            const ExecStr& name = *(ExecStr*)node->params[0];
            LOG_NNL("\n%.*s ", name.len, name.buff);
            printExecNode((ExecNode*)node->params[1]); // block
        } break;

        case ExecNodeType::STRUCT_ACCESS: {
            const ExecStr& var = *(ExecStr*)node->params[0];
            const ExecStr& member = *(ExecStr*)node->params[1];
            LOG_NNL("%.*s.%.*s", var.len, var.buff, member.len, member.buff);
        } break;

        case ExecNodeType::OP_ADD:
        case ExecNodeType::OP_SUB:
        case ExecNodeType::OP_MUL:
        case ExecNodeType::OP_DIV:{
            ExecNode* left = (ExecNode*)node->params[0];
            ExecNode* right = (ExecNode*)node->params[1];
            LOG_NNL("(");
            printExecNode(left);
            LOG_NNL(") " KCYN);
            switch(node->type) {
                case ExecNodeType::OP_ADD: LOG_NNL("+"); break;
                case ExecNodeType::OP_SUB: LOG_NNL("-"); break;
                case ExecNodeType::OP_MUL: LOG_NNL("*"); break;
                case ExecNodeType::OP_DIV: LOG_NNL("/"); break;
            }
            LOG_NNL(KNRM " (");
            printExecNode(right);
            LOG_NNL(")");
        } break;

        case ExecNodeType::LITTERAL_INT: {
            const i64 i = (i64)(intptr_t)node->params[0];
            LOG_NNL("%lld", i);
        } break;
    }

    LOG_NNL(KGRN "} %s" KNRM, ExecNodeTypeStr[(i32)node->type]);

    if(node->next) {
        LOG_NNL(" ->\n");
        printExecNode(node->next);
    }
}

ExecNode *Script::_execVarDecl(ASTNode* astNode, ASTNode** nextAstNode)
{
    ASTNode* nextAst = astNode->next;

    if(nextAst &&
       ((astNode->type == ASTNodeType::NAME && nextAst->type == ASTNodeType::NAME) ||
        astNode->type == ASTNodeType::ARRAY_ACCESS && nextAst->type == ASTNodeType::NAME)) {
        Token tkType;
        Token tkName = nextAst->token;

        if(astNode->type == ASTNodeType::ARRAY_ACCESS) {
            assert(astNode->param1);
            assert(astNode->param1->type == ASTNodeType::NAME);
            assert(astNode->param2);
            tkType = astNode->param1->token;
        }
        else {
            tkType = astNode->token;
        }

        // variable declaration
        ExecNode varDecl;
        varDecl.type = ExecNodeType::VAR_DECL;
        // type
        varDecl.params[0] = (void*)(intptr_t)_pushExecDataStr(tkType.len, tkType.start);
        // array count
        if(astNode->param2) {
            ASTNode* useless;
            ExecNode* arrayCountExpr = _execExpression(astNode->param2, &useless);
            if(!arrayCountExpr) {
                return nullptr;
            }
            if(!arrayCountExpr->returnsInt()) {
                LOG("ERROR> [expression] must evaluate to integer");
                return nullptr;
            }
            varDecl.params[1] = arrayCountExpr;
        }
        else {
            varDecl.params[1] = nullptr;
        }

        // name
        varDecl.params[2] = (void*)(intptr_t)_pushExecDataStr(tkName.len, tkName.start);

        *nextAstNode = nextAst->next;
        return _pushExecNode(varDecl);
    }

    return nullptr;
}

ExecNode *Script::_execStructDecl(ASTNode* astNode, ASTNode** nextAstNode)
{
    ASTNode* nextAst = astNode->next;

    if(nextAst &&
        astNode->type == ASTNodeType::STRUCT_DECL &&
        nextAst->type == ASTNodeType::BLOCK) {
        assert(astNode->param1);
        assert(astNode->param1->type == ASTNodeType::NAME);
        Token tkName = astNode->param1->token;

        ExecNode* block = _execBlock(nextAst, nextAstNode);
        if(!block) {
            LOG("ERROR> could not parse block (line: %d)", nextAst->token.line);
            return nullptr;
        }

        // struct declaration
        ExecNode structDecl;
        structDecl.type = ExecNodeType::STRUCT_DECL;
        // name
        structDecl.params[0] = (void*)(intptr_t)_pushExecDataStr(tkName.len, tkName.start);
        // members block
        structDecl.params[1] = block;

        *nextAstNode = nextAst->next;
        return _pushExecNode(structDecl);
    }

    return nullptr;
}

ExecNode *Script::_execBlock(ASTNode* astNode, ASTNode** nextAstNode)
{
    if(astNode->type != ASTNodeType::BLOCK) {
        return nullptr;
    }

    ExecNode block;
    block.type = ExecNodeType::BLOCK;
    assert(astNode->param1);
    ASTNode* astBlock = astNode;
    astNode = astNode->param1;

    ASTNode* thisAstNext;
    ExecNode* newExpr = _execExpression(astNode, &thisAstNext);
    if(!newExpr) {
        LOG("ERROR> can't parse expression (line: %d)", astNode->token.line);
        return nullptr;
    }

    block.params[0] = newExpr;

    ExecNode* curExpr = newExpr;
    while(thisAstNext) {
        astNode = thisAstNext;
        newExpr = _execExpression(astNode, &thisAstNext);
        if(!newExpr) {
            LOG("ERROR> can't parse expression (line: %d)", astNode->token.line);
            return nullptr;
        }
        curExpr->next = newExpr;
        curExpr = newExpr;
    }

    *nextAstNode = astBlock->next;
    return _pushExecNode(block);
}

ExecNode *Script::_execStructAccess(ASTNode* astNode, ASTNode** nextAstNode)
{
    if(astNode->type != ASTNodeType::STRUCT_ACCESS) {
        return nullptr;
    }

    Token tkVar = astNode->param1->token;
    Token tkMember = astNode->param2->token;

    ExecNode execSa;
    execSa.type = ExecNodeType::STRUCT_ACCESS;
    // struct variable name
    execSa.params[0] = (void*)(intptr_t)_pushExecDataStr(tkVar.len, tkVar.start);
    // member name
    execSa.params[1] = (void*)(intptr_t)_pushExecDataStr(tkMember.len, tkMember.start);

    *nextAstNode = astNode->next;
    return _pushExecNode(execSa);
}

ExecNode *Script::_execOpMath(ASTNode* astNode, ASTNode** nextAstNode)
{
    if(astNode->type != ASTNodeType::OP_MATH) {
        return nullptr;
    }

    Token tkOp = astNode->token;

    ExecNode execOp;
    switch(tkOp.start[0]) {
        case '+': execOp.type = ExecNodeType::OP_ADD; break;
        case '-': execOp.type = ExecNodeType::OP_SUB; break;
        case '*': execOp.type = ExecNodeType::OP_MUL; break;
        case '/': execOp.type = ExecNodeType::OP_DIV; break;
        default: assert(0); break;
    }

    ASTNode* useless;
    ExecNode* leftExpr = _execExpression(astNode->param1, &useless);
    ExecNode* rightExpr = _execExpression(astNode->param2, &useless);

    if(!leftExpr || (!leftExpr->returnsInt() && !leftExpr->returnsFloat())) {
        LOG("ERROR> %s: left expression must evaluate to int or float (line: %d)",
            ExecNodeTypeStr[(i32)execOp.type], tkOp.line);
        return nullptr;
    }
    if(!rightExpr || (!rightExpr->returnsInt() && !rightExpr->returnsFloat())) {
        LOG("ERROR> %s: right expression must evaluate to int or float (line: %d)",
            ExecNodeTypeStr[(i32)execOp.type], tkOp.line);
        return nullptr;
    }

    // left
    execOp.params[0] = leftExpr;
    // right
    execOp.params[1] = rightExpr;

    *nextAstNode = astNode->next;
    return _pushExecNode(execOp);
}

ExecNode*Script::_execLitteral(ASTNode* astNode, ASTNode** nextAstNode)
{
    if(astNode->type != ASTNodeType::INT_LITERAL) {
        return nullptr;
    }

    Token tkInt = astNode->token;
    i64 count;
    sscanf(tkInt.start, "%lld", &count);

    ExecNode litteralInt;
    litteralInt.type = ExecNodeType::LITTERAL_INT;
    litteralInt.returnType = BaseType::INT64;
    litteralInt.params[0] = (void*)(intptr_t)count;

    *nextAstNode = astNode->next;
    return _pushExecNode(litteralInt);
}

ExecNode *Script::_execExpression(ASTNode* astNode, ASTNode** nextAstNode)
{
    ExecNode* varDecl = _execVarDecl(astNode, nextAstNode);
    if(varDecl) {
        return varDecl;
    }

    ExecNode* structDecl = _execStructDecl(astNode, nextAstNode);
    if(structDecl) {
        return structDecl;
    }

    ExecNode* block = _execBlock(astNode, nextAstNode);
    if(block) {
        return block;
    }

    ExecNode* structAccess = _execStructAccess(astNode, nextAstNode);
    if(structAccess) {
        return structAccess;
    }

    ExecNode* opMath = _execOpMath(astNode, nextAstNode);
    if(opMath) {
        return opMath;
    }

    ExecNode* litt = _execLitteral(astNode, nextAstNode);
    if(litt) {
        return litt;
    }

    return nullptr;
}

void Script::release()
{
    execDataSize = 0;
    if(execData) {
        free(execData);
        execData = nullptr;
    }
}

bool Script::openAndCompile(const char* path)
{
    release();

    if(!openFileReadAll(path, &file)) {
        return false;
    }
	// TODO: restore file buffer free (ast display needs it for now)
	//defer(free(file.data));

    execDataSize = 2048; // 2 Ko
    execData = (u8*)malloc(execDataSize);
    execDataCur = 0;

    g_ast.clear();
	pFirstNode = newNodeNext(Token{}, ASTNodeType::_INVALID, nullptr); // first node;
	ASTNode* scopeLast = pFirstNode;

    bool parsing = true;
    Cursor cursor = { (char*)file.data };

    while(parsing) {
        ASTNode* node;
        if(!(node = astExpression(&cursor))) {
            LOG("ERROR> could not parse script properly");
            return false;
        }
        else {
			scopeLast->next = node; // first
            while(node->next) {
                node = node->next;
            }
			scopeLast = node; // last
        }
        if(cursor.lastToken.type == TokenType::END_OF_FILE) {
            parsing = false;
        }
    }

    LOG("File parsed successfully (%s)", path);

	pFirstNode = pFirstNode->next;

    LOG("AST ----------");
	printAstNode(pFirstNode);
    LOG("\n--------------");


	/*
    execNodeList.clear();
    execTree = _pushExecNode(ExecNode{ ExecNodeType::_INVALID }); // first node
    ExecNode* lastExecNode = execTree;

	ASTNode* curAst = pFirstNode;
    while(curAst && curAst->type != ASTNodeType::SCRIPT_END) {
        ASTNode* nextAst;
        ExecNode* expr = _execExpression(curAst, &nextAst);
        if(!expr) {
            LOG("ERROR> can't parse expression (line: %d)", curAst->token.line);
            return nullptr;
        }

        lastExecNode->next = expr;
        lastExecNode = lastExecNode->next;
        curAst = nextAst;
    }

    // fix exec data pointers
    const i32 bucketCount = execNodeList.buckets.count();
    List<ExecNode>::Bucket* buckets = execNodeList.buckets.data();

    for(i32 b = 0; b < bucketCount; b++) {
        const i32 itemCount = buckets[b].count;
        ExecNode* items = buckets[b].buffer;

        for(i32 i = 0; i < itemCount; i++) {
            ExecNode& n = items[i];
            switch(n.type) {
                case ExecNodeType::VAR_DECL: {
                    n.params[0] = execData + (intptr_t)n.params[0]; // type
                    n.params[2] = execData + (intptr_t)n.params[2]; // name
                } break;

                case ExecNodeType::STRUCT_DECL: {
                    n.params[0] = execData + (intptr_t)n.params[0]; // name
                } break;

                case ExecNodeType::STRUCT_ACCESS: {
                    n.params[0] = execData + (intptr_t)n.params[0]; // var
                    n.params[1] = execData + (intptr_t)n.params[1]; // member
                } break;
            }
        }
    }

    LOG("File compiled successfully (%s)", path);

    LOG("Execution tree -");
    printExecNode(execTree->next);
    LOG("\n--------------");
*/
    return true;
}

bool Script::execute(BrickWall* wall)
{
    return false;
    *wall = {}; // reset wall



    wall->finalize();
    return true;
}

u32 Script::_pushExecData(const void* data, u32 dataSize)
{
    if(execDataCur + dataSize >= execDataSize) {
        execDataSize = MAX(execDataSize * 2, execDataSize + dataSize);
        execData = (u8*)realloc(execData, execDataSize);
    }

    memmove(execData + execDataCur, data, dataSize);
    u32 offset = execDataCur;
    execDataCur += dataSize;
    return offset;
}

u32 Script::_pushExecDataStr(const i32 len, const char* str)
{
    // ExecStr{ i32 len; char buff[] }
    u32 off = _pushExecData(&len, sizeof(len));
    _pushExecData(str, len);
    return off;
}

ExecNode* Script::_pushExecNode(ExecNode node)
{
    return &execNodeList.push(node);
}

void* ExecNode::execute()
{
    switch(type) {
        case ExecNodeType::STRUCT_DECL: {
            const ExecStr& name = *(ExecStr*)params[0];
            ExecArr<ExecNode>& memberNodeArray = *(ExecArr<ExecNode>*)params[1];
            LOG("struct %.*s {", name.len, name.buff);

            const i32 count = memberNodeArray.count;
            for(i32 i = 0; i < count; i++) {
                memberNodeArray.data[i].execute();
            }

            LOG("}");
        } break;

        case ExecNodeType::VAR_DECL: {
            const ExecStr& type = *(ExecStr*)params[0];
            const i32 arrayCount = (i32)(intptr_t)params[1];
            const ExecStr& name = *(ExecStr*)params[2];
            LOG("%.*s[%d] %.*s", type.len, type.buff, arrayCount, name.len, name.buff);
        } break;
    }

    return nullptr;
}

void printAstNodeUi(ASTNode* node)
{
	if(ImGui::TreeNode(node, "%s '%.*s'", ASTNodeTypeStr[(i32)node->type], node->token.len, node->token.start)) {

		if(node->param1) {
			printAstNodeUi(node->param1);
		}

		if(node->param2) {
			printAstNodeUi(node->param2);
		}

		ImGui::TreePop();
	}

	if(node->next) {
		printAstNodeUi(node->next);
	}
}

void scriptPrintAstAsUi(const Script &script)
{
	printAstNodeUi(script.pFirstNode);
}
