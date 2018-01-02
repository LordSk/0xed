#include "data_panel.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

namespace ImGui {

static void DoScrollbarVertical(i64* outScrollVal, i64 scrollPageSize, i64 scrollTotalSize)
{
    if(scrollPageSize >= scrollTotalSize) {
        return;
    }

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID("#SCROLLY");
    i64& scrollVal = *outScrollVal;

    ImRect winRect = window->Rect();
    winRect.Expand(ImVec2(0, -2.f)); // padding
    window->Size.x -= style.ScrollbarSize; // this feels weird

    // Render background
    ImRect bb = winRect;
    bb.Min.x = bb.Max.x - style.ScrollbarSize;
    ImRect bbBg = bb;

    int window_rounding_corners;
    window_rounding_corners = ImDrawCornerFlags_TopRight|ImDrawCornerFlags_BotRight;
    window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_ScrollbarBg),
                                    window->WindowRounding, 0);

    f32 height = winRect.GetHeight() * ((f64)scrollPageSize / scrollTotalSize);
    f32 scrollSurfaceSizeY = winRect.GetHeight();

    constexpr f32 minGrabHeight = 24.f;
    if(height < minGrabHeight) {
        scrollSurfaceSizeY -= minGrabHeight - height;
        height = minGrabHeight;
    }

    //LOG("height=%.5f pageSize=%llu contentSize=%llu", height, pageSize, contentSize);
    f32 yOffset = scrollSurfaceSizeY * ((f64)scrollVal / scrollTotalSize);
    bb = ImRect(bbBg.Min.x + 2.0f, bbBg.Min.y + yOffset, bbBg.Max.x - 2.0f, bbBg.Min.y + yOffset + height);
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
        my -= bbBg.Min.y;
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

static void TextBox(const char* label, const ImVec2& boxSize, const ImVec4 bgColor = ImVec4(1,1,1,1),
                    const ImVec4 textColor = ImVec4(0,0,0,1))
{
    ImGuiWindow* window = GetCurrentWindow();
    if(window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = CalcItemSize(boxSize, label_size.x, label_size.y);

    const ImRect bb(pos, pos + size);
    ItemSize(bb, 0);
    if (!ItemAdd(bb, id))
        return;

    // Render
    const ImU32 col = ColorConvertFloat4ToU32(bgColor);
    RenderFrame(bb.Min, bb.Max, col, false, 0);

    PushStyleColor(ImGuiCol_Text, textColor);
    RenderTextClipped(bb.Min, bb.Max, label, NULL,
                      &label_size, style.ButtonTextAlign, &bb);
    PopStyleColor();
}

}

DataPanels::DataPanels()
{
    memset(panelType, 0, sizeof(panelType));
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

    static i64 scrollCurrent = 0;
    ImGui::DoScrollbarVertical(&scrollCurrent,
                               (viewRect.h/rowHeight), // page size (in lines)
                               dataSize/columnCount + 2); // total lines + 2 (for last line visibility)

    const f32 panelWidth = ImGui::GetWindowWidth() / panelCount;
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

        f32 bgColor = val/255.f * 0.5 + 0.5;
        f32 textColor = 0.0f;
        ImGui::TextBox((const char*)&hex, ImVec2(columnWidth, rowHeight),
                       ImVec4(bgColor, bgColor, bgColor, 1),
                       ImVec4(textColor, textColor, textColor, 1));

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
    const i32 intColumnWidth = 34;


    // TODO: round off item count based of byteSize
    const i64 dataIdOff = (i64)startLine * columnCount;
    for(i64 i = 0; i < itemCount; i += byteSize) {
        char integerStr[32];

        // TODO: this is VERY innefficient, find a better way to do this
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


        ImGui::TextBox(integerStr, ImVec2(intColumnWidth * byteSize, rowHeight));

        if((i+byteSize) & (columnCount-1)) ImGui::SameLine();
    }
}
