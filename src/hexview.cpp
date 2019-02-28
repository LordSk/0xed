#include "hexview.h"
#include "bricks.h"
#include "imgui_extended.h"
#include <stdlib.h>
#include <float.h>
#include <limits.h>

GradientRange getDefaultTypeGradientRange(PanelType::Enum ptype)
{
    GradientRange g;
    switch(ptype) {
        case PanelType::HEX: {
            g.setBounds(u8(0x00), u8(0xff));
        } break;

        case PanelType::ASCII: {
            g.setBounds(u8(0x00), u8(0x20));
        } break;

        case PanelType::INT8: {
            g.setBounds(i8(-127), i8(127));
        } break;

        case PanelType::UINT8: {
            g.setBounds(u8(0), u8(255));
        } break;

        case PanelType::INT16: {
            g.setBounds(i16(SHRT_MIN), i16(SHRT_MAX));
        } break;

        case PanelType::UINT16: {
            g.setBounds(u16(0), u16(USHRT_MAX));
        } break;

        case PanelType::INT32: {
            g.setBounds(i32(-100000), i32(100000));
        } break;

        case PanelType::UINT32: {
            g.setBounds(u32(0), u32(100000));
        } break;

        case PanelType::INT64: {
            g.setBounds(i32(-100000), i32(100000));
        } break;

        case PanelType::UINT64: {
            g.setBounds(u64(0), u64(100000));
        } break;

        case PanelType::FLOAT32: {
            g.setBounds(f32(-10000.0f), f32(10000.0f));
        } break;

        case PanelType::FLOAT64: {
            g.setBounds(f64(-10000.0), f64(10000.0));
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

HexView::HexView()
{
    memset(panelType, 0, sizeof(panelType));
    panelType[0] = PanelType::HEX;
    panelType[1] = PanelType::ASCII;

    for(i32 p = 0; p < PANEL_MAX_COUNT; p++) {
        panelParams[p].gradSetDefaultColors();
        for(i32 t = 0; t < (i32)PanelType::_COUNT; t++) {
            panelParams[p].gradRange[t] = getDefaultTypeGradientRange((PanelType::Enum)t);
        }
    }

    // ascii panel
    panelParams[1].gradSetColors(0xffcccccc, 0xffffffff);

    for(i32 t = 0; t < (i32)PanelType::_COUNT; t++) {
        panelParams[3].gradSetColors(0xff800000, 0xff0000ff);
    }
    for(i32 t = 0; t < (i32)PanelType::_COUNT; t++) {
        panelParams[2].gradSetColors(0xff008000, 0xff0000ff);
    }
}

void HexView::setFileBuffer(u8* buff, i64 buffSize)
{
    fileBuffer = buff;
    fileBufferSize = buffSize;
    selectionState = {};
    scrollCurrentLine = 0;
}

void HexView::addNewPanel()
{
    if(panelCount >= PANEL_MAX_COUNT) {
        return;
    }

    const i32 pid = panelCount++;
    panelType[pid] = PanelType::HEX;
    panelParams[pid].colorDisplay = ColorDisplay::GRADIENT;
    for(i32 t = 0; t < (i32)PanelType::_COUNT; t++) {
        panelParams[pid].gradRange[t] = getDefaultTypeGradientRange((PanelType::Enum)t);
    }
}

void HexView::removePanel(const i32 pid)
{
    if(pid+1 < panelCount) {
        panelType[pid] = panelType[pid+1];
        memmove(panelType + pid, panelType + pid + 1, sizeof(panelType[0]) * (panelCount - pid - 1));
        panelParams[pid] = panelParams[pid+1];
        memmove(panelParams + pid, panelParams + pid + 1, sizeof(panelParams[0]) * (panelCount - pid - 1));
    }
    panelCount--;
}

void HexView::goTo(i32 offset)
{
    if(offset >= 0 && offset < fileBufferSize) {
        goToLine = offset / columnCount;
    }
}

i32 HexView::getSelectedInt()
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

void HexView::doUiHexViewWindow()
{
	ImGuiIO& io = ImGui::GetIO();

	// clear selection (on right click)
	if(io.MouseClicked[1]) {
		selectionState.deselect();
		return;
	}

	bool mouseInsideAnyPanel = false;

    if(!fileBuffer) {
        return;
    }

    const i32 totalLineCount = fileBufferSize/columnCount + 4;
    i32 panelMarkedForDelete = -1;
    const f32 panelHeaderHeight = ImGui::GetComboHeight();
    static i32 panelParamWindowOpenId = -1;
    static ImVec2 panelParamWindowPos;
    bool openPanelParamPopup = false;

	f32 panelRectWidth[PANEL_MAX_COUNT];
	for(i32 p = 0; p < panelCount; ++p)
		panelRectWidth[p] = hexViewCalculatePanelWidth(panelType[p], columnCount);

	const UiStyle& style = setUiStyle();

	// window begin
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Hex view");
	ImGui::PopStyleVar(1);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	uiHexRowHeader(scrollCurrentLine, columnCount, style.columnHeaderHeight + panelHeaderHeight,
				   selectionState);

    ImGui::SameLine();

	// force appropriate content size
	f32 windowWidth = style.panelSpacing + ImGui::GetStyle().ScrollbarSize;
	for(i32 p = 0; p < panelCount; ++p)
		windowWidth += panelRectWidth[p] + style.panelSpacing;

	ImGui::SetNextWindowContentSize(ImVec2(windowWidth, totalLineCount * style.rowHeight));
	ImGui::BeginChild("#child_panel", ImVec2(0, 0), false,
					  ImGuiWindowFlags_HorizontalScrollbar);

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if(goToLine != -1) {
			ImGui::SetScrollY(goToLine * style.rowHeight);
			goToLine = -1;
		}
		scrollCurrentLine = window->Scroll.y / style.rowHeight;

		/*ImRect inputRect = window->Rect();
		inputRect.Min.y += panelHeaderHeight;
		inputRect.Translate(ImVec2(panelSpacing, 0));
		processMouseInput(inputRect);*/

		// TODO: report bug? Columns don't increase content size even when width is forced to overflow
		// Thus no horitontal scrollbar is displayed. Width has to be forced beforehand.
		// NOTE: 1st column is spacing, second is panel, and it repeats
		ImGui::Columns(panelCount * 2, nullptr, false);

		for(i32 p = 0; p < panelCount; ++p) {
			const f32 panelWidth = panelRectWidth[p];
			ImGui::SetColumnWidth(p * 2, style.panelSpacing);
			ImGui::SetColumnWidth(p * 2 + 1, panelWidth);
		}

		// panel headers
		for(i32 p = 0; p < panelCount; ++p) {
			ImGui::NextColumn(); // skip spacing column

			ImVec2 csp = ImGui::GetCursorScreenPos();
			csp.y = window->Rect().Min.y + 1;
			ImGui::SetCursorScreenPos(csp);

			const f32 comboWidth = ImGui::GetContentRegionAvailWidth() -
								   style.panelCloseButtonWidth - style.panelColorButtonWidth - 1;
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
			if(ImGui::ButtonEx(paramButStr, ImVec2(style.panelColorButtonWidth, 0))) {
				if(panelParamWindowOpenId != p) {
					panelParamWindowOpenId = p;
					panelParamWindowPos = ImGui::GetCursorScreenPos() + ImVec2(comboWidth, 0);
					openPanelParamPopup = true;
				}
				else {
					panelParamWindowOpenId = -1;
				}
			}

			ImGui::SameLine();

			ImGui::PushID(&panelType[p] + 0xb00b + 'X');
			if(ImGui::Button("X", ImVec2(style.panelCloseButtonWidth, 0))) {
				panelMarkedForDelete = p;
			}
			ImGui::PopID();

			ImGui::NextColumn();
		}

		for(i32 p = 0; p < panelCount; ++p) {
			ImGui::NextColumn(); // skip spacing column
			uiHexColumnHeader(columnCount, selectionState);
			ImGui::NextColumn();
		}

		// reset hover state
		selectionState.hoverStart = -1;
		// process mouse input over panels before displaying them
		for(i32 p = 0; p < panelCount; ++p) {
			ImGui::NextColumn(); // skip spacing column
			mouseInsideAnyPanel |= uiHexPanelDoSelection(p, panelType[p], &selectionState, scrollCurrentLine, columnCount);
			ImGui::NextColumn();
		}

		// data panels
		for(i32 p = 0; p < panelCount; ++p) {
			ImGui::NextColumn(); // skip spacing column

			switch(panelType[p]) {
				case PanelType::HEX:
					uiHexDoHexPanel(p, panelParams[p], scrollCurrentLine, fileBuffer, fileBufferSize, columnCount, selectionState);
					break;
				case PanelType::ASCII:
					uiHexDoAsciiPanel(p, panelParams[p], scrollCurrentLine, fileBuffer, fileBufferSize, columnCount, selectionState);
					break;
				case PanelType::INT8:
					uiHexDoFormatPanel<i8>(p, panelParams[p], scrollCurrentLine, fileBuffer, fileBufferSize, columnCount, selectionState, PanelType::INT8, "%d");
					break;
				case PanelType::UINT8:
					uiHexDoFormatPanel<u8>(p, panelParams[p], scrollCurrentLine, fileBuffer, fileBufferSize, columnCount, selectionState, PanelType::INT8, "%u");
					break;
				case PanelType::INT16:
					uiHexDoFormatPanel<i16>(p, panelParams[p], scrollCurrentLine, fileBuffer, fileBufferSize, columnCount, selectionState, PanelType::INT16, "%d");
					break;
				case PanelType::UINT16:
					uiHexDoFormatPanel<u16>(p, panelParams[p], scrollCurrentLine, fileBuffer, fileBufferSize, columnCount, selectionState, PanelType::UINT16, "%u");
					break;
				case PanelType::INT32:
					uiHexDoFormatPanel<i32>(p, panelParams[p], scrollCurrentLine, fileBuffer, fileBufferSize, columnCount, selectionState, PanelType::INT32, "%d");
					break;
				case PanelType::UINT32:
					uiHexDoFormatPanel<u32>(p, panelParams[p], scrollCurrentLine, fileBuffer, fileBufferSize, columnCount, selectionState, PanelType::UINT32, "%u");
					break;
				case PanelType::INT64:
					uiHexDoFormatPanel<i64>(p, panelParams[p], scrollCurrentLine, fileBuffer, fileBufferSize, columnCount, selectionState, PanelType::INT64, "%lld");
					break;
				case PanelType::UINT64:
					uiHexDoFormatPanel<u64>(p, panelParams[p], scrollCurrentLine, fileBuffer, fileBufferSize, columnCount, selectionState, PanelType::UINT64, "%llu");
					break;
				case PanelType::FLOAT32:
					uiHexDoFormatPanel<f32>(p, panelParams[p], scrollCurrentLine, fileBuffer, fileBufferSize, columnCount, selectionState, PanelType::FLOAT32, "%g");
					break;
				case PanelType::FLOAT64:
					uiHexDoFormatPanel<f64>(p, panelParams[p], scrollCurrentLine, fileBuffer, fileBufferSize, columnCount, selectionState, PanelType::FLOAT64, "%g");
					break;
				default:
					assert(0);
					break;
			}

			ImGui::NextColumn();
		}

	ImGui::EndChild();
	ImGui::PopStyleVar(1);

    doPanelParamPopup(openPanelParamPopup, &panelParamWindowOpenId, panelParamWindowPos);

    if(panelMarkedForDelete >= 0 && panelCount > 1) {
        removePanel(panelMarkedForDelete);
    }

	// FIXME: clicking outside the hex view window will deselect
	/*if(!mouseInsideAnyPanel && io.MouseClicked[0]) {
		selectionState.deselect();
	}*/

	ImGui::End(); // window end
}

void HexView::doPanelParamPopup(bool open, i32* panelId, ImVec2 popupPos)
{
    if(open && *panelId != -1) {
        ImGui::OpenPopup("Panel parameters");
    }
    ImGui::SetNextWindowPos(popupPos);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5);
    ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 2);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));

    if(ImGui::BeginPopup("Panel parameters",
                    ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_AlwaysAutoResize)) {
        const i32 p = *panelId;
        ImGui::Text("Color display:");

        ColorDisplay::Enum& colorDisplay = panelParams[p].colorDisplay;

        const char* colorTypeSelectList[] = {
            "Gradient",
            "Plain",
            "Bricks",
            "Search",
        };

        ImGui::ButtonListOne("color_type_select", colorTypeSelectList,
                                arr_count(colorTypeSelectList), (i32*)&colorDisplay);

        ImGui::Separator();

        ImGui::Text("Gradient:");

        const PanelType::Enum ptype = panelType[p];
        GradientRange& grad = panelParams[p].gradRange[ptype];

        switch(ptype) {
            case PanelType::ASCII:
            case PanelType::UINT8:
            case PanelType::HEX: {
                const u32 u8Min = 0;
                const u32 u8Max = 255;
                static u32 imin, imax;
                imin = *(u8*)&grad.gmin;
                imax = *(u8*)&grad.gmax;
                if(ImGui::DragScalar("min", ImGuiDataType_U32, &imin, 1.0f, &u8Min, &u8Max)) {
                    *(u8*)&grad.gmin = min(imin, imax-1);
                }
                if(ImGui::DragScalar("max", ImGuiDataType_U32, &imax, 1.0f, &u8Min, &u8Max)) {
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
                const u32 u16Min = 0;
                const u32 u16Max = USHRT_MAX;

                static u32 imin, imax;
                imin = *(u16*)&grad.gmin;
                imax = *(u16*)&grad.gmax;
                if(ImGui::DragScalar("min", ImGuiDataType_U32, &imin, 1.0f, &u16Min, &u16Max)) {
                    *(u16*)&grad.gmin = min(imin, imax-1);
                }
                if(ImGui::DragScalar("max", ImGuiDataType_U32, &imax, 1.0f, &u16Min, &u16Max)) {
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
                static u32 imin, imax;
                imin = *(u32*)&grad.gmin;
                imax = *(u32*)&grad.gmax;
                if(ImGui::DragScalar("min", ImGuiDataType_U32, &imin, 1.0f)) {
                    *(u32*)&grad.gmin = min(imin, imax-1);
                }
                if(ImGui::DragScalar("max", ImGuiDataType_U32,  &imax, 1.0f)) {
                    *(u32*)&grad.gmax = max(imin+1, imax);
                }
            } break;

            case PanelType::INT64: {
                const i64 i64Min = _I64_MIN;
                const i64 i64Max = _I64_MAX;

                static i64 imin, imax;
                imin = *(i64*)&grad.gmin;
                imax = *(i64*)&grad.gmax;
                if(ImGui::DragScalar("min", ImGuiDataType_S64, &imin, 1.0f, &i64Min, &i64Max, "%lld")) {
                    *(i64*)&grad.gmin = min(imin, imax-1);
                }
                if(ImGui::DragScalar("max", ImGuiDataType_S64, &imax, 1.0f, &i64Min, &i64Max, "%lld")) {
                    *(i64*)&grad.gmax = max(imin+1, imax);
                }
            } break;

            case PanelType::UINT64: {
                const i64 u64Min = 0;
                const i64 u64Max = _UI64_MAX;

                static u64 imin, imax;
                imin = *(u64*)&grad.gmin;
                imax = *(u64*)&grad.gmax;
                if(ImGui::DragScalar("min", ImGuiDataType_U64, &imin, 1.0f, &u64Min, &u64Max, "%llu")) {
                    *(u64*)&grad.gmin = min(imin, imax-1);
                }
                if(ImGui::DragScalar("max", ImGuiDataType_U64, &imax, 1.0f, &u64Min, &u64Max, "%llu")) {
                    *(u64*)&grad.gmax = max(imin+1, imax);
                }
            } break;

            case PanelType::FLOAT32: {
                static f32 imin, imax;
                imin = *(f32*)&grad.gmin;
                imax = *(f32*)&grad.gmax;
                if(ImGui::DragFloat("min", &imin, 1.0f, -FLT_MAX, FLT_MAX)) {
                    *(f32*)&grad.gmin = min(imin, imax-1);
                }
                if(ImGui::DragFloat("max", &imax, 1.0f, imin+1, FLT_MAX)) {
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
        Color3& rgb1 = panelParams[p].gradColor1;
        Color3& rgb2 = panelParams[p].gradColor2;
        gradCol1 = rgb1;
        gradCol2 = rgb2;

        if(ImGui::ColorEdit3("Color1", gradCol1.data)) {
            rgb1 = gradCol1;
        }
        if(ImGui::ColorEdit3("Color2", gradCol2.data)) {
            rgb2 = gradCol2;
        }

        if(ImGui::Button("Reset min/max", ImVec2(100, 25))) {
            GradientRange defGrad = getDefaultTypeGradientRange(ptype);
            grad.gmin = defGrad.gmin;
            grad.gmax = defGrad.gmax;
        }

        ImGui::SameLine();

        if(ImGui::Button("Reset colors", ImVec2(100, 25))) {
            panelParams[p].gradSetDefaultColors();
        }

        ImGui::Separator();

        if(ImGui::Button("Ok", ImVec2(300, 25))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    else {
        *panelId = -1;
    }

    ImGui::PopStyleVar(4);
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

UiStyle* g_uiStyle = nullptr;

void setUiStyleLight(ImFont* asciiFont)
{
	static UiStyle style;
	style.fontAscii = asciiFont;
	g_uiStyle = &style;
}

void uiHexRowHeader(i64 firstRow, i32 rowStep, f32 textOffsetY, const SelectionState& selection)
{
	const UiStyle& style = setUiStyle();

	// row header
	const ImVec2 winPos = ImGui::GetCurrentWindow()->DC.CursorPos;
	const float winHeight = ImGui::GetCurrentWindow()->Rect().GetHeight();
	const i32 lineCount = (winHeight - textOffsetY) / style.rowHeight + 1;

	char numStr[24];
	i32 strLen = sprintf(numStr, "%lld", (lineCount + firstRow) * rowStep);
	const ImVec2 maxTextSize = ImGui::CalcTextSize(numStr, numStr+strLen);
	f32 rowHeaderWidth = maxTextSize.x + 12;

	ImGui::ItemSize(ImVec2(rowHeaderWidth, -1));

	ImRect rowHeadBb(winPos, winPos + ImVec2(rowHeaderWidth, winHeight));
	ImGui::RenderFrame(rowHeadBb.Min, rowHeadBb.Max, style.headerBgColorEven, false, 0);

	const i64 hoverStart = min(selection.hoverStart, selection.hoverEnd) / rowStep;
	const i64 hoverEnd = (max(selection.hoverStart, selection.hoverEnd)-1) / rowStep;
	const bool hovered = selection.hoverStart >= 0;
	const i64 selectStart = min(selection.selectStart, selection.selectEnd) / rowStep;
	const i64 selectEnd = max(selection.selectStart, selection.selectEnd) / rowStep;
	const bool selected = selection.selectStart >= 0;

	for(i64 i = 0; i < lineCount; ++i) {
		const i64 line = i + firstRow;
		const i32 len = sprintf(numStr, "%lld", line * rowStep);
		const char* label = numStr;
		const ImVec2 label_size = ImGui::CalcTextSize(label, label+len);
		const ImVec2 size = ImGui::CalcItemSize(ImVec2(rowHeaderWidth, style.rowHeight),
												label_size.x, label_size.y);

		ImVec2 cellPos(0, style.rowHeight * i);
		cellPos += winPos;
		cellPos.y += textOffsetY;
		const ImRect bb(cellPos, cellPos + size);

		const bool isOdd = i & 1;

		u32 frameColor = isOdd ? style.headerBgColorOdd : style.headerBgColorEven;
		u32 textColor = isOdd ? style.headerTextColorOdd : style.headerTextColorEven;

		if(selected && line >= selectStart && line <= selectEnd) {
			frameColor = style.selectedFrameColor;
			textColor = style.selectedTextColor;
		}
		else if(hovered && line >= hoverStart && line <= hoverEnd) {
			frameColor = style.hoverFrameColor;
			textColor = style.textColor;
		}

		if(frameColor != style.headerTextColorEven) {
		   ImGui::RenderFrame(bb.Min, bb.Max, frameColor, false, 0);
		}

		ImGui::PushStyleColor(ImGuiCol_Text, textColor);
		ImGui::RenderTextClipped(bb.Min, bb.Max, label, label+len,
						  &label_size, ImVec2(0.3, 0.5), &bb);

		ImGui::PopStyleColor();
	}
}

void uiHexColumnHeader(i32 columnCount, const SelectionState& selection)
{
	const UiStyle& style = setUiStyle();

	i32 hoverStart = min(selection.hoverStart, selection.hoverEnd);
	i32 hoverEnd = (max(selection.hoverStart, selection.hoverEnd)-1);
	const i32 hoverSize = hoverEnd - hoverStart;
	hoverStart = hoverStart % columnCount;
	hoverEnd = hoverStart + hoverSize;
	if(hoverEnd > columnCount)
		hoverStart = -1;
	const bool hovered = hoverStart >= 0;
	i32 selectStart = min(selection.selectStart, selection.selectEnd);
	i32 selectEnd = max(selection.selectStart, selection.selectEnd);
	const i32 selectSize = selectEnd - selectStart;
	selectStart = selectStart % columnCount;
	selectEnd = selectStart + selectSize;
	if(selectEnd > columnCount)
		selectStart = -1;
	const bool selected = selectStart >= 0;

	ImVec2 headerPos =  ImGui::GetCursorScreenPos();
	const f32 width = ImGui::GetContentRegionAvailWidth();
	const f32 columnWidth = width / columnCount;
	ImRect colHeadBb(headerPos, headerPos + ImVec2(width, style.columnHeaderHeight));
	const ImU32 headerColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.9, 0.9, 0.9, 1));
	const ImU32 headerColorOdd = ImGui::ColorConvertFloat4ToU32(ImVec4(0.85, 0.85, 0.85, 1));
	ImGui::ItemSize(colHeadBb);
	ImGui::RenderFrame(colHeadBb.Min, colHeadBb.Max, headerColor, false, 0);

	for(i32 i = 0; i < columnCount; ++i) {
		const u32 hex = toHexStr(i);
		const char* label = (const char*)&hex;
		const ImVec2 label_size = ImGui::CalcTextSize(label, label+2);
		const ImVec2 size = ImGui::CalcItemSize(ImVec2(columnWidth, style.columnHeaderHeight),
												label_size.x, label_size.y);

		ImVec2 cellPos(i * columnWidth, 0);
		cellPos += headerPos;
		const ImRect bb(cellPos, cellPos + size);

		const bool isOdd = i & 1;
		u32 frameColor = isOdd ? style.headerBgColorOdd : style.headerBgColorEven;
		u32 textColor = isOdd ? style.headerTextColorOdd : style.headerTextColorEven;

#if 0
		if(selected && i >= selectStart && i <= selectEnd) {
			frameColor = style.selectedFrameColor;
			textColor = style.selectedTextColor;
		}
		else if(hovered && i >= hoverStart && i <= hoverEnd) {
			frameColor = style.hoverFrameColor;
			textColor = style.textColor;
		}
#endif

		if(frameColor != style.headerTextColorEven) {
		   ImGui::RenderFrame(bb.Min, bb.Max, frameColor, false, 0);
		}

		ImGui::PushStyleColor(ImGuiCol_Text, textColor);
		ImGui::RenderTextClipped(bb.Min, bb.Max, label, label+2,
						  &label_size, ImVec2(0.5, 0.5), &bb);
		ImGui::PopStyleColor();
	}
}

bool uiHexPanelDoSelection(i32 panelID, i32 panelType, SelectionState* outSelectionState, i32 firstLine, i32 columnCount)
{
	// TODO: move this out?
	if(ImGui::IsAnyPopupOpen()) {
		return false;
	}

	const ImVec2 contentSize = ImGui::GetContentRegionAvail();
	const ImVec2 cursorPos = ImGui::GetCursorScreenPos();
	const ImRect winRect = ImRect(cursorPos, cursorPos + contentSize);

	ImGuiIO& io = ImGui::GetIO();
	const bool isLockedPanel = outSelectionState->lockedPanelId == panelID;
	const bool isAnyPanelLocked = outSelectionState->lockedPanelId > -1;

	if(isAnyPanelLocked) {
		if(!isLockedPanel)
			return false;
	}
	else if(!winRect.Contains(io.MousePos))
		return false;

	if(!ImGui::IsWindowHovered()) {
		return false;
	}

	ImVec2 mousePos = io.MousePos - winRect.Min; // window space

	// clamp mouse pos if a panel is selection-locked (we started selecting on this panel)
	if(isLockedPanel) {
		mousePos.x = clamp(mousePos.x, 0.0f, winRect.Max.x - winRect.Min.x - 1);
		mousePos.y = clamp(mousePos.y, 0.0f, winRect.Max.y - winRect.Min.y - 1);
	}

	const UiStyle& style = setUiStyle();

	switch(panelType) {
		case PanelType::HEX:
			uiHexPanelTypeDoSelection(outSelectionState, panelID, mousePos, winRect, style.columnWidth, style.rowHeight, firstLine, columnCount, 1);
			break;
		case PanelType::ASCII:
			uiHexPanelTypeDoSelection(outSelectionState, panelID, mousePos, winRect, style.asciiCharWidth, style.rowHeight, firstLine, columnCount, 1);
			break;
		case PanelType::INT8:
		case PanelType::UINT8:
			uiHexPanelTypeDoSelection(outSelectionState, panelID, mousePos, winRect, style.intColumnWidth * 1, style.rowHeight, firstLine, columnCount, 1);
			break;
		case PanelType::INT16:
		case PanelType::UINT16:
			uiHexPanelTypeDoSelection(outSelectionState, panelID, mousePos, winRect, style.intColumnWidth * 2, style.rowHeight, firstLine, columnCount, 2);
			break;
		case PanelType::INT32:
		case PanelType::UINT32:
		case PanelType::FLOAT32:
			uiHexPanelTypeDoSelection(outSelectionState, panelID, mousePos, winRect, style.intColumnWidth * 4, style.rowHeight, firstLine, columnCount, 4);
			break;
		case PanelType::INT64:
		case PanelType::UINT64:
		case PanelType::FLOAT64:
			uiHexPanelTypeDoSelection(outSelectionState, panelID, mousePos, winRect, style.intColumnWidth * 8, style.rowHeight, firstLine, columnCount, 8);
			break;
	}

	return true;
}

void uiHexPanelTypeDoSelection(SelectionState* outSelectionState, i32 panelId, ImVec2 mousePos, ImRect rect, i32 columnWidth_, i32 rowHeight_, i32 startLine, i32 columnCount, i32 hoverLen)
{
	const ImGuiIO& io = ImGui::GetIO();

	// hover
	i32 hoverColumn = mousePos.x / columnWidth_;
	i32 hoverLine = mousePos.y / rowHeight_;
	outSelectionState->hoverStart = (startLine + hoverLine) * columnCount + hoverColumn * hoverLen;
	outSelectionState->hoverEnd = outSelectionState->hoverStart + hoverLen;

	// selection
	if(io.MouseClicked[0]) {
		outSelectionState->selectStart = (startLine + hoverLine) * columnCount + hoverColumn * hoverLen;
		outSelectionState->selectEnd = outSelectionState->selectStart + hoverLen-1;
		outSelectionState->lockedPanelId = panelId;
		return;
	}

	if(!io.MouseDown[0]) {
		outSelectionState->lockedPanelId = -1;
	}

	hoverColumn = mousePos.x / columnWidth_;
	hoverLine = mousePos.y / rowHeight_;

	if(io.MouseDown[0] && outSelectionState->selectStart >= 0 && outSelectionState->lockedPanelId >= 0) {
		i64 startCell = outSelectionState->selectStart & ~(hoverLen-1);
		i64 hoveredCell = (startLine + hoverLine) * columnCount + hoverColumn * hoverLen;
		if(hoveredCell < startCell) {
			outSelectionState->selectEnd = (startLine + hoverLine) * columnCount + hoverColumn * hoverLen;
			outSelectionState->selectStart = startCell + hoverLen - 1;
		}
		else {
			outSelectionState->selectEnd = (startLine + hoverLine) * columnCount + hoverColumn * hoverLen
					+ hoverLen-1;
			outSelectionState->selectStart = startCell;
		}
	}
}

void uiHexDoHexPanel(ImGuiID imguiID, const PanelParams& panelParams, i32 startLine, const u8* data, i64 dataSize, i32 columnCount, const SelectionState& selection)
{
	// TODO: there are a LOT of arguments here
	// Remove selection & gradient depedency? Replace with a color buffer?
	const UiStyle& style = setUiStyle();

	const float panelWidth = ImGui::GetContentRegionAvailWidth();
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImVec2 winPos = ImGui::GetCursorScreenPos();
	ImVec2 panelSize = {panelWidth, 10}; // height doesnt really matter here
	ImRect panelRect = {winPos, winPos + panelSize};

	const i32 lineCount = window->Rect().GetHeight() / style.rowHeight;
	const i32 itemCount = min(dataSize - (i64)startLine * columnCount, lineCount * columnCount);

	ImGui::ItemSize(panelRect, 0);
	if(!ImGui::ItemAdd(panelRect, imguiID)) // TODO: is this useful?
		return;

	const ImVec2 cellSize(style.columnWidth, style.rowHeight);

	const ColorDisplay::Enum colorDisplay = panelParams.colorDisplay;
	const GradientRange& grad = panelParams.gradRange[PanelType::HEX];

	// hex table
	const i64 startDataOff = startLine * columnCount;
	for(i64 i = 0; i < itemCount; ++i) {
		const i64 dataOffset = i + startDataOff;
		u8 val = data[dataOffset];
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

		if(selection.isInSelectionRange(dataOffset)) {
			frameColor = style.selectedFrameColor;
			textColor = style.selectedTextColor;
		}
		else if(selection.isInHoverRange(dataOffset)) {
			frameColor = style.hoverFrameColor;
		}
		else {
			const f32 ga = grad.getLerpVal(val);

			const Color3 rgbCol = panelParams.gradLerpColor(ga);
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
			/*else if(colorDisplay == ColorDisplay::BRICK_COLOR) {
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
			}*/
		}

		ImGui::PushStyleColor(ImGuiCol_Text, textColor);
		ImGui::RenderFrame(bb.Min, bb.Max, frameColor, false, 0);
		ImGui::RenderTextClipped(bb.Min, bb.Max, label, label+2,
						  &label_size, ImVec2(0.5, 0.5), &bb);
		ImGui::PopStyleColor(1);
	}
}

void uiHexDoAsciiPanel(ImGuiID imguiID, const PanelParams& panelParams, i32 startLine, const u8* data, i64 dataSize, i32 columnCount, const SelectionState& selection)
{
	// TODO: there are a LOT of arguments here
	// Remove selection & gradient depedency? Replace with a color buffer?
	const UiStyle& style = setUiStyle();

	const float panelWidth = ImGui::GetContentRegionAvailWidth();
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImVec2 winPos = ImGui::GetCursorScreenPos();
	ImVec2 panelSize = {panelWidth, 10}; // height doesnt really matter here
	ImRect panelRect = {winPos, winPos + panelSize};

	const i32 lineCount = window->Rect().GetHeight() / style.rowHeight;
	const i32 itemCount = min(dataSize - (i64)startLine * columnCount, lineCount * columnCount);

	ImGui::ItemSize(panelRect, 0);
	if(!ImGui::ItemAdd(panelRect, imguiID)) // TODO: is this useful?
		return;

	const ColorDisplay::Enum colorDisplay = panelParams.colorDisplay;
	const GradientRange& grad = panelParams.gradRange[PanelType::ASCII];

	ImGui::PushFont(style.fontAscii);

	// ascii table
	const i64 startDataOff = startLine * columnCount;
	for(i64 i = 0; i < itemCount; ++i) {
		const i64 dataOff = i + startDataOff;
		const char c = (char)data[dataOff];
		i32 line = i / columnCount;
		i32 column = i % columnCount;
		ImRect bb(winPos.x + column * style.asciiCharWidth, winPos.y + line * style.rowHeight,
				  winPos.x + column * style.asciiCharWidth + style.asciiCharWidth,
				  winPos.y + line * style.rowHeight + style.rowHeight);


		ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);
		const bool isHovered = selection.isInHoverRange(dataOff);
		const bool isSelected = selection.isInSelectionRange(dataOff);

		ImU32 frameColor = 0xffffffff;

		const f32 ga = grad.getLerpVal((u8)c);

		const Color3 rgbCol = panelParams.gradLerpColor(ga);
		const f32 brightness = rgbGetBrightness(rgbCol);
		const u32 gradientColor = rgbToU32(rgbCol);
		if(brightness < 0.25) {
			textColor = rgbToU32(rgbGetLighterColor(rgbCol, 0.6f));
		}

		/*if(colorDisplay == ColorDisplay::BRICK_COLOR) {
			assert(brickWall);
			const Brick* b = brickWall->getBrick(dataOff);
			if(b) {
				frameColor = b->color;
			}
			else {
				frameColor = gradientColor;
			}
		}
		else */if(colorDisplay == ColorDisplay::GRADIENT) {
			frameColor = gradientColor;
		}

		if(isSelected) {
			frameColor = style.selectedFrameColor;
		}
		else if(isHovered) {
			frameColor = style.hoverFrameColor;
		}

		ImGui::RenderFrame(bb.Min, bb.Max, frameColor, false);

		if((u8)c >= 0x20) {
			if(isSelected) {
				textColor = style.selectedTextColor;
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
void uiHexDoFormatPanel(ImGuiID imguiID, const PanelParams& panelParams, i32 startLine, const u8* data, i64 dataSize, i32 columnCount, const SelectionState& selection, i32 panelType, const char* format)
{
	// TODO: there are a LOT of arguments here
	// Remove selection & gradient depedency? Replace with a color buffer?
	const UiStyle& style = setUiStyle();

	if(columnCount % sizeof(T)) {
		ImGui::TextColored(ImVec4(0.8, 0, 0, 1), "\nColumn count is not divisible by %d", sizeof(T));
		return;
	}

	const float panelWidth = ImGui::GetContentRegionAvailWidth();
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImVec2 winPos = ImGui::GetCursorScreenPos();
	const ImVec2 panelPos = winPos;
	const ImVec2 panelSize = {panelWidth, 10}; // height doesnt really matter here
	const ImRect panelRect = {winPos, winPos + panelSize};

	const i32 byteSize = sizeof(T);
	const i32 bitSize = byteSize << 3;

	const i32 lineCount = window->Rect().GetHeight() / style.rowHeight;
	i64 itemCount = min(dataSize - (i64)startLine * columnCount, lineCount * columnCount);
	itemCount &= ~(itemCount & (byteSize-1)); // round off item count based on byteSize
	const i64 itemCount2 = itemCount;

	ImGui::ItemSize(panelRect, 0);
	if(!ImGui::ItemAdd(panelRect, imguiID)) // TODO: is this useful?
		return;

	const ImVec2 cellSize(style.intColumnWidth * byteSize, style.rowHeight);

	const ColorDisplay::Enum colorDisplay = panelParams.colorDisplay;
	const GradientRange& grad = panelParams.gradRange[panelType];

	const i64 startLineOff = (i64)startLine * columnCount;
	for(i64 i = 0; i < itemCount2; i += byteSize) {
		const i64 dataOff = i + startLineOff;
		char integerStr[32];
		sprintf(integerStr, format, *(T*)&data[dataOff]);

		const ImVec2 labelSize = ImGui::CalcTextSize(integerStr, NULL, true);
		ImVec2 size = ImGui::CalcItemSize(cellSize, labelSize.x, labelSize.y);

		i32 col = i % columnCount;
		i32 line = i / columnCount;
		ImVec2 cellPos(col * style.intColumnWidth, line * cellSize.y);
		ImRect bb(winPos + cellPos, winPos + cellPos + size);

		f32 avgByteVal = data[dataOff];
		for(i32 a = 1; a < byteSize; ++a) {
			avgByteVal += data[dataOff + a];
		}
		avgByteVal /= byteSize;

		ImU32 frameColor = ImGui::GetColorU32(ImGuiCol_FrameBg);
		ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);

		if(colorDisplay == ColorDisplay::GRADIENT) {
			T val = *(T*)&data[dataOff];
			if(!(val == val)) { // check for NaN
				val = 0;
			}
			const f32 ga = grad.getLerpVal(val);

			const Color3 rgbCol = panelParams.gradLerpColor(ga);
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
			hovered |= selection.isInHoverRange(dataOff+h);
			selected |= selection.isInSelectionRange(dataOff+h);
		}

		if(selected) {
			frameColor = style.selectedFrameColor;
			textColor = style.selectedTextColor;
		}
		else if(hovered) {
			frameColor = style.hoverFrameColor;
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
	const f32 colHeight = actualLineCount * style.rowHeight;
	const i32 typeColumnCount = columnCount/byteSize;

	for(i32 c = 0; c <= typeColumnCount; ++c) {
		ImRect bb(panelPos.x + cellSize.x * c, panelPos.y,
				  panelPos.x + cellSize.x * c + 1, panelPos.y + colHeight + 2);
		ImGui::RenderFrame(bb.Min, bb.Max, lineColor, false, 0);
	}

	for(i32 l = 0; l < actualLineCount; ++l) {
		ImRect bb(winPos.x, winPos.y + cellSize.y * l,
				  winPos.x + panelSize.x + 1, winPos.y + cellSize.y * l + 1);
		ImGui::RenderFrame(bb.Min, bb.Max, lineColor, false, 0);
	}
}

f32 hexViewCalculatePanelWidth(i32 panelType, i32 columnCount)
{
	const UiStyle& style = setUiStyle();

	switch(panelType) {
		case PanelType::HEX:
			return columnCount * style.columnWidth;
		case PanelType::ASCII:
			return columnCount * style.asciiCharWidth;
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
			return columnCount * style.intColumnWidth;
	}

	assert(0); // should not reach here
	return -1;
}
