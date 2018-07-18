#pragma once

#include "utils.h"

struct ExecNodeType
{
    enum Enum {
        _INVALID = 0,
        STRUCT_DECL,
        VAR_DECL,
        BLOCK,
        IF_ELSE,

        STRUCT_ACCESS,
        ARRAY_ACCESS,

        LITTERAL_INT,
        LITTERAL_FLOAT,

        OP_ADD,
        OP_SUB,
        OP_MUL,
        OP_DIV,
    };
};

struct BaseType
{
    enum Enum {
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
};

struct ExecNode
{
    ExecNodeType::Enum type;
    BaseType::Enum returnType = BaseType::INT32;
    void* params[5]; // FIXME: only works with x64 (void* is 8 bytes)
    ExecNode* next = nullptr;

    void* execute();

    inline bool isValid() const { return type != ExecNodeType::_INVALID; }
    inline bool returnsInt() const {
        return (returnType >= BaseType::INT8 && returnType <= BaseType::INT64) ||
               (returnType >= BaseType::UINT8 && returnType <= BaseType::UINT64);
    }
    inline bool returnsFloat() const {
        return returnType == BaseType::FLOAT32 || returnType == BaseType::FLOAT64;
    }
};

#define ExecNode_INVALID ExecNode{ ExecNodeType::_INVALID }

struct ASTNode;

struct Script
{
    FileBuffer file;
    u8* execData = nullptr;
    u32 execDataCur = 0;
    u32 execDataSize = 0;
    List<ExecNode> execNodeList;
    ExecNode* execTree = nullptr;

    ~Script() { release(); }
    void release();

    bool openAndCompile(const char* path);
    bool execute(struct BrickWall* wall);

    u32 _pushExecData(const void* data, u32 dataSize);
    u32 _pushExecDataStr(const i32 len, const char* str);
    ExecNode* _pushExecNode(ExecNode node);

    ExecNode* _execVarDecl(ASTNode* astNode, ASTNode** nextAstNode);
    ExecNode* _execStructDecl(ASTNode* astNode, ASTNode** nextAstNode);
    ExecNode* _execBlock(ASTNode* astNode, ASTNode** nextAstNode);
    ExecNode* _execStructAccess(ASTNode* astNode, ASTNode** nextAstNode);
    ExecNode* _execOpMath(ASTNode* astNode, ASTNode** nextAstNode);
    ExecNode* _execLitteral(ASTNode* astNode, ASTNode** nextAstNode);
    ExecNode* _execExpression(ASTNode* astNode, ASTNode** nextAstNode);
};
