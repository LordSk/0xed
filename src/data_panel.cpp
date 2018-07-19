#include "data_panel.h"
#include "bricks.h"
#include "imgui_extended.h"
#include <stdlib.h>
#include <float.h>

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

    "Float32",
    "Float64",
};

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
void DataPanels::processMouseInput(ImRect winRect)
{
    winRect.Max.x -= ImGui::GetStyle().ScrollbarSize;
    winRect.Max.y -= ImGui::GetStyle().ScrollbarSize;

    selectionState.hoverStart = -1;

    ImGuiIO& io = ImGui::GetIO();
    if(!winRect.Contains(io.MousePos)) {
        return;
    }
    if(ImGui::IsAnyPopupOpen()) {
        return;
    }

    f32 panelOffX = 0;
    bool mouseClickedOutsidePanel = io.MouseClicked[0] && winRect.Contains(io.MousePos);
    ImVec2 mousePos = io.MousePos - winRect.Min; // window space

    ImRect panelRect[PANEL_MAX_COUNT];
    const f32 scrollX = ImGui::GetScrollX();

    for(i32 p = 0; p < panelCount; ++p) {
        const f32 panelWidth = panelRectWidth[p];
        panelRect[p] = {ImVec2(0,columnHeaderHeight), ImVec2(panelWidth, winRect.GetHeight())};
        panelRect[p].Translate(ImVec2(panelOffX - scrollX, 0));
        panelOffX += panelWidth + panelSpacing;
    }

    for(i32 p = 0; p < panelCount; ++p) {
        ImRect prect = panelRect[p];
        mouseClickedOutsidePanel &= !prect.Contains(mousePos);

        // TODO: should be able to know every panel rect, all the time
        if(selectionState.lockedPanelId >= 0) {
            const auto& rect = panelRect[selectionState.lockedPanelId];
            mousePos.x = clamp(mousePos.x, rect.Min.x, rect.Max.x-1);
            mousePos.y = clamp(mousePos.y, rect.Min.y, rect.Max.y-1);
        }

        switch(panelType[p]) {
            case PT_HEX:
                selectionProcessMouseInput(p, mousePos, prect, columnWidth,
                                           rowHeight, scrollCurrentLine, 1);
                break;
            case PT_ASCII:
                selectionProcessMouseInput(p, mousePos, prect, asciiCharWidth,
                                           rowHeight, scrollCurrentLine, 1);
                break;
            case PT_INT8:
            case PT_UINT8:
                selectionProcessMouseInput(p, mousePos, prect, intColumnWidth * 1,
                                           rowHeight, scrollCurrentLine, 1);
                break;
            case PT_INT16:
            case PT_UINT16:
                selectionProcessMouseInput(p, mousePos, prect, intColumnWidth * 2,
                                           rowHeight, scrollCurrentLine, 2);
                break;
            case PT_INT32:
            case PT_UINT32:
            case PT_FLOAT32:
                selectionProcessMouseInput(p, mousePos, prect, intColumnWidth * 4,
                                           rowHeight, scrollCurrentLine, 4);
                break;
            case PT_INT64:
            case PT_UINT64:
            case PT_FLOAT64:
                selectionProcessMouseInput(p, mousePos, prect, intColumnWidth * 8,
                                           rowHeight, scrollCurrentLine, 8);
                break;
        }
    }

    // clear selection
    if(mouseClickedOutsidePanel || io.MouseClicked[1]) {
        deselect();
    }
}

