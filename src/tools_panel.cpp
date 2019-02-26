#include "tools_panel.h"
#include "imgui_extended.h"
#include "bricks.h"
#include "script.h"
#include "search.h"

void toolsDoInspector(const u8* fileBuffer, const i64 fileBufferSize, const SelectionState& selection)
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    const u8* dataStart = &fileBuffer[min(selection.selectStart, selection.selectEnd)];
    const u8* dataEnd = &fileBuffer[max(selection.selectStart, selection.selectEnd) + 1];

    if(selection.selectStart < 0) {
        if(selection.hoverStart >= 0) {
            dataStart = &fileBuffer[min(selection.hoverStart, selection.hoverEnd)];
            dataEnd = &fileBuffer[max(selection.hoverStart, selection.hoverEnd)];
        }
        else {
            dataStart = 0;
            dataEnd = 0;
        }
    }

    constexpr char* typeStr[] = {
        "String",
        "Integer 8",
        "Unsigned integer 8",
        "Integer 16",
        "Unsigned integer 16",
        "Integer 32",
        "Unsigned integer 32",
        "Integer 64",
        "Unsigned integer 64",
        "Float 32",
        "Float 64",
        "File offset 32",
        "File offset 64",
        "Cursor",
        "Selection start",
        "Selection end",
        "Selection size",
    };

    constexpr i32 typeCount = IM_ARRAYSIZE(typeStr);
    constexpr f32 tableLineHeight = 24;

    // ---- TYPES ----
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::BeginChild("#inspector_left", ImVec2(150, typeCount * tableLineHeight), false,
                      ImGuiWindowFlags_AlwaysUseWindowPadding);

        ImU32 frameColor = 0xffeeeeee;
        ImU32 frameColorOdd = 0xffdddddd;
        ImU32 textColor = 0xff303030;
        ImVec2 cellSize(150, tableLineHeight);
        ImVec2 align(0, 0.5);
        ImVec2 textOffset(8, 0);

        for(i32 t = 0; t < typeCount; ++t) {
            ImU32 fc = t & 1 ? frameColorOdd : frameColor;
            ImGui::TextBox(fc, textColor, cellSize, align, textOffset, typeStr[t]);
        }

    ImGui::EndChild();
    ImGui::SameLine();

    // ---- VALUES ----
    ImGui::BeginChild("#inspector_right", ImVec2(0, typeCount * 24), false,
                      ImGuiWindowFlags_AlwaysUseWindowPadding);

        frameColor = 0xffffffff;
        frameColorOdd = 0xfff0f0f0;
        textColor = 0xff000000;
        cellSize = ImVec2(ImGui::GetCurrentWindow()->Rect().GetWidth(), tableLineHeight);
        align = ImVec2(0, 0.5);
        textOffset = ImVec2(8, 0);

        char dataStr[64];
        i32 strLen = min(sizeof(dataStr)-1, dataEnd - dataStart);
        memset(dataStr, 0, sizeof(dataStr));
        memmove(dataStr, dataStart, strLen);
        for(i32 i = 0; i < strLen; ++i) {
            if(*(u8*)&dataStr[i] < 0x20) {
                dataStr[i] = ' ';
            }
        }

        // String ASCII
        ImGui::TextBox(frameColor, textColor, cellSize, align, textOffset, "%.*s", strLen, dataStr);

        i64 dataInt = 0;
        memmove(&dataInt, dataStart, min(8, dataEnd - dataStart));

        // Int 8
        ImGui::TextBox(frameColorOdd, textColor, cellSize, align, textOffset, "%d", *(i8*)&dataInt);
        // Uint 8
        ImGui::TextBox(frameColor, textColor, cellSize, align, textOffset, "%u", *(u8*)&dataInt);
        // Int 16
        ImGui::TextBox(frameColorOdd, textColor, cellSize, align, textOffset, "%d", *(i16*)&dataInt);
        // Uint 16
        ImGui::TextBox(frameColor, textColor, cellSize, align, textOffset, "%u", *(u16*)&dataInt);
        // Int 32
        ImGui::TextBox(frameColorOdd, textColor, cellSize, align, textOffset, "%d", *(i32*)&dataInt);
        // Uint 32
        ImGui::TextBox(frameColor, textColor, cellSize, align, textOffset, "%u", *(u32*)&dataInt);
        // Int 64
        ImGui::TextBox(frameColorOdd, textColor, cellSize, align, textOffset, "%lld", *(i64*)&dataInt);
        // Uint 64
        ImGui::TextBox(frameColor, textColor, cellSize, align, textOffset, "%llu", *(u64*)&dataInt);

        // Float 32
        f32 dataFloat = 0;
        memmove(&dataFloat, dataStart, min(4, dataEnd - dataStart));
        if(isnan(dataFloat)) {
            ImGui::TextBox(frameColorOdd, textColor, cellSize, align, textOffset, "NaN");
        }
        else {
            ImGui::TextBox(frameColorOdd, textColor, cellSize, align, textOffset, "%g", dataFloat);
        }

        // Float 64
        f64 dataFloat64 = 0;
        memmove(&dataFloat64, dataStart, min(8, dataEnd - dataStart));
        if(isnan(dataFloat64)) {
            ImGui::TextBox(frameColor, textColor, cellSize, align, textOffset, "NaN");
        }
        else {
            ImGui::TextBox(frameColor, textColor, cellSize, align, textOffset, "%g", dataFloat64);
        }

        const ImU32 green = 0xff00ff00;
        const ImU32 red = 0xff0000ff;

        // Offset 32
        u32 offset = *(u32*)&dataInt;
        ImGui::TextBox(frameColorOdd, offset < fileBufferSize ? green : red, cellSize, align,
                       textOffset, "%#x", offset);

        // Offset 64
        u64 offset64 = *(u64*)&dataInt;
        ImGui::TextBox(frameColor, offset < fileBufferSize ? green : red, cellSize, align,
                       textOffset, "%#llx", offset64);


        // Cursor/Selection info
        textColor = 0xff404040;
        if(selection.hoverStart >=0 ) {
            ImGui::TextBox(frameColorOdd, textColor, cellSize, align,
                           textOffset, "0x%llX", selection.hoverStart);
        }
        else {
            ImGui::TextBox(frameColorOdd, textColor, cellSize, align,
                           textOffset, "");
        }
        if(selection.selectStart >= 0) {
            ImGui::TextBox(frameColor, textColor, cellSize, align,
                           textOffset, "0x%llX", selection.selectStart);
            ImGui::TextBox(frameColorOdd, textColor, cellSize, align,
                           textOffset, "0x%llX", selection.selectEnd);
            ImGui::TextBox(frameColor, textColor, cellSize, align,
                           textOffset, "0x%llX",
                           llabs(selection.selectEnd - selection.selectStart) + 1);
        }
        else {
            ImGui::TextBox(frameColor, textColor, cellSize, align,
                           textOffset, "");
            ImGui::TextBox(frameColorOdd, textColor, cellSize, align,
                           textOffset, "");
            ImGui::TextBox(frameColor, textColor, cellSize, align,
                           textOffset, "");
        }


    ImGui::EndChild();

	ImGui::PopStyleVar(2); // ItemSpacing, WindowPadding
}

