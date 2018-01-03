#pragma once
#include "base.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#define PANEL_MAX_COUNT 5

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

    ImRect panelRect[PANEL_MAX_COUNT];
    i32 panelType[PANEL_MAX_COUNT];
    i32 panelCount = 3;

    u8* data;
    i64 dataSize;

    const i32 columnCount = 16; // NOTE: has to be power of 2
    const i32 columnWidth = 22;
    const i32 columnHeaderHeight = 20;
    const i32 rowHeight = 22;
    const i32 rowHeaderWidth = 22;
    i32 rowCount = 100;

    const i32 scrollbarWidth = 20;

    struct ImFont* fontMono;

    DataPanels();
    void doUi(const ImRect& viewRect);

    void doHexPanel(const ImRect& panelRect, const i32 startLine);
    void doAsciiPanel(const ImRect& panelRect, const i32 startLine);
    void doIntegerPanel(const ImRect& panelRect, const i32 startLine, i32 bitSize, bool isSigned);
};
