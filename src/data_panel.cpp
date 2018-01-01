#include "data_panel.h"
#include "imgui.h"
#include "imgui_internal.h"

static void uiDrawHexByte(u8 val)
{
    constexpr char base16[] = "0123456789ABCDEF";
    char hexStr[3];
    hexStr[0] = base16[val >> 4];
    hexStr[1] = base16[val & 15];
    hexStr[2] = 0;

    ImGui::Text(hexStr);
}

DataPanels::DataPanels()
{
    memset(panelType, 0, sizeof(panelType));
}

static void DoScrollbar(const Rect& rect, i64* outScrollVal, i64 scrollPageSize, i64 scrollTotalSize)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    Rect locRect = rect;
    locRect.x += window->Pos.x;
    locRect.y += window->Pos.y;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID("#SCROLLY");
    i64& scrollVal = *outScrollVal;

    // Render background
    ImRect bb = ImRect(locRect.x, locRect.y, locRect.x + locRect.w, locRect.y + locRect.h);

    int window_rounding_corners;
    window_rounding_corners = ImDrawCornerFlags_TopRight|ImDrawCornerFlags_BotRight;
    window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_ScrollbarBg),
                                    window->WindowRounding, 0);

    f32 height = locRect.h * ((f64)scrollPageSize / scrollTotalSize);
    //LOG("height=%.5f pageSize=%llu contentSize=%llu", height, pageSize, contentSize);
    f32 yOffset = locRect.h * ((f64)scrollVal / scrollTotalSize);
    bb = ImRect(locRect.x, locRect.y + yOffset, locRect.x + locRect.w, locRect.y + yOffset + height);
    bb.Min.x += 2.0f;
    bb.Max.x -= 2.0f;

    bool held = false;
    bool hovered = false;
    const bool previously_held = (g.ActiveId == id);
    ImGui::ButtonBehavior(bb, id, &hovered, &held);

    static f32 mouseGrabDeltaY = 0;

    if(held) {
        if(!previously_held) {
            f32 my = g.IO.MousePos.y;
            my -= bb.Min.y;
            mouseGrabDeltaY = my;
        }

        f32 my = g.IO.MousePos.y;
        my -= locRect.y;
        my -= mouseGrabDeltaY;
        scrollVal = (my/locRect.h) * (f64)scrollTotalSize;

        // clamp
        if(scrollVal < 0) {
            scrollVal = 0;
        }
        else if(scrollVal > (scrollTotalSize - scrollPageSize)) {
            scrollVal = scrollTotalSize - scrollPageSize;
        }
    }
    else {
        // TODO: find a better place for this?
        ImGuiWindow* hovered = g.HoveredWindow;
        while(hovered && hovered != window) {
            hovered = hovered->ParentWindow;
        }
        if(hovered && g.IO.MouseWheel != 0.0f) {
            scrollVal -= g.IO.MouseWheel;

            // clamp
            if(scrollVal < 0) {
                scrollVal = 0;
            }
            else if(scrollVal > (scrollTotalSize - scrollPageSize)) {
                scrollVal = scrollTotalSize - scrollPageSize;
            }
        }
    }

    const ImU32 grab_col = ImGui::GetColorU32(held ? ImGuiCol_ScrollbarGrabActive : hovered ?
                                              ImGuiCol_ScrollbarGrabHovered : ImGuiCol_ScrollbarGrab);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, grab_col, style.ScrollbarRounding);
}

void DataPanels::doUi(const Rect& viewRect)
{
    const ImGuiStyle& style = ImGui::GetStyle();

    rowCount = dataSize / columnCount;

    Rect combosRect =  viewRect;
    combosRect.h = 30;

    Rect panelsRect =  viewRect;

    const char* panelComboItems[] = {
        "Hex",
        "ASCII",
    };

    /*if(ImGui::BeginCombo()) {

        ImGui::EndCombo();
    }*/

    static i64 scrollCurrent = 0;
    DoScrollbar(Rect{viewRect.w - style.ScrollbarSize, 0, style.ScrollbarSize, viewRect.h},
                &scrollCurrent,
                (viewRect.h/rowHeight), // page size (in lines)
                dataSize/columnCount + 2); // total lines + 2 (for last line visibility)

    panelsRect.w -= style.ScrollbarSize;

    const f32 panelWidth = panelsRect.w / panelCount;
    ImGui::PushItemWidth(panelWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    for(i32 p = 0; p < panelCount; ++p) {
        ImGui::PushID(&panelType[p]);
        ImGui::Combo("##PanelType", &panelType[p], panelComboItems, IM_ARRAYSIZE(panelComboItems));
        ImGui::PopID();

        if(p+1 < panelCount) ImGui::SameLine();
    }

    ImGui::PopStyleVar(1);
    ImGui::PopItemWidth();

    ImGui::PushItemWidth(panelWidth);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    for(i32 p = 0; p < panelCount; ++p) {
        ImGui::PushID(&panelRect[p]);
        ImGui::BeginChild("##ChildPanel", ImVec2(panelWidth, -1), true,
                          ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse|
                          ImGuiWindowFlags_NoBringToFrontOnFocus);

        doHexPanel(Rect{0, 0, panelWidth, ImGui::GetWindowHeight()}, scrollCurrent);

        ImGui::EndChild();
        ImGui::PopID();

        if(p+1 < panelCount) ImGui::SameLine();
    }

    ImGui::PopStyleVar(1);
    ImGui::PopItemWidth();
}

void DataPanels::doHexPanel(const Rect& panelRect, const i32 startLine)
{
    const i32 lineCount = panelRect.h / rowHeight + 1;
    const i32 itemCount = min(dataSize - (i64)startLine * columnCount, lineCount * columnCount);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    const i64 dataIdOff = (i64)startLine * columnCount;
    for(i64 i = 0; i < itemCount; ++i) {
        u8 val = data[i + dataIdOff];

        // https://johnnylee-sde.github.io/Fast-unsigned-integer-to-hex-string/
        u32 hex = (val >> 4) << 8 | (val & 15);
        u32 mask = ((hex + 0x0606) >> 4) & 0x0101;
        hex |= 0x3030;
        hex += 0x07 * mask;

        ImGui::Button((const char*)&hex, ImVec2(columnWidth, rowHeight));

        if((i+1) & (columnCount-1)) ImGui::SameLine();
    }

    ImGui::PopStyleVar(1);

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