void toolsDoTemplate(BrickWall* brickWall)
{
    //ui_brickStructList(brickWall);
}

void toolsDoOptions(i32* columnCount)
{
    ImGui::SliderInt("Columns", columnCount, 8, 128);
    *columnCount = clamp(*columnCount, 8, 128);
}

void toolsDoScript(Script* script, BrickWall* brickWall)
{
    if(ImGui::Button("Execute")) {
        if(!script->execute(brickWall)) {
            LOG("Script> ERROR failed to fully execute script");
        }
    }
}

bool toolsSearchParams(SearchParams* params)
{
	bool doSearch = false;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 10));

	// TODO: so I didn't find a way to do a group with padding. A child seems required.
	// Change child size to fit content (depending on search type)
	ImGui::BeginChild("search_params", ImVec2(0, 270), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

	const char* dataTypeComboItems[] = {
		"ASCII String",
		"Integer",
		"Float",
	};

	ImGui::ButtonListOne("##comboDataType", dataTypeComboItems, arr_count(dataTypeComboItems),
						 (i32*)&params->dataType);

	switch(params->dataType) {
		case SearchDataType::ASCII_String: {
			ImGui::Text("String:");
			ImGui::InputText("##searchString", params->str, sizeof(params->str));
			params->dataSize = strlen(params->str);
			params->strideKind = SearchParams::Stride::Full;
		} break;

		case SearchDataType::Integer: {
			static i32 bitSizeItemId = 2;
			static i32 searchStrideId = 2;


			ImGui::Text("Integer size (in bits):");
			const char* bitSizeList[] = {
				"8",
				"16",
				"32",
				"64",
			};

			ImGui::ButtonListOne("byte_size_select", bitSizeList, arr_count(bitSizeList),
								 &bitSizeItemId, ImVec2(200, 0));

			ImGui::Text("Search stride:");
			const char* strideSelectList[] = {
				"Full",
				"Twice",
				"Even",
			};
			ImGui::ButtonListOne("stride_select", strideSelectList, arr_count(strideSelectList),
								 &searchStrideId, ImVec2(200, 0));

			static bool signed_ = true;
			ImGui::Checkbox("Signed", &signed_);

			ImGui::Text("Integer:");
			const i32 step = 1;
			const i32 stepFast = 100;

			ImGuiDataType_ intType = bitSizeItemId == 3 ? ImGuiDataType_S64: ImGuiDataType_S32;
			const char* format = bitSizeItemId == 3 ? "%lld": "%d";
			void* searchInt = &params->vint;
			if(!signed_) {
				intType = bitSizeItemId == 3 ? ImGuiDataType_U64: ImGuiDataType_U32;
				format = bitSizeItemId == 3 ? "%llu": "%u";
				searchInt = &params->vuint;
			}

			params->dataSize = 1 << bitSizeItemId;
			params->strideKind = (SearchParams::Stride)searchStrideId;

			ImGui::InputScalar("##int_input", intType, searchInt, &step,
							   &stepFast, format);
		} break;

		case SearchDataType::Float: {
			static i32 bitSizeItemId = 0;
			static i32 searchStrideId = 2;


			ImGui::Text("Float size (in bits):");
			const char* bitSizeList[] = {
				"32",
				"64",
			};

			ImGui::ButtonListOne("byte_size_select", bitSizeList, arr_count(bitSizeList),
								 &bitSizeItemId, ImVec2(100, 0));

			ImGui::Text("Search stride:");
			const char* strideSelectList[] = {
				"Full",
				"Twice",
				"Even",
			};
			ImGui::ButtonListOne("stride_select", strideSelectList, arr_count(strideSelectList),
								 &searchStrideId, ImVec2(200, 0));

			ImGui::Text("Float:");

			const f64 step = 1;
			const f64 stepFast = 10;
			ImGuiDataType_ fltType = bitSizeItemId == 0 ? ImGuiDataType_Float: ImGuiDataType_Double;
			const char* format = bitSizeItemId == 0 ? "%.5f": "%.10f";
			void* fltVal = bitSizeItemId == 0 ? &params->vf32: &params->vf64;

			params->dataSize = bitSizeItemId == 0 ? 4 : 8;
			params->strideKind = (SearchParams::Stride)searchStrideId;

			ImGui::InputScalar("##flt_input", fltType, fltVal, &step,
							   &stepFast, format);
		} break;

		default: assert(0); break;
	}

	if(ImGui::Button("Search", ImVec2(120,0))) {
		doSearch = true;
	}

	ImGui::EndChild();
	ImGui::PopStyleVar(2); // ItemSpacing, WindowPadding
	return doSearch;
}

