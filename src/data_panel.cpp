#include "data_panel.h"

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
    window->ContentsRegionRect.Max.x -= style.ScrollbarSize;

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

// https://johnnylee-sde.github.io/Fast-unsigned-integer-to-hex-string/
inline u32 toHexStr(u8 val)
{
    u32 hex = (val & 15) << 8 | (val >> 4);
    u32 mask = ((hex + 0x0606) >> 4) & 0x0101;
    hex |= 0x3030;
    hex += 0x07 * mask;
    return hex;
}

DataPanels::DataPanels()
{
    memset(panelType, 0, sizeof(panelType));
}

void DataPanels::doUi(const ImRect& viewRect)
{
    // TODO: remove this limitation
    assert(columnCount <= 16);

    for(i32 p = 0; p < panelCount; ++p) {
        switch(panelType[p]) {
            case PT_HEX:
                panelRectWidth[p] = columnCount * columnWidth;
                break;
            case PT_ASCII:
                panelRectWidth[p] = columnCount * asciiCharWidth;
                break;
            case PT_INT8:
            case PT_UINT8:
            case PT_INT16:
            case PT_UINT16:
            case PT_INT32:
            case PT_UINT32:
            case PT_INT64:
            case PT_UINT64:
                panelRectWidth[p] = intColumnWidth * columnCount;
                break;
        }
    }

    const ImGuiStyle& style = ImGui::GetStyle();
    rowCount = dataSize / columnCount;

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

    static i64 scrollCurrent = 0;
    ImGui::DoScrollbarVertical(&scrollCurrent,
                               (viewRect.GetHeight()/rowHeight), // page size (in lines)
                               dataSize/columnCount + 2); // total lines + 2 (for last line visibility)

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    // row header
    ImGui::BeginChild("##ROWHEADER", ImVec2(rowHeaderWidth, -1), false,
                      ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::EndChild();
    ImGui::SameLine();

    // panels
    for(i32 p = 0; p < panelCount; ++p) {
        const f32 panelWidth = panelRectWidth[p];

        ImGui::PushID(&panelRectWidth[p]);
        ImGui::BeginChild("##ChildPanel", ImVec2(panelWidth, -1), false,
                          ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse);

        ImGui::PushItemWidth(panelWidth);
        ImGui::PushID(&panelType[p]);
        ImGui::Combo("##PanelType", &panelType[p], panelComboItems, IM_ARRAYSIZE(panelComboItems),
                     IM_ARRAYSIZE(panelComboItems));
        ImGui::PopID();
        ImGui::PopItemWidth();

        ImRect panelRect_ = ImGui::GetCurrentWindow()->Rect();

        switch(panelType[p]) {
            case PT_HEX:
                doHexPanel(panelRect_, scrollCurrent);
                break;
            case PT_ASCII:
                doAsciiPanel(panelRect_, scrollCurrent);
                break;
            case PT_INT8:
                doIntegerPanel(panelRect_, scrollCurrent, 8, true);
                break;
            case PT_UINT8:
                doIntegerPanel(panelRect_, scrollCurrent, 8, false);
                break;
            case PT_INT16:
                doIntegerPanel(panelRect_, scrollCurrent, 16, true);
                break;
            case PT_UINT16:
                doIntegerPanel(panelRect_, scrollCurrent, 16, false);
                break;
            case PT_INT32:
                doIntegerPanel(panelRect_, scrollCurrent, 32, true);
                break;
            case PT_UINT32:
                doIntegerPanel(panelRect_, scrollCurrent, 32, false);
                break;
            case PT_INT64:
                doIntegerPanel(panelRect_, scrollCurrent, 64, true);
                break;
            case PT_UINT64:
                doIntegerPanel(panelRect_, scrollCurrent, 64, false);
                break;
        }


        ImGui::EndChild();
        ImGui::PopID();

        if(p+1 < panelCount) ImGui::SameLine();
    }

    ImGui::PopStyleVar(1);
    //ImGui::PopItemWidth();
}

void DataPanels::doHexPanel(const ImRect& panelRect, const i32 startLine)
{
    const i32 lineCount = panelRect.GetHeight() / rowHeight + 1;
    const i32 itemCount = min(dataSize - (i64)startLine * columnCount, lineCount * columnCount);

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if(window->SkipItems)
        return;

    ImVec2 winPos = window->DC.CursorPos;
    const ImGuiID id = window->GetID("#HEXPANEL");
    ImGui::ItemSize(panelRect, 0);
    if(!ImGui::ItemAdd(panelRect, id))
        return;

    const ImVec2 cellSize(columnWidth, rowHeight);

    // column header
    ImRect colHeadBb(winPos, winPos + ImVec2(panelRect.GetWidth(), columnHeaderHeight));
    const ImU32 headerColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.9, 0.9, 0.9, 1));
    ImGui::RenderFrame(colHeadBb.Min, colHeadBb.Max, headerColor, false, 0);

    for(i64 i = 0; i < columnCount; ++i) {
        u32 hex = toHexStr(i);
        const char* label = (const char*)&hex;
        const ImVec2 label_size = ImGui::CalcTextSize(label, label+2);
        const ImVec2 size = ImGui::CalcItemSize(cellSize, label_size.x, label_size.y);

        ImVec2 cellPos(i * cellSize.x, columnHeaderHeight - cellSize.y);
        cellPos += winPos;
        const ImRect bb(cellPos, cellPos + size);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5, 0.5, 0.5, 1));
        ImGui::RenderTextClipped(bb.Min, bb.Max, label, label+2,
                          &label_size, ImVec2(0.5, 0.5), &bb);
        ImGui::PopStyleColor();
    }
    winPos.y += columnHeaderHeight;

    // hex table
    const i64 dataIdOff = (i64)startLine * columnCount;
    for(i64 i = 0; i < itemCount; ++i) {
        u8 val = data[i + dataIdOff];
        u32 hex = toHexStr(val);

        f32 bgColor = val/255.f * 0.7 + 0.3;
        constexpr f32 textColor = 0.0f;

        const char* label = (const char*)&hex;
        const ImVec2 label_size = ImGui::CalcTextSize(label, label+2);
        const ImVec2 size = ImGui::CalcItemSize(cellSize, label_size.x, label_size.y);

        i32 column = i & (columnCount - 1);
        i32 line = i / columnCount;
        ImVec2 cellPos(column * cellSize.x, line * cellSize.y);
        cellPos += winPos;
        const ImRect bb(cellPos, cellPos + size);

        const ImU32 frameColor = ImGui::ColorConvertFloat4ToU32(ImVec4(bgColor, bgColor, bgColor, 1));
        ImGui::RenderFrame(bb.Min, bb.Max, frameColor, false, 0);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(textColor, textColor, textColor, 1));
        ImGui::RenderTextClipped(bb.Min, bb.Max, label, label+2,
                          &label_size, ImVec2(0.5, 0.5), &bb);
        ImGui::PopStyleColor();
    }
}

