#include "data_panel.h"
#include <stdlib.h>

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
    ButtonBehavior(bb, id, &hovered, &held);

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
                      &label_size, ImVec2(1, 0.5), &bb);
    PopStyleColor();
}

static f32 GetComboHeight()
{
    const ImVec2 label_size = CalcTextSize("SomeText", NULL, true);
    return label_size.y + ImGui::GetStyle().FramePadding.y * 2.0f;
}

static void Tabs(const char* label, const char** names, const i32 count, i32* selected)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = GetStyle();
    ImGuiWindow* window = GetCurrentWindow();
    if(window->SkipItems)
        return;

    const ImGuiID id = window->GetID(label);
    ImRect rect = window->Rect();
    f32 buttonHeight = CalcTextSize(names[0], NULL, true).y + style.FramePadding.y * 2.0f;

    rect.Max.y = rect.Min.y + buttonHeight;
    ItemSize(rect, 0);
    if(!ItemAdd(rect, id)) {
        return;
    }

    RenderFrame(rect.Min, rect.Max, 0xffaaaaaa, false);

    f32 offX = 0;
    for(i32 i = 0; i < count; ++i) {
        const ImVec2 nameSize = CalcTextSize(names[i], NULL, true);
        ImRect bb = rect;
        bb.Min.x += offX;
        bb.Max.x = bb.Min.x + nameSize.x + style.FramePadding.x * 2.0f;
        offX += bb.GetWidth();

        const ImGuiID butId = id + i;
        bool held = false;
        bool hovered = false;
        const bool previouslyHeld = (g.ActiveId == butId);
        ButtonBehavior(bb, butId, &hovered, &held);

        if(held) {
            *selected = i;
        }

        ImU32 buttonColor = 0xffcccccc;
        ImU32 textColor = 0xff505050;
        if(*selected == i) {
            buttonColor = 0xffffffff;
            textColor = 0xff000000;
        }
        else if(hovered) {
            buttonColor = 0xffffc5a3;
            textColor = 0xff000000;
        }
        RenderFrame(bb.Min, bb.Max, buttonColor, false);

        PushStyleColor(ImGuiCol_Text, textColor);
        RenderTextClipped(bb.Min, bb.Max, names[i], NULL,
                          &nameSize, ImVec2(0.5, 0.5), &bb);
        PopStyleColor();
    }
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

// TODO: might not be such a good idea to generalize this function...
void DataPanels::processMouseInput(const ImRect& winRect)
{
    // Mouse hovering
    selectionState.hoverStart = -1;
    f32 panelOffX = 0;
    for(i32 p = 0; p < panelCount; ++p) {
        const f32 panelWidth = panelRectWidth[p];
        ImRect panelRect = winRect;
        panelRect.Min.x += panelOffX + rowHeaderWidth;
        panelRect.Max.x = panelRect.Min.x + panelWidth;
        panelRect.Min.y += ImGui::GetComboHeight() + columnHeaderHeight;
        panelOffX += panelWidth;

        ImVec2 mousePos = ImGui::GetIO().MousePos;

        // TODO: should be able to know every panel rect, all the time
        if(selectionState.lockedPanelId >= 0) {
            const auto& rect = selectionState.lockedPanelRect;
            mousePos.x = clamp(mousePos.x, rect.Min.x, rect.Max.x-1);
            mousePos.y = clamp(mousePos.y, rect.Min.y, rect.Max.y-1);
        }

        switch(panelType[p]) {
            case PT_HEX:
                selectionProcessMouseInput(p, mousePos, panelRect, columnWidth,
                                           rowHeight, scrollCurrent, 1);
                break;
            case PT_ASCII:
                selectionProcessMouseInput(p, mousePos, panelRect, asciiCharWidth,
                                           rowHeight, scrollCurrent, 1);
                break;
            case PT_INT8:
            case PT_UINT8:
                selectionProcessMouseInput(p, mousePos, panelRect, intColumnWidth * 1,
                                           rowHeight, scrollCurrent, 1);
                break;
            case PT_INT16:
            case PT_UINT16:
                selectionProcessMouseInput(p, mousePos, panelRect, intColumnWidth * 2,
                                           rowHeight, scrollCurrent, 2);
                break;
            case PT_INT32:
            case PT_UINT32:
                selectionProcessMouseInput(p, mousePos, panelRect, intColumnWidth * 4,
                                           rowHeight, scrollCurrent, 4);
                break;
            case PT_INT64:
            case PT_UINT64:
                selectionProcessMouseInput(p, mousePos, panelRect, intColumnWidth * 8,
                                           rowHeight, scrollCurrent, 8);
                break;
        }
    }
}

