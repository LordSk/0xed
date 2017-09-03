#pragma once
#include "base.h"
#include "nuklear.h"

typedef struct nk_rect Rect;
#define PANEL_MAX_COUNT 5

struct DataPanels
{
    enum {
        PT_HEX,
        PT_ASCII
    };

    Rect viewRect;
    Rect panelRect[PANEL_MAX_COUNT];
    i32 panelType[PANEL_MAX_COUNT];

    u8* data;
    i64 dataSize;

    const i32 columnCount = 16;
    const i32 columnWidth = 20;
    const i32 columnHeaderHeight = 20;
    const i32 rowHeight = 20;
    const i32 rowHeaderWidth = 22;
    i32 rowCount = 100;

    const i32 scrollbarWidth = 15;
    float scrollOffset = 0;
    float scrollStep = 1.0;

    void uiDataPanels(nk_context* ctx, const Rect& viewRect);
};
