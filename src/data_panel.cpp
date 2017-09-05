#include "data_panel.h"

static void uiDrawHexByte(nk_context* ctx, u8 val, const struct nk_rect& rect, const struct nk_color& color)
{
    constexpr char base16[] = "0123456789ABCDEF";
    char hexStr[3];
    hexStr[0] = base16[val >> 4];
    hexStr[1] = base16[val & 15];
    hexStr[2] = 0;

    nk_text_colored_rect(ctx, hexStr, 2, NK_TEXT_CENTERED, rect, color);
}

void DataPanels::uiDataPanels(nk_context* ctx, const Rect& viewRect)
{
    rowCount = dataSize / columnCount;

    Rect dataRect = nk_rect(rowHeaderWidth,
                                            viewRect.y + columnHeaderHeight,
                                            columnWidth * columnCount,
                                            viewRect.h - columnHeaderHeight);
    const i32 visibleRowCount = dataRect.h / rowHeight + 1;


    Rect scrollbarRect = nk_rect(viewRect.x + viewRect.w - scrollbarWidth - 1,
                                 viewRect.y,
                                 scrollbarWidth,
                                 viewRect.h);


    if(nk_begin(ctx, "data_panels", viewRect, NK_WINDOW_NO_SCROLLBAR)) {

    // scrollbar
    scrollOffset = nk_scrollbarv(ctx, scrollOffset, scrollStep, rowCount, scrollbarRect);
    const i64 dataIdOff = scrollOffset * columnCount;

    struct nk_command_buffer* canvas = nk_window_get_canvas(ctx);

    nk_fill_rect(canvas, nk_rect(viewRect.x, viewRect.y,
                 rowHeaderWidth + columnWidth * columnCount,
                 columnHeaderHeight), 0, nk_rgb(240, 240, 240));
    nk_fill_rect(canvas, nk_rect(viewRect.x, viewRect.y, rowHeaderWidth, viewRect.h),
                 0, nk_rgb(240, 240, 240));

    // column header horizontal line
    nk_stroke_line(canvas, 0, dataRect.y,
                   dataRect.x + dataRect.w, dataRect.y,
                   1.0, nk_rgb(200, 200, 200));

    // vertical end line
    nk_stroke_line(canvas, dataRect.x + dataRect.w,
                   viewRect.y, dataRect.x + dataRect.w,
                   viewRect.y + viewRect.h, 1.0, nk_rgb(200, 200, 200));

    // horizontal lines
    for(i32 r = 1; r < visibleRowCount; ++r) {
        nk_stroke_line(canvas, dataRect.x, dataRect.y + r * rowHeight,
                       dataRect.x + dataRect.w, dataRect.y + r * rowHeight,
                       1.0, nk_rgb(200, 200, 200));
    }

    // vertical lines
    for(i32 c = 0; c < columnCount; c += 4) {
        nk_stroke_line(canvas, dataRect.x + c * columnWidth,
                       dataRect.y, dataRect.x + c * columnWidth,
                       dataRect.y + dataRect.h, 1.0, nk_rgb(200, 200, 200));
    }

    // line number
    for(i32 r = 0; r < visibleRowCount; ++r) {
        uiDrawHexByte(ctx, r + (i32)scrollOffset,
           nk_rect(0, viewRect.y + columnHeaderHeight + r * rowHeight, rowHeaderWidth, rowHeight),
           nk_rgb(150, 150, 150));
    }

    // column number
    for(i32 c = 0; c < columnCount; ++c) {
        uiDrawHexByte(ctx, c,
           nk_rect(c * columnWidth + rowHeaderWidth, viewRect.y, columnWidth, columnHeaderHeight),
           nk_rgb(150, 150, 150));
    }

    // data text
    for(i32 r = 0; r < visibleRowCount; ++r) {
        for(i32 c = 0; c < columnCount; ++c) {
            uiDrawHexByte(ctx, data[dataIdOff + r * columnCount + c],
               nk_rect(c * columnWidth + rowHeaderWidth, r * rowHeight + columnHeaderHeight + viewRect.y,
                       columnWidth, rowHeight),
               nk_rgb(0, 0, 0));
        }
    }

    } nk_end(ctx);
}