void DataPanels::selectionProcessMouseInput(const i32 panelId, ImVec2 mousePos, ImRect rect, const i32 columnWidth_,
                                            const i32 rowHeight_, const i32 startLine, const i32 hoverLen)
{
    const auto& io = ImGui::GetIO();
    ImVec2 relMousePos = mousePos - rect.Min;
    if(!rect.Contains(mousePos)) {
        return;
    }

    // hover
    i32 hoverColumn = relMousePos.x / columnWidth_;
    i32 hoverLine = relMousePos.y / rowHeight_;
    selectionState.hoverStart = (startLine + hoverLine) * columnCount + hoverColumn * hoverLen;
    selectionState.hoverEnd = selectionState.hoverStart + hoverLen;

    // selection
    if(io.MouseClicked[0]) {
        selectionState.selectStart = (startLine + hoverLine) * columnCount + hoverColumn * hoverLen;
        selectionState.selectEnd = selectionState.selectStart + hoverLen-1;
        selectionState.lockedPanelId = panelId;
        selectionState.lockedPanelRect = rect;
        return;
    }

    if(!io.MouseDown[0]) {
        selectionState.lockedPanelId = -1;
    }

    hoverColumn = relMousePos.x / columnWidth_;
    hoverLine = relMousePos.y / rowHeight_;

    if(io.MouseDown[0] && selectionState.selectStart >= 0) {
        i64 startCell = selectionState.selectStart & ~(hoverLen-1);
        i64 hoveredCell = (startLine + hoverLine) * columnCount + hoverColumn * hoverLen;
        if(hoveredCell < startCell) {
            selectionState.selectEnd = (startLine + hoverLine) * columnCount + hoverColumn * hoverLen;
            selectionState.selectStart = startCell + hoverLen - 1;
        }
        else {
            selectionState.selectEnd = (startLine + hoverLine) * columnCount + hoverColumn * hoverLen
                    + hoverLen-1;
            selectionState.selectStart = startCell;
        }
    }
}

bool DataPanels::selectionInHoverRange(i64 dataOffset)
{
    if(selectionState.hoverStart < 0) return false;
    return dataOffset >= selectionState.hoverStart && dataOffset < selectionState.hoverEnd;
}

