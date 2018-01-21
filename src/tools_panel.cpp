#include "tools_panel.h"
#include "imgui_extended.h"

void toolsDoInspector(const u8* fileBuffer, const i64 fileBufferSize, const SelectionState& selection)
{
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
    ImGui::BeginChild("#inspector_left", ImVec2(150, typeCount * tableLineHeight));

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
    ImGui::BeginChild("#inspector_right", ImVec2(0, typeCount * 24));

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
}

void toolsDoTemplate()
{
    ImGui::GetCurrentWindow()->ContentsRegionRect.Expand(-10);
    ImRect winRect = ImGui::GetCurrentWindow()->Rect();
    ImGui::BeginChild("#template_content", ImVec2(0, winRect.GetHeight() - 50));



    ImGui::EndChild();

    ImGui::Button("Int8"); ImGui::SameLine();
    ImGui::Button("Int16"); ImGui::SameLine();
    ImGui::Button("Int32"); ImGui::SameLine();
    ImGui::Button("Int64"); ImGui::SameLine();
}