void DataPanels::selectionProcessMouseInput(const i32 panelId, ImVec2 mousePos, ImRect rect,
                                            const i32 columnWidth_, const i32 rowHeight_,
                                            const i32 startLine, const i32 hoverLen)
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
    if(io.MouseClicked[0] &&
       ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows)) {
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

    if(io.MouseDown[0] && selectionState.selectStart >= 0 && selectionState.lockedPanelId >= 0) {
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

bool DataPanels::selectionIsEmpty()
{
    if(selectionState.selectStart < 0) return true;
    return false;
}

void DataPanels::deselect()
{
    selectionState.selectStart = -1;
    selectionState.selectEnd = -1;
    selectionState.lockedPanelId = -1;
}

DataPanels::DataPanels()
{
    memset(panelType, 0, sizeof(panelType));
    panelType[0] = PT_HEX;
    panelType[1] = PT_ASCII;
    panelType[2] = PT_HEX;
    panelColorDisplay[2] = ColorDisplay::BRICK_COLOR;
}

void DataPanels::setFileBuffer(u8* buff, i64 buffSize)
{
    fileBuffer = buff;
    fileBufferSize = buffSize;
    selectionState = {};
    scrollCurrentLine = 0;
}

void DataPanels::addNewPanel()
{
    if(panelCount >= PANEL_MAX_COUNT) {
        return;
    }

    panelType[panelCount++] = PT_HEX;
}

void DataPanels::goTo(i32 offset)
{
    if(offset >=0 && offset < fileBufferSize) {
        scrollCurrentLine = offset / columnCount;
    }
}

i32 DataPanels::getSelectedInt()
{
    if(selectionState.selectStart > -1) {
        i64 selMin = min(selectionState.selectStart, selectionState.selectEnd);
        i64 selMax = max(selectionState.selectStart, selectionState.selectEnd);
        if((selMax - selMin + 1) == 4) {
            return *(i32*)(fileBuffer + selMin);
        }
    }
    return 0;
}

void DataPanels::calculatePanelWidth()
{
    childPanelWidth = 0;
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
            case PT_FLOAT32:
            case PT_FLOAT64:
                panelRectWidth[p] = intColumnWidth * columnCount;
                break;

           childPanelWidth += panelRectWidth[p] + panelSpacing;
        }
    }
}

void DataPanels::doRowHeader(const ImRect& winRect)
{
    // row header
    ImGui::ItemSize(ImVec2(rowHeaderWidth, -1));

    const i32 lineCount = (winRect.GetHeight() - columnHeaderHeight) / rowHeight + 1;
    const ImVec2& winPos = winRect.Min;

    ImRect rowHeadBb(winPos, winPos + ImVec2(rowHeaderWidth, winRect.GetHeight()));
    const ImU32 headerColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.9, 0.9, 0.9, 1));
    const ImU32 headerColorOdd = ImGui::ColorConvertFloat4ToU32(ImVec4(0.85, 0.85, 0.85, 1));
    ImGui::RenderFrame(rowHeadBb.Min, rowHeadBb.Max, headerColor, false, 0);

    for(i64 i = 0; i < lineCount; ++i) {
        u32 hex = toHexStr(((i + scrollCurrentLine) * columnCount) & 0xFF);

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
}

