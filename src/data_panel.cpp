#include "data_panel.h"
#include "bricks.h"
#include "imgui_extended.h"
#include <stdlib.h>
#include <float.h>
#include <limits.h>

Gradient getDefaultTypeGradient(PanelType::Enum ptype)
{
    Gradient g;
    switch(ptype) {
        case PanelType::HEX: {
            g.setBounds(u8(0x00), u8(0xff));
            g.setColors(0xff4c4c4c, 0xffffffff);
        } break;

        case PanelType::ASCII: {
            g.setBounds(u8(0x00), u8(0x20));
            g.setColors(0xffcccccc, 0xffffffff);
        } break;

        case PanelType::INT8: {
            g.setBounds(i8(-127), i8(127));
            g.setColors(0xff4c4c4c, 0xffffffff);
        } break;

        case PanelType::UINT8: {
            g.setBounds(u8(0), u8(255));
            g.setColors(0xff4c4c4c, 0xffffffff);
        } break;

        case PanelType::INT16: {
            g.setBounds(i16(SHRT_MIN), i16(SHRT_MAX));
            g.setColors(0xff4c4c4c, 0xffffffff);
        } break;

        case PanelType::UINT16: {
            g.setBounds(u16(0), u16(USHRT_MAX));
            g.setColors(0xff4c4c4c, 0xffffffff);
        } break;

        case PanelType::INT32: {
            g.setBounds(i32(-100000), i32(100000));
            g.setColors(0xff4c4c4c, 0xffffffff);
        } break;

        case PanelType::UINT32: {
            g.setBounds(u32(0), u32(100000));
            g.setColors(0xff4c4c4c, 0xffffffff);
        } break;

        case PanelType::INT64: {
            g.setBounds(i32(-100000), i32(100000));
            g.setColors(0xff4c4c4c, 0xffffffff);
        } break;

        case PanelType::UINT64: {
            g.setBounds(u64(0), u64(100000));
            g.setColors(0xff4c4c4c, 0xffffffff);
        } break;

        case PanelType::FLOAT32: {
            g.setBounds(f32(-10000.0f), f32(10000.0f));
            g.setColors(0xff4c4c4c, 0xffffffff);
        } break;

        case PanelType::FLOAT64: {
            g.setBounds(f64(-10000.0), f64(10000.0));
            g.setColors(0xff4c4c4c, 0xffffffff);
        } break;

        default: assert(0); break;
    }
    return g;
}


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
    if(!(io.MouseDown[0] && selectionState.selectStart >= 0 && selectionState.lockedPanelId >= 0) &&
       (!winRect.Contains(io.MousePos) || !ImGui::IsWindowHovered())) {
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
            case PanelType::HEX:
                selectionProcessMouseInput(p, mousePos, prect, columnWidth,
                                           rowHeight, scrollCurrentLine, 1);
                break;
            case PanelType::ASCII:
                selectionProcessMouseInput(p, mousePos, prect, asciiCharWidth,
                                           rowHeight, scrollCurrentLine, 1);
                break;
            case PanelType::INT8:
            case PanelType::UINT8:
                selectionProcessMouseInput(p, mousePos, prect, intColumnWidth * 1,
                                           rowHeight, scrollCurrentLine, 1);
                break;
            case PanelType::INT16:
            case PanelType::UINT16:
                selectionProcessMouseInput(p, mousePos, prect, intColumnWidth * 2,
                                           rowHeight, scrollCurrentLine, 2);
                break;
            case PanelType::INT32:
            case PanelType::UINT32:
            case PanelType::FLOAT32:
                selectionProcessMouseInput(p, mousePos, prect, intColumnWidth * 4,
                                           rowHeight, scrollCurrentLine, 4);
                break;
            case PanelType::INT64:
            case PanelType::UINT64:
            case PanelType::FLOAT64:
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
    if(io.MouseClicked[0]) {
        selectionState.selectStart = (startLine + hoverLine) * columnCount + hoverColumn * hoverLen;
        selectionState.selectEnd = selectionState.selectStart + hoverLen-1;
        selectionState.lockedPanelId = panelId;
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
    panelType[0] = PanelType::HEX;
    panelType[1] = PanelType::ASCII;
    panelType[2] = PanelType::INT32;

    for(i32 p = 0; p < PANEL_MAX_COUNT; p++) {
        for(i32 t = 0; t < (i32)PanelType::_COUNT; t++) {
            panelParams[p].grads[t] = getDefaultTypeGradient((PanelType::Enum)t);
        }
    }

    for(i32 t = 0; t < (i32)PanelType::_COUNT; t++) {
        panelParams[3].grads[t].setColors(0xff800000, 0xff0000ff);
    }
    for(i32 t = 0; t < (i32)PanelType::_COUNT; t++) {
        panelParams[2].grads[t].setColors(0xff008000, 0xff0000ff);
    }
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

    const i32 pid = panelCount++;
    panelType[pid] = PanelType::HEX;
    panelParams[pid].colorDisplay = ColorDisplay::GRADIENT;
    for(i32 t = 0; t < (i32)PanelType::_COUNT; t++) {
        panelParams[pid].grads[t] = getDefaultTypeGradient((PanelType::Enum)t);
    }
}

void DataPanels::removePanel(const i32 pid)
{
    if(pid+1 < panelCount) {
        panelType[pid] = panelType[pid+1];
        memmove(panelType + pid, panelType + pid + 1, sizeof(panelType[0]) * (panelCount - pid - 1));
        panelParams[pid] = panelParams[pid+1];
        memmove(panelParams + pid, panelParams + pid + 1, sizeof(panelParams[0]) * (panelCount - pid - 1));
    }
    panelCount--;
}

void DataPanels::goTo(i32 offset)
{
    if(offset >= 0 && offset < fileBufferSize) {
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
            case PanelType::HEX:
                panelRectWidth[p] = columnCount * columnWidth;
                break;
            case PanelType::ASCII:
                panelRectWidth[p] = columnCount * asciiCharWidth;
                break;
            case PanelType::INT8:
            case PanelType::UINT8:
            case PanelType::INT16:
            case PanelType::UINT16:
            case PanelType::INT32:
            case PanelType::UINT32:
            case PanelType::INT64:
            case PanelType::UINT64:
            case PanelType::FLOAT32:
            case PanelType::FLOAT64:
                panelRectWidth[p] = intColumnWidth * columnCount;
                break;

           childPanelWidth += panelRectWidth[p] + panelSpacing;
        }
    }
}

