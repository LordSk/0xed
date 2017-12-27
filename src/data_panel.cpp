#include "data_panel.h"

static void uiDrawHexByte(u8 val, const struct nk_rect& rect, const struct nk_color& color)
{
#if 0
    constexpr char base16[] = "0123456789ABCDEF";
    char hexStr[3];
    hexStr[0] = base16[val >> 4];
    hexStr[1] = base16[val & 15];
    hexStr[2] = 0;

    nk_text_colored_rect(ctx, hexStr, 2, NK_TEXT_CENTERED, rect, color);
#endif
}

void DataPanels::doUi(const Rect& viewRect)
{
#if 0
    rowCount = dataSize / columnCount;

    Rect combosRect =  viewRect;
    combosRect.h = 30;

    Rect panelsRect =  viewRect;

    const char* panelComboItems[] = {
        "Hex",
        "ASCII",
    };

    if(nk_begin(ctx, "panel_combos", combosRect, NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_begin(ctx, NK_STATIC, combosRect.h, panelCount);

        for(i32 p = 0; p < panelCount; ++p) {
            nk_layout_row_push(ctx, 150);

            nk_style_push_color(ctx, &ctx->style.window.border_color, nk_rgb(0, 120, 215));

            if(nk_combo_begin_label(ctx, "Hex", nk_vec2(150,200))) {
                nk_layout_row_dynamic(ctx, 22, 1);

                nk_button_label(ctx, panelComboItems[0]);
                nk_button_label(ctx, panelComboItems[1]);

                nk_combo_end(ctx);
            }

            nk_style_pop_color(ctx); // window.border_color
        }

        nk_layout_row_end(ctx);

        Rect r = nk_window_get_bounds(ctx);
        panelsRect.y += r.h;
        panelsRect.h -= r.h;
    }
    nk_end(ctx);


    Rect scrollbarRect = nk_rect(panelsRect.x + panelsRect.w - scrollbarWidth - 1,
                                 panelsRect.y,
                                 scrollbarWidth,
                                 panelsRect.h);

    if(nk_begin(ctx, "data_panels", panelsRect, NK_WINDOW_NO_SCROLLBAR)) {
        // scrollbar
        scrollOffset = nk_scrollbarv(ctx, scrollOffset, scrollStep, rowCount, scrollbarRect);
        const i64 dataIdOff = scrollOffset * columnCount;

        Rect hexPanelRect = nk_rect(panelsRect.x,
                                    panelsRect.y,
                                    columnWidth * columnCount + rowHeaderWidth + 1,
                                    panelsRect.h);
        doHexPanel(hexPanelRect, dataIdOff);

        hexPanelRect.x += hexPanelRect.w;
        doHexPanel(hexPanelRect, dataIdOff);
        hexPanelRect.x += hexPanelRect.w;
        doHexPanel(hexPanelRect, dataIdOff);
        hexPanelRect.x += hexPanelRect.w;
        doHexPanel(hexPanelRect, dataIdOff);
    }
    nk_end(ctx);
#endif
}

void DataPanels::doHexPanel(const Rect& panelRect, const i64 dataIdOff)
{
#if 0
    Rect dataRect = nk_rect(panelRect.x + rowHeaderWidth,
                            panelRect.y + columnHeaderHeight,
                            columnWidth * columnCount,
                            panelRect.h - columnHeaderHeight);

    const i32 visibleRowCount = panelRect.h / rowHeight + 1;
    struct nk_command_buffer* canvas = nk_window_get_canvas(ctx);

    nk_fill_rect(canvas, nk_rect(panelRect.x, panelRect.y,
                 panelRect.w, columnHeaderHeight), 0, nk_rgb(240, 240, 240));
    nk_fill_rect(canvas, nk_rect(panelRect.x, panelRect.y, rowHeaderWidth, panelRect.h),
                 0, nk_rgb(240, 240, 240));

    // column header horizontal line
    nk_stroke_line(canvas, panelRect.x, dataRect.y,
                   panelRect.x + panelRect.w, dataRect.y,
                   1.0, nk_rgb(200, 200, 200));

    // vertical end line
    nk_stroke_line(canvas, panelRect.x + panelRect.w - 1,
                   panelRect.y, panelRect.x + panelRect.w - 1,
                   panelRect.y + panelRect.h, 1.0, nk_rgb(200, 200, 200));

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
           nk_rect(panelRect.x, panelRect.y + columnHeaderHeight + r * rowHeight,
                   rowHeaderWidth, rowHeight),
           nk_rgb(150, 150, 150));
    }

    // column number
    for(i32 c = 0; c < columnCount; ++c) {
        uiDrawHexByte(ctx, c,
           nk_rect(panelRect.x + c * columnWidth + rowHeaderWidth,
                   panelRect.y, columnWidth, columnHeaderHeight),
           nk_rgb(150, 150, 150));
    }

    // data text
    for(i32 r = 0; r < visibleRowCount; ++r) {
        for(i32 c = 0; c < columnCount; ++c) {
            uiDrawHexByte(ctx, data[dataIdOff + r * columnCount + c],
               nk_rect(dataRect.x + c * columnWidth,
                       dataRect.y + r * rowHeight,
                       columnWidth, rowHeight),
               nk_rgb(0, 0, 0));
        }
    }
#endif
}