void DataPanels::doUi()
{
    if(!fileBuffer) {
        return;
    }

    const i32 totalLineCount = fileBufferSize/columnCount + 4;
    i32 panelMarkedForDelete = -1;
    const f32 panelHeaderHeight = ImGui::GetComboHeight();
    calculatePanelWidth();

    // force appropriate content height
    ImGui::SetNextWindowContentSize(
                ImVec2(0,
                totalLineCount * rowHeight)
                );
    ImGui::BeginChild("#child_panel", ImVec2(-1, -1), false,
                      ImGuiWindowFlags_HorizontalScrollbar);

        ImGuiWindow* window = ImGui::GetCurrentWindow();
        scrollCurrentLine = window->Scroll.y / rowHeight;
        ImRect inputRect = window->Rect();
        inputRect.Min.y += panelHeaderHeight;
        processMouseInput(inputRect);

        f32 offsetX = 0;
        for(i32 p = 0; p < panelCount; ++p) {
            const f32 panelWidth = panelRectWidth[p];

            ImVec2 csp;
            csp.x = - window->Scroll.x + offsetX;
            csp.y = window->Rect().Min.y;
            ImGui::SetCursorScreenPos(csp);
            offsetX += panelWidth + panelSpacing;
            ImGui::BeginGroup();

            ImGui::PushItemWidth(panelWidth - panelCloseButtonWidth - panelColorButtonWidth);
            ImGui::PushID(&panelType[p]);
            ImGui::Combo("##PanelType", &panelType[p], panelComboItems, IM_ARRAYSIZE(panelComboItems),
                         IM_ARRAYSIZE(panelComboItems));
            ImGui::PopID();
            ImGui::PopItemWidth();

            ImGui::SameLine();

            ImGui::PushID(&panelType[p] + 0xbaab + 'C');
            if(ImGui::Button("C", ImVec2(panelColorButtonWidth, 0))) {
                panelColorDisplay[p] = (ColorDisplay)(((i32)panelColorDisplay[p] + 1) % 3);
            }
            ImGui::PopID();

            ImGui::SameLine();

            ImGui::PushID(&panelType[p] + 0xb00b + 'X');
            if(ImGui::Button("X", ImVec2(panelCloseButtonWidth, 0))) {
                panelMarkedForDelete = p;
                LOG("DELETE: %d", p);
            }
            ImGui::PopID();

            ImGui::EndGroup();

            csp.y += panelHeaderHeight;
            ImGui::SetCursorScreenPos(csp);

            switch(panelType[p]) {
                case PT_HEX:
                    doHexPanel(p, panelWidth, scrollCurrentLine, panelColorDisplay[p]);
                    break;
                case PT_ASCII:
                    doAsciiPanel(p, panelWidth, scrollCurrentLine, panelColorDisplay[p]);
                    break;
                case PT_INT8:
                    doFormatPanel<i8>(p, panelWidth, scrollCurrentLine, "%d");
                    break;
                case PT_UINT8:
                    doFormatPanel<u8>(p, panelWidth, scrollCurrentLine, "%u");
                    break;
                case PT_INT16:
                    doFormatPanel<i16>(p, panelWidth, scrollCurrentLine, "%d");
                    break;
                case PT_UINT16:
                    doFormatPanel<u16>(p, panelWidth, scrollCurrentLine, "%u");
                    break;
                case PT_INT32:
                    doFormatPanel<i32>(p, panelWidth, scrollCurrentLine, "%d");
                    break;
                case PT_UINT32:
                    doFormatPanel<u32>(p, panelWidth, scrollCurrentLine, "%u");
                    break;
                case PT_INT64:
                    doFormatPanel<i64>(p, panelWidth, scrollCurrentLine, "%lld");
                    break;
                case PT_UINT64:
                    doFormatPanel<u64>(p, panelWidth, scrollCurrentLine, "%llu");
                    break;
                case PT_FLOAT32:
                    // TODO: color range to better see which values may be useful
                    doFormatPanel<f32>(p, panelWidth, scrollCurrentLine, "%g");
                    break;
                case PT_FLOAT64:
                    doFormatPanel<f64>(p, panelWidth, scrollCurrentLine, "%g");
                    break;
                default:
                    assert(0);
                    break;
            }

            if(p+1 < panelCount) {
                ImGui::SameLine();
                ImGui::ItemSize(ImVec2(panelSpacing, -1));
                ImGui::SameLine();
            }
        }

    ImGui::EndChild();

    if(panelMarkedForDelete >= 0 && panelCount > 1) {
        if(panelMarkedForDelete+1 < panelCount) {
            panelType[panelMarkedForDelete] = panelType[panelMarkedForDelete+1];
            memmove(panelType + panelMarkedForDelete, panelType + panelMarkedForDelete + 1,
                    sizeof(panelType[0]) * (panelCount - panelMarkedForDelete - 1));
        }
        panelCount--;
    }
}

