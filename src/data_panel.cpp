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
    if(scrollPageSize >= scrollTotalSize) {
        return;
    }

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
    f32 scrollSurfaceSizeY = locRect.h;

    constexpr f32 minGrabHeight = 24.f;
    if(height < minGrabHeight) {
        scrollSurfaceSizeY -= minGrabHeight - height;
        height = minGrabHeight;
    }

    //LOG("height=%.5f pageSize=%llu contentSize=%llu", height, pageSize, contentSize);
    f32 yOffset = scrollSurfaceSizeY * ((f64)scrollVal / scrollTotalSize);
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
        scrollVal = (my/scrollSurfaceSizeY) * (f64)scrollTotalSize;

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
        "Int8",
        "Uint8",
        "Int16",
        "Uint16",
        "Int32",
        "Uint32",
        "Int64",
        "Uint64",
    };

    /*if(ImGui::BeginCombo()) {

        ImGui::EndCombo();
    }*/

    // TODO: do not hard code y-offset (22), calculate it
    static i64 scrollCurrent = 0;
    DoScrollbar(Rect{viewRect.w - style.ScrollbarSize, 22, style.ScrollbarSize, viewRect.h - 22},
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
        ImGui::BeginChild("##ChildPanel", ImVec2(panelWidth, -1), false,
                          ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse);

        switch(panelType[p]) {
            case PT_HEX:
                doHexPanel(Rect{0, 0, panelWidth, ImGui::GetWindowHeight()}, scrollCurrent);
                break;
            case PT_ASCII:
                doAsciiPanel(Rect{0, 0, panelWidth, ImGui::GetWindowHeight()}, scrollCurrent);
                break;
            case PT_INT8:
                doIntegerPanel(Rect{0, 0, panelWidth, ImGui::GetWindowHeight()}, scrollCurrent, 8, true);
                break;
            case PT_UINT8:
                doIntegerPanel(Rect{0, 0, panelWidth, ImGui::GetWindowHeight()}, scrollCurrent, 8, false);
                break;
            case PT_INT16:
                doIntegerPanel(Rect{0, 0, panelWidth, ImGui::GetWindowHeight()}, scrollCurrent, 16, true);
                break;
            case PT_UINT16:
                doIntegerPanel(Rect{0, 0, panelWidth, ImGui::GetWindowHeight()}, scrollCurrent, 16, false);
                break;
            case PT_INT32:
                doIntegerPanel(Rect{0, 0, panelWidth, ImGui::GetWindowHeight()}, scrollCurrent, 32, true);
                break;
            case PT_UINT32:
                doIntegerPanel(Rect{0, 0, panelWidth, ImGui::GetWindowHeight()}, scrollCurrent, 32, false);
                break;
            case PT_INT64:
                doIntegerPanel(Rect{0, 0, panelWidth, ImGui::GetWindowHeight()}, scrollCurrent, 64, true);
                break;
            case PT_UINT64:
                doIntegerPanel(Rect{0, 0, panelWidth, ImGui::GetWindowHeight()}, scrollCurrent, 64, false);
                break;
        }


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

void DataPanels::doAsciiPanel(const Rect& panelRect, const i32 startLine)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    const ImVec2 winPos = window->DC.CursorPos;
    const ImGuiStyle& style = ImGui::GetStyle();

    const i32 lineCount = panelRect.h / rowHeight + 1;
    const i32 itemCount = min(dataSize - (i64)startLine * columnCount, lineCount * columnCount);
    const f32 charWidth = 14;

    ImGui::RenderFrame(winPos,
                       ImVec2(winPos.x + columnCount * charWidth, winPos.y + lineCount * rowHeight),
                       ImGui::GetColorU32(ImGuiCol_FrameBg), false);

    ImGui::PushFont(fontMono);


    const i64 dataIdOff = (i64)startLine * columnCount;
    for(i64 i = 0; i < itemCount; ++i) {
        char c = (char)data[i + dataIdOff];
        if(c < 0x20) {
            // TODO: grey out instead of space
            c = ' ';
        }

        i32 line = i / columnCount;
        i32 column = i & (columnCount - 1);
        ImRect bb(winPos.x + column * charWidth, winPos.y + line * rowHeight,
                  winPos.x + column * charWidth + charWidth,
                  winPos.y + line * rowHeight + rowHeight);

        ImGui::RenderTextClipped(bb.Min,
                                 bb.Max,
                                 &c, (const char*)&c + 1, NULL,
                                 ImVec2(0.5, 0.5), &bb);


        if((i+1) & (columnCount-1)) ImGui::SameLine();
    }

    ImGui::PopFont();
}

void DataPanels::doIntegerPanel(const Rect& panelRect, const i32 startLine, i32 bitSize, bool isSigned)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    const ImVec2 winPos = window->DC.CursorPos;
    const ImGuiStyle& style = ImGui::GetStyle();

    const i32 lineCount = panelRect.h / rowHeight + 1;
    const i32 itemCount = min(dataSize - (i64)startLine * columnCount, lineCount * columnCount);
    const i32 byteSize = bitSize >> 3; // div 8
    const i32 intColumnWidth = 30;


    // TODO: round off item count based of byteSize
    const i64 dataIdOff = (i64)startLine * columnCount;
    for(i64 i = 0; i < itemCount; i += byteSize) {
        char integerStr[32];

        if(isSigned) {
            switch(bitSize) {
                case 8:
                    sprintf(integerStr, "%d", *(i8*)&data[i + dataIdOff]);
                    break;
                case 16:
                    sprintf(integerStr, "%d", *(i16*)&data[i + dataIdOff]);
                    break;
                case 32:
                    sprintf(integerStr, "%d", *(i32*)&data[i + dataIdOff]);
                    break;
                case 64:
                    sprintf(integerStr, "%lld", *(i64*)&data[i + dataIdOff]);
                    break;
                default:
                    assert(0);
                    break;
            }
        }
        else {
            switch(bitSize) {
                case 8:
                    sprintf(integerStr, "%u", *(u8*)&data[i + dataIdOff]);
                    break;
                case 16:
                    sprintf(integerStr, "%u", *(u16*)&data[i + dataIdOff]);
                    break;
                case 32:
                    sprintf(integerStr, "%u", *(u32*)&data[i + dataIdOff]);
                    break;
                case 64:
                    sprintf(integerStr, "%llu", *(u64*)&data[i + dataIdOff]);
                    break;
                default:
                    assert(0);
                    break;
            }
        }


        ImGui::Button(integerStr, ImVec2(intColumnWidth * byteSize, rowHeight));

        if((i+byteSize) & (columnCount-1)) ImGui::SameLine();
    }
}
