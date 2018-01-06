#pragma once
#include "base.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#define PANEL_MAX_COUNT 5

struct SelectionState
{
    i64 hoverStart = -1;
    i64 hoverEnd = -1;
    i64 selectStart = -1;
    i64 selectEnd = -1;
    i32 lockedPanelId = -1;
};

struct DataPanels
{
    enum {
        PT_HEX = 0,
        PT_ASCII,

        PT_INT8,
        PT_UINT8,
        PT_INT16,
        PT_UINT16,
        PT_INT32,
        PT_UINT32,
        PT_INT64,
        PT_UINT64,
    };

    f32 panelRectWidth[PANEL_MAX_COUNT];
    i32 panelType[PANEL_MAX_COUNT];
    i32 panelCount = 3;

    u8* data;
    i64 dataSize;
    i64 scrollCurrent = 0;

    const i32 columnCount = 16; // NOTE: has to be power of 2
    const i32 columnWidth = 22;
    const i32 rowHeight = 22;
    const i32 columnHeaderHeight = 24;
    const i32 rowHeaderWidth = 32;
    i32 rowCount = 100;

    const i32 asciiCharWidth = 14;
    const i32 intColumnWidth = 34;

    const ImU32 hoverFrameColor = 0xffff9c4c;

    struct ImFont* fontMono;

    // TODO: do hover logic all in the same place?
    SelectionState selectionState;
    inline void sel_hoverLogic(ImVec2 relMousePos, const ImRect& rect, i32 columnWidth_, i32 rowHeight_,
                               i32 startLine, i32 hoverLen);
    inline bool sel_inHoverRange(i64 dataOffset);

    i32 inspectorWidth = 400;

    DataPanels();
    void doUi(const ImRect& viewRect);

    void doHexPanel(const ImRect& panelRectWidth, const i32 startLine);
    void doAsciiPanel(const ImRect& panelRectWidth, const i32 startLine);
    void doIntegerPanel(const ImRect& panelRectWidth, const i32 startLine, i32 bitSize, bool isSigned);
};