void DataPanels::doHexPanel(i32 pid, f32 panelWidth, const i32 startLine, ColorDisplay colorDisplay)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImVec2 winPos = ImGui::GetCursorScreenPos();
    ImVec2 panelSize = {panelWidth, 10}; // height doesnt really matter here
    ImRect panelRect = {winPos, winPos + panelSize};

    const i32 lineCount = window->Rect().GetHeight() / rowHeight;
    const i32 itemCount = min(fileBufferSize - (i64)startLine * columnCount, lineCount * columnCount);

    const ImGuiID imid = window->GetID(&panelRectWidth[pid]);
    ImGui::ItemSize(panelRect, 0);
    if(!ImGui::ItemAdd(panelRect, imid))
        return;

    const ImVec2 cellSize(columnWidth, rowHeight);

    // column header
    ImVec2 headerPos = winPos;
    ImRect colHeadBb(headerPos, headerPos + ImVec2(panelSize.x, columnHeaderHeight));
    const ImU32 headerColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.9, 0.9, 0.9, 1));
    const ImU32 headerColorOdd = ImGui::ColorConvertFloat4ToU32(ImVec4(0.85, 0.85, 0.85, 1));
    ImGui::RenderFrame(colHeadBb.Min, colHeadBb.Max, headerColor, false, 0);

    winPos.y += columnHeaderHeight;

    for(i64 i = 0; i < columnCount; ++i) {
        u32 hex = toHexStr(i);
        const char* label = (const char*)&hex;
        const ImVec2 label_size = ImGui::CalcTextSize(label, label+2);
        const ImVec2 size = ImGui::CalcItemSize(ImVec2(columnWidth, columnHeaderHeight),
                                                label_size.x, label_size.y);

        ImVec2 cellPos(i * cellSize.x, 0);
        cellPos += headerPos;
        const ImRect bb(cellPos, cellPos + size);

        if(i & 1) {
            ImGui::RenderFrame(bb.Min, bb.Max, headerColorOdd, false, 0);
        }

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5, 0.5, 0.5, 1));
        ImGui::RenderTextClipped(bb.Min, bb.Max, label, label+2,
                          &label_size, ImVec2(0.5, 0.5), &bb);
        ImGui::PopStyleColor();
    }

    // hex table
    const i64 startDataOff = startLine * columnCount;
    for(i64 i = 0; i < itemCount; ++i) {
        const i64 dataOffset = i + startDataOff;
        u8 val = fileBuffer[dataOffset];
        u32 hex = toHexStr(val);

        ImU32 frameColor = 0xffffffff;
        ImU32 textColor = 0xff000000;
        f32 geyScale = val/255.f * 0.7 + 0.3;

        const char* label = (const char*)&hex;
        const ImVec2 label_size = ImGui::CalcTextSize(label, label+2);
        const ImVec2 size = ImGui::CalcItemSize(cellSize, label_size.x, label_size.y);

        i32 column = i % columnCount;
        i32 line = i / columnCount;
        ImVec2 cellPos(column * cellSize.x, line * cellSize.y);
        cellPos += winPos;
        const ImRect bb(cellPos, cellPos + size);

        if(selectionInSelectionRange(dataOffset)) {
            frameColor = selectedFrameColor;
            textColor = 0xffffffff;
        }
        else if(selectionInHoverRange(dataOffset)) {
            frameColor = hoverFrameColor;
        }
        else {
            if(colorDisplay == ColorDisplay::GRAY_SCALE) {
                frameColor = ImGui::ColorConvertFloat4ToU32(ImVec4(geyScale, geyScale, geyScale, 1.0f));
            }
            else if(colorDisplay == ColorDisplay::BRICK_COLOR) {
                assert(brickWall);
                const Brick* b = brickWall->getBrick(dataOffset);
                if(b) {
                    if(b->userStruct) {
                        const Brick* sub = b->userStruct->getBrick(dataOffset - b->start);
                        assert(sub);
                        textColor = sub->color;
                    }
                    frameColor = b->color;
                }
                else {
                    frameColor = ImGui::ColorConvertFloat4ToU32(ImVec4(geyScale, geyScale, geyScale, 1.0f));
                }
            }
        }

        ImGui::PushStyleColor(ImGuiCol_Text, textColor);
        ImGui::RenderFrame(bb.Min, bb.Max, frameColor, false, 0);
        ImGui::RenderTextClipped(bb.Min, bb.Max, label, label+2,
                          &label_size, ImVec2(0.5, 0.5), &bb);
        ImGui::PopStyleColor(1);
    }
}