bool DataPanels::selectionInSelectionRange(i64 dataOffset)
{
    if(selectionState.selectStart < 0) return false;
    i64 selMin = min(selectionState.selectStart, selectionState.selectEnd);
    i64 selMax = max(selectionState.selectStart, selectionState.selectEnd);
    return dataOffset >= selMin && dataOffset <= selMax;
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

    constexpr char* panelComboItems[] = {
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

    // START MAIN LEFT (DATA PANELS)
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::BeginChild("Mainframe_left", ImVec2(viewRect.GetWidth() - inspectorWidth, -1), false,
                      ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse);
#if 1
        const ImRect& winRect = ImGui::GetCurrentWindow()->Rect();

        ImGui::DoScrollbarVertical(&scrollCurrent,
                                   (viewRect.GetHeight() - columnHeaderHeight)/rowHeight, // page size (in lines)
                                   dataSize/columnCount + 2); // total lines (for last line visibility)

        processMouseInput(winRect);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 0));

        // row header
        ImGui::BeginChild("##ROWHEADER", ImVec2(rowHeaderWidth, -1), false,
                          ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse);

        const i32 lineCount = (winRect.GetHeight() - columnHeaderHeight) / rowHeight + 1;
        const ImVec2& winPos = winRect.Min;

        ImRect rowHeadBb(winPos, winPos + ImVec2(rowHeaderWidth, winRect.GetHeight()));
        const ImU32 headerColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.9, 0.9, 0.9, 1));
        const ImU32 headerColorOdd = ImGui::ColorConvertFloat4ToU32(ImVec4(0.85, 0.85, 0.85, 1));
        ImGui::RenderFrame(rowHeadBb.Min, rowHeadBb.Max, headerColor, false, 0);

        for(i64 i = 0; i < lineCount; ++i) {
            u32 hex = toHexStr(((i + scrollCurrent) * columnCount) & 0xFF);

            const char* label = (const char*)&hex;
            const ImVec2 label_size = ImGui::CalcTextSize(label, label+2);
            const ImVec2 size = ImGui::CalcItemSize(ImVec2(rowHeaderWidth, rowHeight),
                                                    label_size.x, label_size.y);

            ImVec2 cellPos(0, rowHeight * i);
            cellPos += winPos;
            cellPos.y += columnHeaderHeight + 21;
            const ImRect bb(cellPos, cellPos + size);

            ImVec4 textCol = ImVec4(0.5, 0.5, 0.5, 1);
            if(i & 1) {
                textCol = ImVec4(0.45, 0.45, 0.45, 1);
                ImGui::RenderFrame(bb.Min, bb.Max, headerColorOdd, false, 0);
            }

            ImGui::PushStyleColor(ImGuiCol_Text, textCol);
            ImGui::RenderTextClipped(bb.Min, bb.Max, label, label+2,
                              &label_size, ImVec2(0.3, 0.5), &bb);

            ImGui::PopStyleColor();
        }

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

            ImRect panelRect = winRect;
            panelRect.Max.x = panelRect.Min.x + panelWidth;

            /*LOG("panelRect={min: %d,%d; max: %d,%d}",
                (i32)panelRect.Min.x, (i32)panelRect.Min.y,
                (i32)panelRect.Max.x, (i32)panelRect.Max.y);*/

            switch(panelType[p]) {
                case PT_HEX:
                    doHexPanel(panelRect, scrollCurrent);
                    break;
                case PT_ASCII:
                    doAsciiPanel(panelRect, scrollCurrent);
                    break;
                case PT_INT8:
                    doIntegerPanel(panelRect, scrollCurrent, 8, true);
                    break;
                case PT_UINT8:
                    doIntegerPanel(panelRect, scrollCurrent, 8, false);
                    break;
                case PT_INT16:
                    doIntegerPanel(panelRect, scrollCurrent, 16, true);
                    break;
                case PT_UINT16:
                    doIntegerPanel(panelRect, scrollCurrent, 16, false);
                    break;
                case PT_INT32:
                    doIntegerPanel(panelRect, scrollCurrent, 32, true);
                    break;
                case PT_UINT32:
                    doIntegerPanel(panelRect, scrollCurrent, 32, false);
                    break;
                case PT_INT64:
                    doIntegerPanel(panelRect, scrollCurrent, 64, true);
                    break;
                case PT_UINT64:
                    doIntegerPanel(panelRect, scrollCurrent, 64, false);
                    break;
            }


            ImGui::EndChild();
            ImGui::PopID();

            if(p+1 < panelCount) ImGui::SameLine();
        }

        ImGui::PopStyleVar(1);
#endif

        ImGui::Text("LEFT");

    // END MAIN LEFT (DATA PANELS)
    ImGui::EndChild();
    ImGui::SameLine();

    ImGui::BeginChild("Mainframe_right", ImVec2(inspectorWidth, -1), false);

        const char* tabs[] = {
            "Inspector",
            "Script",
            "Output"
        };

        static i32 selectedTab = 0;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 5));
        ImGui::Tabs("Inspector_tabs", tabs, IM_ARRAYSIZE(tabs), &selectedTab);
        ImGui::PopStyleVar(1);

        switch(selectedTab) {
            case 0:
                ImGui::Text("tab0 selected");
                break;
            case 1:
                ImGui::Text("tab1 selected");
                break;
            case 2:
                ImGui::Text("tab2 selected");
                break;
        }


    ImGui::EndChild();

    ImGui::PopStyleVar(1);
}