void DataPanels::doAsciiPanel(const ImRect& panelRect, const i32 startLine)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImVec2 winPos = window->DC.CursorPos;
    const ImGuiStyle& style = ImGui::GetStyle();

    // column header
    ImRect colHeadBb(winPos, winPos + ImVec2(panelRect.GetWidth(), columnHeaderHeight));
    const ImU32 headerColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 1));
    ImGui::RenderFrame(colHeadBb.Min, colHeadBb.Max, headerColor, false, 0);

    for(i64 i = 0; i < columnCount; i+=4) {
        u32 hex = toHexStr(i);
        const char* label = (const char*)&hex;
        const ImVec2 label_size = ImGui::CalcTextSize(label, label+2);
        const ImVec2 size = ImGui::CalcItemSize(ImVec2(asciiCharWidth, rowHeight),
                                                label_size.x, label_size.y);

        ImVec2 cellPos(i * asciiCharWidth, columnHeaderHeight - rowHeight);
        cellPos += winPos;
        const ImRect bb(cellPos, cellPos + size);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7, 0.7, 0.7, 1));
        ImGui::RenderTextClipped(bb.Min, bb.Max, label, label+2,
                          &label_size, ImVec2(0.5, 0.5), &bb);
        ImGui::PopStyleColor();
    }
    winPos.y += columnHeaderHeight;

    const i32 lineCount = panelRect.GetHeight() / rowHeight + 1;
    const i32 itemCount = min(dataSize - (i64)startLine * columnCount, lineCount * columnCount);

    ImGui::RenderFrame(winPos,
                       ImVec2(winPos.x + columnCount * asciiCharWidth, winPos.y + lineCount * rowHeight),
                       ImGui::GetColorU32(ImGuiCol_FrameBg), false);

    ImGui::PushFont(fontMono);


    const i64 dataIdOff = (i64)startLine * columnCount;
    for(i64 i = 0; i < itemCount; ++i) {
        const char c = (char)data[i + dataIdOff];
        i32 line = i / columnCount;
        i32 column = i & (columnCount - 1);
        ImRect bb(winPos.x + column * asciiCharWidth, winPos.y + line * rowHeight,
                  winPos.x + column * asciiCharWidth + asciiCharWidth,
                  winPos.y + line * rowHeight + rowHeight);

        if((u8)c < 0x20) {
            f32 greyOne = (f32)c / 0x19 * 0.2 + 0.8;
            ImGui::RenderFrame(bb.Min, bb.Max,
                               ImGui::ColorConvertFloat4ToU32(ImVec4(greyOne, greyOne, greyOne, 1)),
                               false);
        }
        else {
            ImGui::RenderTextClipped(bb.Min,
                                     bb.Max,
                                     &c, (const char*)&c + 1, NULL,
                                     ImVec2(0.5, 0.5), &bb);
        }


        if((i+1) & (columnCount-1)) ImGui::SameLine();
    }

    ImGui::PopFont();
}