void DataPanels::doAsciiPanel(i32 pid, f32 panelWidth, const i32 startLine, ColorDisplay colorDisplay)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImVec2 winPos = ImGui::GetCursorScreenPos();
    ImVec2 panelSize = {panelWidth, 10}; // height doesnt really matter here
    ImRect panelRect = {winPos, winPos + panelSize};

    const i32 lineCount = window->Rect().GetHeight() / rowHeight;
    const i32 itemCount = min(fileBufferSize - (i64)startLine * columnCount, lineCount * columnCount);

    const ImGuiID imid = window->GetID(&panelRectWidth[pid]);
    ImGui::ItemSize(panelRect, 0);
    if(!ImGui::ItemAdd(panelRect, imid))
        return;

    // column header
    ImVec2 headerPos = winPos;
    ImRect colHeadBb(headerPos, headerPos + ImVec2(panelRect.GetWidth(), columnHeaderHeight));
    const ImU32 headerColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 1));
    ImGui::RenderFrame(colHeadBb.Min, colHeadBb.Max, headerColor, false, 0);

    winPos.y += columnHeaderHeight;

    ImGui::PushFont(fontMono);

    // ascii table
    const i64 startDataOff = startLine * columnCount;
    for(i64 i = 0; i < itemCount; ++i) {
        const i64 dataOff = i + startDataOff;
        const char c = (char)fileBuffer[dataOff];
        i32 line = i / columnCount;
        i32 column = i % columnCount;
        ImRect bb(winPos.x + column * asciiCharWidth, winPos.y + line * rowHeight,
                  winPos.x + column * asciiCharWidth + asciiCharWidth,
                  winPos.y + line * rowHeight + rowHeight);


        const bool isHovered = selectionInHoverRange(dataOff);
        const bool isSelected = selectionInSelectionRange(dataOff);

        ImU32 frameColor = ImGui::GetColorU32(ImGuiCol_FrameBg);
        f32 greyOne = (f32)c / 0x19 * 0.2 + 0.8;

        if(colorDisplay == ColorDisplay::BRICK_COLOR) {
            assert(brickWall);
            const Brick* b = brickWall->getBrick(dataOff);
            if(b) {
                frameColor = b->color;
            }
            else if((u8)c < 0x20) {
                frameColor = ImGui::ColorConvertFloat4ToU32(ImVec4(greyOne, greyOne, greyOne, 1));
            }
        }
        else if((u8)c < 0x20 && colorDisplay != ColorDisplay::PLAIN) {
            frameColor = ImGui::ColorConvertFloat4ToU32(ImVec4(greyOne, greyOne, greyOne, 1));
        }

        if(isSelected) {
            frameColor = selectedFrameColor;
        }
        else if(isHovered) {
            frameColor = hoverFrameColor;
        }

        ImGui::RenderFrame(bb.Min, bb.Max, frameColor, false);

        if((u8)c > 0x19) {
            ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);
            if(isSelected) {
                textColor = selectedTextColor;
            }

            ImGui::PushStyleColor(ImGuiCol_Text, textColor);

            ImGui::RenderTextClipped(bb.Min,
                                     bb.Max,
                                     &c, (const char*)&c + 1, NULL,
                                     ImVec2(0.5, 0.5), &bb);

            ImGui::PopStyleColor();
        }


        if((i+1) % columnCount) ImGui::SameLine();
    }

    ImGui::PopFont();
}

