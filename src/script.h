#pragma once

#include "utils.h"

struct ExecNodeType
{
    enum Enum {
        _INVALID = 0,
        STRUCT_DECL,
        VAR_DECL,
        BLOCK
    };
};

struct ExecNode
{
    ExecNodeType::Enum type;
    void* params[5];
    ExecNode* next = nullptr;

    void* execute();

    inline bool isValid() const { return type != ExecNodeType::_INVALID; }
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
    ExecNode* _execExpression(ASTNode* astNode, ASTNode** nextAstNode);
};