void DataPanels::doIntegerPanel(const ImRect& panelRect, const i32 startLine, i32 bitSize, bool isSigned)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImVec2 winPos = window->DC.CursorPos;
    const ImGuiStyle& style = ImGui::GetStyle();

    // column header
    ImRect colHeadBb(winPos, winPos + ImVec2(panelRect.GetWidth(), columnHeaderHeight));
    const ImU32 headerColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.9, 0.9, 0.9, 1));
    ImGui::RenderFrame(colHeadBb.Min, colHeadBb.Max, headerColor, false, 0);

    for(i64 i = 0; i < columnCount; i++) {
        u32 hex = toHexStr(i);
        const char* label = (const char*)&hex;
        const ImVec2 label_size = ImGui::CalcTextSize(label, label+2);
        const ImVec2 size = ImGui::CalcItemSize(ImVec2(intColumnWidth, rowHeight),
                                                label_size.x, label_size.y);

        ImVec2 cellPos(i * intColumnWidth, columnHeaderHeight - rowHeight);
        cellPos += winPos;
        const ImRect bb(cellPos, cellPos + size);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5, 0.5, 0.5, 1));
        ImGui::RenderTextClipped(bb.Min, bb.Max, label, label+2,
                          &label_size, ImVec2(0.5, 0.5), &bb);
        ImGui::PopStyleColor();
    }
    winPos.y += columnHeaderHeight;

    ImGui::ItemSize(colHeadBb);

    const i32 lineCount = panelRect.GetHeight() / rowHeight + 1;
    const i32 itemCount = min(dataSize - (i64)startLine * columnCount, lineCount * columnCount);
    const i32 byteSize = bitSize >> 3; // div 8

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