void DataPanels::doHexPanel(const ImRect& panelRect, const i32 startLine)
{
    const i32 lineCount = (panelRect.GetHeight() - columnHeaderHeight) / rowHeight;
    const i32 itemCount = min(dataSize - (i64)startLine * columnCount, lineCount * columnCount);

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if(window->SkipItems)
        return;

    ImVec2 winPos = window->DC.CursorPos;
    /*const ImGuiID id = window->GetID("#HEXPANEL");
    ImGui::ItemSize(panelRect, 0);
    if(!ImGui::ItemAdd(panelRect, id))
        return;*/

    const ImVec2 cellSize(columnWidth, rowHeight);

    // column header
    ImRect colHeadBb(winPos, winPos + ImVec2(panelRect.GetWidth(), columnHeaderHeight));
    const ImU32 headerColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.9, 0.9, 0.9, 1));
    const ImU32 headerColorOdd = ImGui::ColorConvertFloat4ToU32(ImVec4(0.85, 0.85, 0.85, 1));
    ImGui::RenderFrame(colHeadBb.Min, colHeadBb.Max, headerColor, false, 0);

    for(i64 i = 0; i < columnCount; ++i) {
        u32 hex = toHexStr(i);
        const char* label = (const char*)&hex;
        const ImVec2 label_size = ImGui::CalcTextSize(label, label+2);
        const ImVec2 size = ImGui::CalcItemSize(ImVec2(columnWidth, columnHeaderHeight),
                                                label_size.x, label_size.y);

        ImVec2 cellPos(i * cellSize.x, 0);
        cellPos += winPos;
        const ImRect bb(cellPos, cellPos + size);

        if(i & 1) {
            ImGui::RenderFrame(bb.Min, bb.Max, headerColorOdd, false, 0);
        }

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5, 0.5, 0.5, 1));
        ImGui::RenderTextClipped(bb.Min, bb.Max, label, label+2,
                          &label_size, ImVec2(0.5, 0.5), &bb);
        ImGui::PopStyleColor();
    }
    winPos.y += columnHeaderHeight;

    // hex table
    const i64 startLineOff = (i64)startLine * columnCount;
    for(i64 i = 0; i < itemCount; ++i) {
        const i64 dataOffset = i + startLineOff;
        u8 val = data[dataOffset];
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

        if(selectionInSelectionRange(dataOffset)) {
            ImGui::RenderFrame(bb.Min, bb.Max, selectedFrameColor, false, 0);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
        }
        else if(selectionInHoverRange(dataOffset)) {
            ImGui::RenderFrame(bb.Min, bb.Max, hoverFrameColor, false, 0);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(textColor, textColor, textColor, 1));
        }
        else {
            const ImU32 frameColor = ImGui::ColorConvertFloat4ToU32(ImVec4(bgColor, bgColor, bgColor, 1));
            ImGui::RenderFrame(bb.Min, bb.Max, frameColor, false, 0);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(textColor, textColor, textColor, 1));
        }

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
    winPos.y += columnHeaderHeight;

    const i32 lineCount = (panelRect.GetHeight() - columnHeaderHeight) / rowHeight + 1;
    const i32 itemCount = min(dataSize - (i64)startLine * columnCount, lineCount * columnCount);

    ImGui::PushFont(fontMono);

    const i64 startLineOff = (i64)startLine * columnCount;
    for(i64 i = 0; i < itemCount; ++i) {
        const i64 dataOff = i + startLineOff;
        const char c = (char)data[dataOff];
        i32 line = i / columnCount;
        i32 column = i & (columnCount - 1);
        ImRect bb(winPos.x + column * asciiCharWidth, winPos.y + line * rowHeight,
                  winPos.x + column * asciiCharWidth + asciiCharWidth,
                  winPos.y + line * rowHeight + rowHeight);


        const bool isHovered = selectionInHoverRange(dataOff);
        const bool isSelected = selectionInSelectionRange(dataOff);
        ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);
        if(isSelected) {
            textColor = selectedTextColor;
        }

        ImGui::PushStyleColor(ImGuiCol_Text, textColor);

        if((u8)c < 0x20) {
            f32 greyOne = (f32)c / 0x19 * 0.2 + 0.8;
            ImU32 frameColor = ImGui::ColorConvertFloat4ToU32(ImVec4(greyOne, greyOne, greyOne, 1));
            if(isSelected) {
                frameColor = selectedFrameColor;
            }
            else if(isHovered) {
                frameColor = hoverFrameColor;
            }

            ImGui::RenderFrame(bb.Min, bb.Max,
                               frameColor,
                               false);
        }
        else {
            ImU32 frameColor = ImGui::GetColorU32(ImGuiCol_FrameBg);
            if(isSelected) {
                frameColor = selectedFrameColor;
            }
            else if(isHovered) {
                frameColor = hoverFrameColor;
            }

            ImGui::RenderFrame(bb.Min, bb.Max,
                               frameColor,
                               false);

            ImGui::RenderTextClipped(bb.Min,
                                     bb.Max,
                                     &c, (const char*)&c + 1, NULL,
                                     ImVec2(0.5, 0.5), &bb);
        }

        ImGui::PopStyleColor();


        if((i+1) & (columnCount-1)) ImGui::SameLine();
    }

    ImGui::PopFont();
}