void DataPanels::doRowHeader()
{
    // row header
    ImRect winRect = ImGui::GetCurrentWindow()->Rect();
    const i32 lineCount = (winRect.GetHeight() - columnHeaderHeight) / rowHeight + 1;

    char numStr[24];
    i32 strLen = sprintf(numStr, "%lld", (lineCount + scrollCurrentLine) * columnCount);
    const ImVec2 maxTextSize = ImGui::CalcTextSize(numStr, numStr+strLen);
    rowHeaderWidth = maxTextSize.x + 12;

    ImGui::ItemSize(ImVec2(rowHeaderWidth, -1));

    const ImVec2& winPos = winRect.Min;
    ImRect rowHeadBb(winPos, winPos + ImVec2(rowHeaderWidth, winRect.GetHeight()));
    const ImU32 headerColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.9, 0.9, 0.9, 1));
    const ImU32 headerColorOdd = ImGui::ColorConvertFloat4ToU32(ImVec4(0.85, 0.85, 0.85, 1));
    ImGui::RenderFrame(rowHeadBb.Min, rowHeadBb.Max, headerColor, false, 0);

    const i64 hoverStart = selectionState.hoverStart / columnCount;
    const i64 hoverEnd = (selectionState.hoverEnd-1) / columnCount;
    const bool hovered = selectionState.hoverStart >= 0;
    const i64 selectStart = selectionState.selectStart / columnCount;
    const i64 selectEnd = (selectionState.selectEnd) / columnCount;
    const bool selected = selectionState.selectStart >= 0;

    for(i64 i = 0; i < lineCount; ++i) {
        const i64 line = i + scrollCurrentLine;
        const i32 len = sprintf(numStr, "%lld", line * columnCount);
        const char* label = numStr;
        const ImVec2 label_size = ImGui::CalcTextSize(label, label+len);
        const ImVec2 size = ImGui::CalcItemSize(ImVec2(rowHeaderWidth, rowHeight),
                                                label_size.x, label_size.y);

        ImVec2 cellPos(0, rowHeight * i);
        cellPos += winPos;
        cellPos.y += columnHeaderHeight + 21;
        const ImRect bb(cellPos, cellPos + size);

        u32 frameColor = (i & 1) ? headerColorOdd : headerColor;
        u32 textColor = (i & 1) ? 0xff808080 : 0xff737373;
        if(selected && line >= selectStart && line <= selectEnd) {
            frameColor = selectedFrameColor;
            textColor = 0xffffffff;
        }
        else if(hovered && line >= hoverStart && line <= hoverEnd) {
            frameColor = hoverFrameColor;
            textColor = 0xff000000;
        }

        if(frameColor != headerColor) {
           ImGui::RenderFrame(bb.Min, bb.Max, frameColor, false, 0);
        }

        ImGui::PushStyleColor(ImGuiCol_Text, textColor);
        ImGui::RenderTextClipped(bb.Min, bb.Max, label, label+len,
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
    static i32 panelParamWindowOpenId = -1;
    static ImVec2 panelParamWindowPos;
    ImGuiIO& io = ImGui::GetIO();

    doRowHeader();
    ImGui::SameLine();

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
        inputRect.Translate(ImVec2(panelSpacing, 0));
        processMouseInput(inputRect);

        f32 offsetX = panelSpacing;
        for(i32 p = 0; p < panelCount; ++p) {
            const f32 panelWidth = panelRectWidth[p];

            ImVec2 csp;
            csp.x = window->Pos.x - window->Scroll.x + offsetX;
            csp.y = window->Rect().Min.y;
            ImGui::SetCursorScreenPos(csp);
            offsetX += panelWidth + panelSpacing;
            ImGui::BeginGroup();

            const f32 comboWidth = panelWidth - panelCloseButtonWidth - panelColorButtonWidth;
            ImGui::PushItemWidth(comboWidth);
            ImGui::PushID(&panelType[p]);
            ImGui::Combo("##PanelType", (i32*)&panelType[p], panelComboItems, IM_ARRAYSIZE(panelComboItems),
                         IM_ARRAYSIZE(panelComboItems));
            ImGui::PopID();
            ImGui::PopItemWidth();

            ImGui::SameLine();

            char paramButStr[24];
            char paramPopupStr[32];
            sprintf(paramButStr, "P##%d", p);
            sprintf(paramPopupStr, "PanelParamsPopup%d", p);

            // TODO: make toggle button
            if(ImGui::ButtonEx(paramButStr, ImVec2(panelColorButtonWidth, 0))) {
                if(panelParamWindowOpenId != p) {
                    panelParamWindowOpenId = p;
                    panelParamWindowPos = ImGui::GetCursorScreenPos() + ImVec2(comboWidth, 0);
                }
                else {
                    panelParamWindowOpenId = -1;
                }
            }

            ImGui::SameLine();

            ImGui::PushID(&panelType[p] + 0xb00b + 'X');
            if(ImGui::Button("X", ImVec2(panelCloseButtonWidth, 0))) {
                panelMarkedForDelete = p;
            }
            ImGui::PopID();

            ImGui::EndGroup();

            csp.y += panelHeaderHeight;
            ImGui::SetCursorScreenPos(csp);

            switch(panelType[p]) {
                case PanelType::HEX:
                    doHexPanel(p, panelWidth, scrollCurrentLine);
                    break;
                case PanelType::ASCII:
                    doAsciiPanel(p, panelWidth, scrollCurrentLine);
                    break;
                case PanelType::INT8:
                    doFormatPanel<i8>(p, panelWidth, scrollCurrentLine, "%d");
                    break;
                case PanelType::UINT8:
                    doFormatPanel<u8>(p, panelWidth, scrollCurrentLine, "%u");
                    break;
                case PanelType::INT16:
                    doFormatPanel<i16>(p, panelWidth, scrollCurrentLine, "%d");
                    break;
                case PanelType::UINT16:
                    doFormatPanel<u16>(p, panelWidth, scrollCurrentLine, "%u");
                    break;
                case PanelType::INT32:
                    doFormatPanel<i32>(p, panelWidth, scrollCurrentLine, "%d");
                    break;
                case PanelType::UINT32:
                    doFormatPanel<u32>(p, panelWidth, scrollCurrentLine, "%u");
                    break;
                case PanelType::INT64:
                    doFormatPanel<i64>(p, panelWidth, scrollCurrentLine, "%lld");
                    break;
                case PanelType::UINT64:
                    doFormatPanel<u64>(p, panelWidth, scrollCurrentLine, "%llu");
                    break;
                case PanelType::FLOAT32:
                    doFormatPanel<f32>(p, panelWidth, scrollCurrentLine, "%g");
                    break;
                case PanelType::FLOAT64:
                    doFormatPanel<f64>(p, panelWidth, scrollCurrentLine, "%g");
                    break;
                default:
                    assert(0);
                    break;
            }


            ImGui::SameLine();
            ImGui::ItemSize(ImVec2(panelSpacing, -1));

            if(p+1 < panelCount) {
                ImGui::SameLine();
            }
        }

    ImGui::EndChild();


    if(panelParamWindowOpenId != -1) {
        const i32 p = panelParamWindowOpenId;
        ImGui::SetNextWindowPos(panelParamWindowPos);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 2);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));

        if(ImGui::Begin("Panel parameters", nullptr,
                        ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_AlwaysAutoResize)) {
            ColorDisplay::Enum& colorDisplay = panelParams[p].colorDisplay;
            LOG("POPUP");
            ImGui::Text("Color display:");
            static bool butPlain, butGradient, butBricks;
            butPlain = colorDisplay == ColorDisplay::PLAIN;
            butGradient = colorDisplay == ColorDisplay::GRADIENT;
            butBricks = colorDisplay == ColorDisplay::BRICK_COLOR;

            if(ImGui::Selectable("Plain", &butPlain, 0, ImVec2(50,20))) {
                colorDisplay = ColorDisplay::PLAIN;
            } ImGui::SameLine();
            if(ImGui::Selectable("Gradient", &butGradient, 0, ImVec2(50,20))) {
                colorDisplay = ColorDisplay::GRADIENT;
            } ImGui::SameLine();
            if(ImGui::Selectable("Bricks", &butBricks, 0, ImVec2(50,20))) {
                colorDisplay = ColorDisplay::BRICK_COLOR;
            };

            ImGui::Separator();

            ImGui::Text("Gradient:");

            const PanelType::Enum ptype = panelType[p];
            Gradient& grad = panelParams[p].grads[ptype];

            switch(ptype) {
                case PanelType::ASCII:
                case PanelType::UINT8:
                case PanelType::HEX: {
                    static i32 imin, imax;
                    imin = *(u8*)&grad.gmin;
                    imax = *(u8*)&grad.gmax;
                    if(ImGui::DragInt("min", &imin, 1.0f, 0, 255)) {
                        *(u8*)&grad.gmin = min(imin, imax-1);
                    }
                    if(ImGui::DragInt("max", &imax, 1.0f, imin+1, 255)) {
                        *(u8*)&grad.gmax = max(imin+1, imax);
                    }
                } break;

                case PanelType::INT8: {
                    static i32 imin, imax;
                    imin = *(i8*)&grad.gmin;
                    imax = *(i8*)&grad.gmax;
                    if(ImGui::DragInt("min", &imin, 1.0f, -127, 127)) {
                        *(i8*)&grad.gmin = min(imin, imax-1);
                    }
                    if(ImGui::DragInt("max", &imax, 1.0f, imin+1, 127)) {
                        *(i8*)&grad.gmax = max(imin+1, imax);
                    }
                } break;

                case PanelType::INT16: {
                    static i32 imin, imax;
                    imin = *(i16*)&grad.gmin;
                    imax = *(i16*)&grad.gmax;
                    if(ImGui::DragInt("min", &imin, 1.0f, SHRT_MIN, SHRT_MAX)) {
                        *(i16*)&grad.gmin = min(imin, imax-1);
                    }
                    if(ImGui::DragInt("max", &imax, 1.0f, imin+1, SHRT_MAX)) {
                        *(i16*)&grad.gmax = max(imin+1, imax);
                    }
                } break;

                case PanelType::UINT16: {
                    static i32 imin, imax;
                    imin = *(u16*)&grad.gmin;
                    imax = *(u16*)&grad.gmax;
                    if(ImGui::DragInt("min", &imin, 1.0f, 0, USHRT_MAX)) {
                        *(u16*)&grad.gmin = min(imin, imax-1);
                    }
                    if(ImGui::DragInt("max", &imax, 1.0f, imin+1, USHRT_MAX)) {
                        *(u16*)&grad.gmax = max(imin+1, imax);
                    }
                } break;

                case PanelType::INT32: {
                    static i32 imin, imax;
                    imin = *(i32*)&grad.gmin;
                    imax = *(i32*)&grad.gmax;
                    if(ImGui::DragInt("min", &imin, 1.0f, _I32_MIN, _I32_MAX)) {
                        *(i32*)&grad.gmin = min(imin, imax-1);
                    }
                    if(ImGui::DragInt("max", &imax, 1.0f, imin+1, _I32_MAX)) {
                        *(i32*)&grad.gmax = max(imin+1, imax);
                    }
                } break;

                case PanelType::UINT32: {
                    // TODO: use DragUint32 (new imgui version)
                    static i32 imin, imax;
                    imin = *(u32*)&grad.gmin;
                    imax = *(u32*)&grad.gmax;
                    if(ImGui::DragInt("min", &imin, 1.0f, 0, _I32_MAX)) {
                        *(u32*)&grad.gmin = min(imin, imax-1);
                    }
                    if(ImGui::DragInt("max", &imax, 1.0f, imin+1, _I32_MAX)) {
                        *(u32*)&grad.gmax = max(imin+1, imax);
                    }
                } break;

                case PanelType::INT64: {
                    static i32 imin, imax;
                    imin = *(i32*)&grad.gmin;
                    imax = *(i32*)&grad.gmax;
                    if(ImGui::DragInt("min", &imin, 1.0f, _I32_MIN, _I32_MAX)) {
                        *(i32*)&grad.gmin = min(imin, imax-1);
                    }
                    if(ImGui::DragInt("max", &imax, 1.0f, imin+1, _I32_MAX)) {
                        *(i32*)&grad.gmax = max(imin+1, imax);
                    }
                } break;

                case PanelType::UINT64: {
                    static i32 imin, imax;
                    imin = *(u32*)&grad.gmin;
                    imax = *(u32*)&grad.gmax;
                    if(ImGui::DragInt("min", &imin, 1.0f, 0, _I32_MAX)) {
                        *(u32*)&grad.gmin = min(imin, imax-1);
                    }
                    if(ImGui::DragInt("max", &imax, 1.0f, imin+1, _I32_MAX)) {
                        *(u32*)&grad.gmax = max(imin+1, imax);
                    }
                } break;

                case PanelType::FLOAT32: {
                    static f32 imin, imax;
                    imin = *(f32*)&grad.gmin;
                    imax = *(f32*)&grad.gmax;
                    if(ImGui::DragFloat("min", &imin, 1.0f, -FLT_MAX, FLT_MAX)) {
                        *(f32*)&grad.gmin = min(imin, imax-1);
                    }
                    if(ImGui::DragFloat("max", &imax, 1.0f, imin+1, _I32_MAX)) {
                        *(f32*)&grad.gmax = max(imin+1, imax);
                    }
                } break;

                case PanelType::FLOAT64: {
                    static f32 imin, imax;
                    imin = *(f64*)&grad.gmin;
                    imax = *(f64*)&grad.gmax;
                    if(ImGui::DragFloat("min", &imin, 1.0f, -FLT_MAX, FLT_MAX)) {
                        *(f64*)&grad.gmin = min(imin, imax-1);
                    }
                    if(ImGui::DragFloat("max", &imax, 1.0f, imin+1, FLT_MAX)) {
                        *(f64*)&grad.gmax = max(imin+1, imax);
                    }
                } break;
            }

            static Color3 gradCol1;
            static Color3 gradCol2;
            Color3& rgb1 = panelParams[p].grads[ptype].color1;
            Color3& rgb2 = panelParams[p].grads[ptype].color2;
            gradCol1 = rgb1;
            gradCol2 = rgb2;

            if(ImGui::ColorEdit3("Color1", gradCol1.data)) {
                rgb1 = gradCol1;
            }
            if(ImGui::ColorEdit3("Color2", gradCol2.data)) {
                rgb2 = gradCol2;
            }

            if(ImGui::Button("Reset min/max", ImVec2(100, 25))) {
                Gradient defGrad = getDefaultTypeGradient(ptype);
                grad.gmin = defGrad.gmin;
                grad.gmax = defGrad.gmax;
            }

            ImGui::SameLine();

            if(ImGui::Button("Reset colors", ImVec2(100, 25))) {
                Gradient defGrad = getDefaultTypeGradient(ptype);
                rgb1 = defGrad.color1;
                rgb2 = defGrad.color2;
            }

            ImGui::Separator();

            if(ImGui::Button("Ok", ImVec2(300, 25))) {
                panelParamWindowOpenId = -1;
            }

            if(!ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows)) {
                panelParamWindowOpenId = -1;
            }

            ImGui::End();
        }

        ImGui::PopStyleVar(4);
    }

    if(panelMarkedForDelete >= 0 && panelCount > 1) {
        removePanel(panelMarkedForDelete);
    }
}