template<typename T>
void DataPanels::doFormatPanel(i32 pid, f32 panelWidth, const i32 startLine, const char* format)
{
    if(columnCount % sizeof(T)) {
        ImGui::TextColored(ImVec4(0.8, 0, 0, 1), "\nColumn count is not divisible by %d", sizeof(T));
        return;
    }

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImVec2 winPos = ImGui::GetCursorScreenPos();
    const ImVec2 panelPos = winPos;
    const ImVec2 panelSize = {panelWidth, 10}; // height doesnt really matter here
    const ImRect panelRect = {winPos, winPos + panelSize};

    const i32 byteSize = sizeof(T);
    const i32 bitSize = byteSize << 3;

    const i32 lineCount = window->Rect().GetHeight() / rowHeight;
    i64 itemCount = min(fileBufferSize - (i64)startLine * columnCount, lineCount * columnCount);
    itemCount &= ~(itemCount & (byteSize-1)); // round off item count based of byteSize
    const i64 itemCount2 = itemCount;

    const ImGuiID imid = window->GetID(&panelRectWidth[pid]);
    ImGui::ItemSize(panelRect, 0);
    if(!ImGui::ItemAdd(panelRect, imid))
        return;

    // column header
    ImVec2 headerPos = winPos;
    ImRect colHeadBb(headerPos, headerPos + ImVec2(panelRect.GetWidth(), columnHeaderHeight));
    const ImU32 headerColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.9, 0.9, 0.9, 1));
    const ImU32 headerColorOdd = ImGui::ColorConvertFloat4ToU32(ImVec4(0.85, 0.85, 0.85, 1));
    ImGui::RenderFrame(colHeadBb.Min, colHeadBb.Max, headerColor, false, 0);

    for(i64 i = 0; i < columnCount; i++) {
        u32 hex = toHexStr(i);
        const char* label = (const char*)&hex;
        const ImVec2 label_size = ImGui::CalcTextSize(label, label+2);
        const ImVec2 size = ImGui::CalcItemSize(ImVec2(intColumnWidth, columnHeaderHeight),
                                                label_size.x, label_size.y);

        ImVec2 cellPos(i * intColumnWidth, 0);
        cellPos += headerPos;
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

    const ImVec2 cellSize(intColumnWidth * byteSize, rowHeight);

    const i64 startLineOff = (i64)startLine * columnCount;
    for(i64 i = 0; i < itemCount2; i += byteSize) {
        const i64 dataOff = i + startLineOff;
        char integerStr[32];
        sprintf(integerStr, format, *(T*)&fileBuffer[dataOff]);

        const ImVec2 labelSize = ImGui::CalcTextSize(integerStr, NULL, true);
        ImVec2 size = ImGui::CalcItemSize(cellSize, labelSize.x, labelSize.y);

        i32 col = i % columnCount;
        i32 line = i / columnCount;
        ImVec2 cellPos(col * intColumnWidth, line * cellSize.y);
        ImRect bb(winPos + cellPos, winPos + cellPos + size);

        f32 avgByteVal = fileBuffer[dataOff];
        for(i32 a = 1; a < byteSize; ++a) {
            avgByteVal += fileBuffer[dataOff + a];
        }
        avgByteVal /= byteSize;

        ImU32 frameColor = ImGui::GetColorU32(ImGuiCol_FrameBg);
        /*if((i + (line & 1) * byteSize) & byteSize) {
            frameColor = 0xffe0e0e0;
        }*/
        f32 bgColor = avgByteVal/255.f * 0.4 + 0.6;
        frameColor = ImGui::ColorConvertFloat4ToU32(ImVec4(bgColor, bgColor, bgColor, 1));
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


    ImU32 lineColor = 0xff808080;
    const i32 actualLineCount = (itemCount/columnCount)+1;
    const f32 colHeight = actualLineCount * rowHeight;

    for(i32 c = 0; c <= columnCount; ++c) {
        ImRect bb(panelPos.x + cellSize.x * c, panelPos.y + columnHeaderHeight,
                  panelPos.x + cellSize.x * c + 1, panelPos.y + colHeight + 2);
        ImGui::RenderFrame(bb.Min, bb.Max, lineColor, false, 0);
    }

    for(i32 l = 0; l < actualLineCount; ++l) {
        ImRect bb(winPos.x, winPos.y + cellSize.y * l,
                  winPos.x + panelSize.x + 1, winPos.y + cellSize.y * l + 1);
        ImGui::RenderFrame(bb.Min, bb.Max, lineColor, false, 0);
    }
}

#if 0
void DataPanels::doBrickPanel(const char* label, const i32 startLine)
{
    assert(brickWall);
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImRect panelRect = window->Rect();

    const i32 lineCount = (panelRect.GetHeight() - columnHeaderHeight) / rowHeight;
    const i32 itemCount = min(fileBufferSize - (i64)startLine * columnCount, lineCount * columnCount);

    ImVec2 winPos = window->DC.CursorPos;
    const ImGuiID id = window->GetID(label);
    ImGui::ItemSize(panelRect, 0);
    if(!ImGui::ItemAdd(panelRect, id))
        return;

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
        const Brick* b = brickWall->getBrick(dataOffset);

        if(b) {
            //frameColor = b->color;
        }
        else {
            u8 val = fileBuffer[dataOffset];
            u32 hex = toHexStr(val);

            ImU32 frameColor = 0xffffffff;
            f32 geyScale = val/255.f * 0.7 + 0.3;
            frameColor = ImGui::ColorConvertFloat4ToU32(ImVec4(geyScale, geyScale, geyScale, 1.0f));

            constexpr f32 textColor = 0.0f;

            const char* label = (const char*)&hex;
            const ImVec2 label_size = ImGui::CalcTextSize(label, label+2);
            const ImVec2 size = ImGui::CalcItemSize(cellSize, label_size.x, label_size.y);

            i32 column = i % columnCount;
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
                ImGui::RenderFrame(bb.Min, bb.Max, frameColor, false, 0);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(textColor, textColor, textColor, 1));
            }

            ImGui::RenderTextClipped(bb.Min, bb.Max, label, label+2,
                              &label_size, ImVec2(0.5, 0.5), &bb);
            ImGui::PopStyleColor();
        }
    }
}
#endif
