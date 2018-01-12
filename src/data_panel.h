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
    ImRect lockedPanelRect;
};

enum PanelTypes {
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

    PT_FLOAT32,
    PT_FLOAT64,
};

struct DataPanels
{
    f32 panelRectWidth[PANEL_MAX_COUNT];
    i32 panelType[PANEL_MAX_COUNT];
    i32 panelCount = 2;

    u8* fileBuffer;
    i64 fileBufferSize;
    i64 scrollCurrent = 0;

    const i32 columnCount = 16; // NOTE: has to be power of 2
    const i32 columnWidth = 22;
    const i32 rowHeight = 22;
    const i32 columnHeaderHeight = 24;
    const i32 rowHeaderWidth = 32;
    const i32 panelSpacing = 10;
    const f32 panelCloseButtonWidth = 20;

    const i32 asciiCharWidth = 14;
    const i32 intColumnWidth = 34;

    const ImU32 hoverFrameColor = 0xffff9c4c;
    const ImU32 selectedFrameColor = 0xffff7200;
    const ImU32 selectedTextColor = 0xffffffff;

    struct ImFont* fontMono;

    SelectionState selectionState;

    ImGuiWindow* mainWindow;

    void processMouseInput(ImRect winRect);
    inline void selectionProcessMouseInput(const i32 panelId, ImVec2 mousePos, ImRect rect,
                               const i32 columnWidth_, const i32 rowHeight_,
                               const i32 startLine, const i32 hoverLen);
    inline bool selectionInHoverRange(i64 dataOffset);
    inline bool selectionInSelectionRange(i64 dataOffset);

    DataPanels();
    void setFileBuffer(u8* buff, i64 buffSize);
    void addNewPanel();

    void calculatePanelWidth();
    void doRowHeader(const ImRect& winRect);

    void doUi();

    void doHexPanel(const char* label, const i32 startLine);
    void doAsciiPanel(const char* label, const i32 startLine);

    template<typename T>
    void doFormatPanel(const char* label, const i32 startLine, const char* format);
};