void DataPanels::doHexPanel(i32 pid, f32 panelWidth, const i32 startLine)
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

    const ColorDisplay::Enum colorDisplay = panelParams[pid].colorDisplay;
    const Gradient grad = panelParams[pid].grads[panelType[pid]];

    // hex table
    const i64 startDataOff = startLine * columnCount;
    for(i64 i = 0; i < itemCount; ++i) {
        const i64 dataOffset = i + startDataOff;
        u8 val = fileBuffer[dataOffset];
        u32 hex = toHexStr(val);

        ImU32 frameColor = 0xffffffff;
        ImU32 textColor = 0xff000000;

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
            const f32 ga = grad.getLerpVal(val);

            const Color3 rgbCol = grad.lerpColor(ga);
            const f32 brightness = rgbGetBrightness(rgbCol);
            const u32 gradientColor = rgbToU32(rgbCol);

            u32 fixedTextColor = textColor;
            if(brightness < 0.25) {
                fixedTextColor = rgbToU32(rgbGetLighterColor(rgbCol, 0.6f));
            }

            if(colorDisplay == ColorDisplay::GRADIENT) {
                frameColor = gradientColor;
                if(brightness < 0.25) {
                    textColor = fixedTextColor;
                }
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
                else { // fallback to gradient
                    frameColor = gradientColor;
                    if(brightness < 0.25) {
                        textColor = fixedTextColor;
                    }
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

void DataPanels::doAsciiPanel(i32 pid, f32 panelWidth, const i32 startLine)
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

    const ColorDisplay::Enum colorDisplay = panelParams[pid].colorDisplay;
    const Gradient grad = panelParams[pid].grads[panelType[pid]];

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


        ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);
        const bool isHovered = selectionInHoverRange(dataOff);
        const bool isSelected = selectionInSelectionRange(dataOff);

        ImU32 frameColor = 0xffffffff;

        const f32 ga = grad.getLerpVal((u8)c);

        const Color3 rgbCol = grad.lerpColor(ga);
        const f32 brightness = rgbGetBrightness(rgbCol);
        const u32 gradientColor = rgbToU32(rgbCol);
        if(brightness < 0.25) {
            textColor = rgbToU32(rgbGetLighterColor(rgbCol, 0.6f));
        }

        if(colorDisplay == ColorDisplay::BRICK_COLOR) {
            assert(brickWall);
            const Brick* b = brickWall->getBrick(dataOff);
            if(b) {
                frameColor = b->color;
            }
            else {
                frameColor = gradientColor;
            }
        }
        else if(colorDisplay == ColorDisplay::GRADIENT) {
            frameColor = gradientColor;
        }

        if(isSelected) {
            frameColor = selectedFrameColor;
        }
        else if(isHovered) {
            frameColor = hoverFrameColor;
        }

        ImGui::RenderFrame(bb.Min, bb.Max, frameColor, false);

        if((u8)c >= 0x20) {
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

    const ColorDisplay::Enum colorDisplay = panelParams[pid].colorDisplay;
    const Gradient grad = panelParams[pid].grads[panelType[pid]];

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
        ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);

        if(colorDisplay == ColorDisplay::GRADIENT) {
            T val = *(T*)&fileBuffer[dataOff];
            if(!(val == val)) { // check for NaN
                val = 0;
            }
            const f32 ga = grad.getLerpVal(val);

            const Color3 rgbCol = grad.lerpColor(ga);
            const f32 brightness = rgbGetBrightness(rgbCol);
            const u32 gradientColor = rgbToU32(rgbCol);
            u32 fixedTextColor = textColor;

            if(brightness < 0.25) {
                fixedTextColor = rgbToU32(rgbGetLighterColor(rgbCol, 0.6f));
            }

            frameColor = gradientColor;
            textColor = fixedTextColor;
        }

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