bool toolsSearchResults(const SearchParams& params, const Array<SearchResult>& results, u64* gotoOffset)
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	switch(params.dataType) {
		case SearchDataType::ASCII_String: {
			const f32 typeFrameLen = ImGui::CalcTextSize("ASCII").x + 20.0f;
			const f32 searchTermFrameLen = ImGui::GetContentRegionAvail().x - typeFrameLen;

			ImGui::TextBox(0xffdfdfdf, 0xff000000, ImVec2(typeFrameLen, 35), ImVec2(0.5, 0.5), ImVec2(0, 0),
						   "ASCII");
			ImGui::SameLine();
			ImGui::TextBox(0xffffefef, 0xffff0000, ImVec2(searchTermFrameLen, 35),
						   ImVec2(0, 0.5), ImVec2(10, 0),
						   "%.*s", params.dataSize, params.str);
		} break;

		case SearchDataType::Integer: {
			if(params.intSigned) {
				const f32 typeFrameLen = ImGui::CalcTextSize("Integer").x + 20.0f;
				const f32 searchTermFrameLen = ImGui::GetContentRegionAvail().x - typeFrameLen;

				ImGui::TextBox(0xffdfdfdf, 0xff000000, ImVec2(typeFrameLen, 35), ImVec2(0.5, 0.5),
							   ImVec2(0, 0), "Integer");
				ImGui::SameLine();
				ImGui::TextBox(0xffffefef, 0xffff0000, ImVec2(searchTermFrameLen, 35), ImVec2(0, 0.5),
							   ImVec2(10, 0),
							   "%d", params.vint);
			}
			else {
				const f32 typeFrameLen = ImGui::CalcTextSize("Unsigned integer").x + 20.0f;
				const f32 searchTermFrameLen = ImGui::GetContentRegionAvail().x - typeFrameLen;

				ImGui::TextBox(0xffdfdfdf, 0xff000000, ImVec2(typeFrameLen, 35), ImVec2(0.5, 0.5),
							   ImVec2(0, 0), "Unsigned integer");
				ImGui::SameLine();
				ImGui::TextBox(0xffffefef, 0xffff0000, ImVec2(searchTermFrameLen, 35),
							   ImVec2(0, 0.5), ImVec2(10, 0),
							   "%llu", params.vuint);
			}
		} break;

		case SearchDataType::Float: {
			const f32 typeFrameLen = ImGui::CalcTextSize("Float").x + 20.0f;
			const f32 searchTermFrameLen = ImGui::GetContentRegionAvail().x - typeFrameLen;

			ImGui::TextBox(0xffdfdfdf, 0xff000000, ImVec2(typeFrameLen, 35), ImVec2(0.5, 0.5), ImVec2(0, 0),
						   "Float");
			ImGui::SameLine();
			ImGui::TextBox(0xffffefef, 0xffff0000, ImVec2(searchTermFrameLen, 35), ImVec2(0, 0.5),
						   ImVec2(10, 0),
						   "%g", params.dataSize == 4 ? params.vf32 : params.vf64);
		} break;
	}

	ImGui::TextBox(0xffffffff, 0xff000000, ImVec2(0, 30), ImVec2(0, 0.5), ImVec2(10, 0),
				   "%d found", results.count());

	const i32 count = results.count();
	if(count <= 0) {
		ImGui::PopStyleVar(1); // ItemSpacing
		return false;
	}

	bool clicked = false;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::BeginChild("search_results", {0,0}, false, ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	const ImVec2 padding = {12, 3};
	const ImVec2 textSize = ImGui::CalcTextSize("AAAAAAAAAAA");
	ImGuiListClipper clipper(count, textSize.y + padding.y * 2);

	for(i32 i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
		const u64 itemDataOffset = results[i].offset;
		const ImVec2 pos = window->DC.CursorPos;
		const ImVec2 size = {ImGui::GetContentRegionAvail().x, textSize.y + padding.y * 2};
		const ImRect frameBb = {pos, pos + size};
		bool hovered, held;
		ImGui::ButtonBehavior(frameBb, window->GetID(((u8*)&results) + i), &hovered, &held);

		u32 textColor = 0xff000000;
		u32 frameColor = (i&1) ? 0xfff0f0f0 : 0xffe0e0e0;

		if(held) {
			frameColor = 0xffff7200;
			textColor = 0xffffffff;
			*gotoOffset = itemDataOffset;
		}
		else if(hovered) {
			frameColor = 0xffffb056;
			textColor = 0xffff0000;
		}
		clicked |= held;

		ImGui::TextBox(frameColor, textColor, size,
					   ImVec2(0, 0.5), ImVec2(padding.x, 0),
					   "%llx", itemDataOffset);
	}

	// TODO: add pages
	clipper.End();

	ImGui::ItemSize(ImVec2(10, 30));

	ImGui::EndChild();
	ImGui::PopStyleVar(2); // ItemSpacing, WindowPadding
	return clicked;
}