void DataPanels::doIntegerPanel(const ImRect& panelRect, const i32 startLine, i32 bitSize, bool isSigned)
{
    const i32 byteSize = bitSize >> 3; // div 8

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImVec2 winPos = window->DC.CursorPos;
    const ImGuiStyle& style = ImGui::GetStyle();

    // column header
    ImRect colHeadBb(winPos, winPos + ImVec2(panelRect.GetWidth(), columnHeaderHeight));
    const ImU32 headerColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.9, 0.9, 0.9, 1));
    const ImU32 headerColorOdd = ImGui::ColorConvertFloat4ToU32(ImVec4(0.85, 0.85, 0.85, 1));
    ImGui::RenderFrame(colHeadBb.Min, colHeadBb.Max, headerColor, false, 0);

    for(i64 i = 0; i < columnCount; i++) {
        u32 hex = toHexStr(i);
        const char* label = (const char*)&hex;
        const ImVec2 label_size = ImGui::CalcTextSize(label, label+2);
        const ImVec2 size = ImGui::CalcItemSize(ImVec2(intColumnWidth, rowHeight),
                                                label_size.x, label_size.y);

        ImVec2 cellPos(i * intColumnWidth, 0);
        cellPos += winPos;
        const ImRect bb(cellPos, cellPos + size);

        if(i & 1) {
            ImGui::RenderFrame(bb.Min, bb.Max, headerColorOdd, false, 0);
        }

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5, 0.5, 0.5, 1));
        ImGui::RenderTextClipped(bb.Min, bb.Max, label, label+2,
                          &label_size, ImVec2(0.5, 0.5), &bb);
        ImGui::PopStyleColor();
    }
    winPos.y += columnHeaderHeight;

    ImGui::ItemSize(colHeadBb);

    const i32 lineCount = (panelRect.GetHeight() - columnHeaderHeight) / rowHeight + 1;
    i32 itemCount = min(dataSize - (i64)startLine * columnCount, lineCount * columnCount);
    itemCount &= ~(itemCount & (byteSize-1)); // round off item count based of byteSize
    const ImVec2 cellSize(intColumnWidth * byteSize, rowHeight);

    const i64 startLineOff = (i64)startLine * columnCount;
    for(i64 i = 0; i < itemCount; i += byteSize) {
        const i64 dataOff = i + startLineOff;
        char integerStr[32];

        // TODO: this is VERY innefficient, find a better way to do this
        if(isSigned) {
            switch(bitSize) {
                case 8:
                    itoa(*(i8*)&data[dataOff], integerStr, 10);
                    break;
                case 16:
                    itoa(*(i16*)&data[dataOff], integerStr, 10);
                    break;
                case 32:
                    itoa(*(i32*)&data[dataOff], integerStr, 10);
                    break;
                case 64:
                    sprintf(integerStr, "%lld", *(i64*)&data[dataOff]);
                    break;
                default:
                    assert(0);
                    break;
            }
        }
        else {
            switch(bitSize) {
                case 8:
                    itoa(*(u8*)&data[dataOff], integerStr, 10);
                    break;
                case 16:
                    itoa(*(u16*)&data[dataOff], integerStr, 10);
                    break;
                case 32:
                    itoa(*(u32*)&data[dataOff], integerStr, 10);
                    break;
                case 64:
                    sprintf(integerStr, "%llu", *(u64*)&data[dataOff]);
                    break;
                default:
                    assert(0);
                    break;
            }
        }

        const ImVec2 labelSize = ImGui::CalcTextSize(integerStr, NULL, true);
        ImVec2 size = ImGui::CalcItemSize(cellSize, labelSize.x, labelSize.y);

        i32 col = i & (columnCount - 1);
        i32 line = i / columnCount;
        ImVec2 cellPos(col * intColumnWidth, line * cellSize.y);
        ImRect bb(winPos + cellPos, winPos + cellPos + size);

        ImU32 frameColor = ImGui::GetColorU32(ImGuiCol_FrameBg);
        ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);

        // TODO: find a better way to do this (overlapping ranges)
        bool hovered = false;
        bool selected = false;
        for(i32 h = 0; h < byteSize; ++h) {
            hovered |= selectionInHoverRange(dataOff+h);
            selected |= selectionInSelectionRange(dataOff+h);
        }

        if(selected) {
            frameColor = selectedFrameColor;
            textColor = selectedTextColor;
        }
        else if(hovered) {
            frameColor = hoverFrameColor;
        }

        ImGui::RenderFrame(bb.Min, bb.Max, frameColor, false, 0);

        bb.Translate(ImVec2(-4, 0));

        ImGui::PushStyleColor(ImGuiCol_Text, textColor);
        ImGui::RenderTextClipped(bb.Min, bb.Max, integerStr, NULL,
                                 &labelSize, ImVec2(1, 0.5), &bb);
        ImGui::PopStyleColor();
    }
}
