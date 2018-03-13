#include "bricks.h"
#include "imgui.h"

#define POPUP_BRICK_ADD_ERROR "Error adding brick"

constexpr i32 g_typeSize[BrickType::_COUNT] = {
    sizeof(char),
    sizeof(wchar_t),

    sizeof(i8),
    sizeof(u8),
    sizeof(i16),
    sizeof(u16),
    sizeof(i32),
    sizeof(u32),
    sizeof(i64),
    sizeof(u64),

    sizeof(f32),
    sizeof(f64),

    sizeof(i32),
    sizeof(i64),
};

const char* g_typeStr[BrickType::_COUNT] = {
    "Char",
    "Wide char",

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

    "Offset32",
    "Offset64",
};

bool BrickWall::addBrick(Brick b)
{
    assert((i32)b.type >= 0 && b.type < BrickType::_COUNT);

    if(b.length % g_typeSize[(i32)b.type] != 0) {
        return false;
    }

    const i32 count = bricks.count();
    i32 where = -1;

    // check for overlaps
    for(i32 i = 0; i < count; ++i) {
        const Brick& b1 = bricks[i];
        if(b.start >= b1.start && b.start < b1.start + b1.length) {
            return false;
        }
    }

    for(i32 i = 0; i < count; ++i) {
        const Brick& b1 = bricks[i];
        if(b1.start + b1.length < b.start) {
            where = i;
            break;
        }
    }

    if(where != -1) {
        bricks.insert(where, b);
    }
    else {
        bricks.push(b);
    }
    return true;
}

const Brick* BrickWall::getBrick(intptr_t offset)
{
    const i32 count = bricks.count();
    for(i32 i = 0; i < count; ++i) {
        const Brick& b1 = bricks[i];
        if(b1.start <= offset && b1.start + b1.length > offset) {
            return &b1;
        }
    }
    return nullptr;
}

void ui_brickPopup(const char* popupId, intptr_t selStart, i64 selLength, BrickWall* wall)
{
    static i32 popupType = 0;
    static f32 popupBrickColor[3] = {1};

    bool popupBrickErrorOpen = false;

    if(ImGui::BeginPopupModal(popupId, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Brick {start: 0x%llx, length: %lld}", selStart, selLength);
        ImGui::Text("Array size: %d", selLength / g_typeSize[popupType]);
        ImGui::Separator();

        ImGui::Combo("Type", (int*)&popupType, g_typeStr, IM_ARRAYSIZE(g_typeStr),
                     IM_ARRAYSIZE(g_typeStr));

        ImGuiColorEditFlags flags = ImGuiColorEditFlags_RGB|ImGuiColorEditFlags_PickerHueBar;
        ImGui::ColorPicker3("Color", popupBrickColor, flags);

        if(ImGui::Button("OK", ImVec2(120,0))) {
            // add brick
            const i32 typeSize = g_typeSize[popupType];
            Brick b;
            b.start = selStart;
            b.length = (selLength / typeSize) * typeSize;
            b.color = ImGui::ColorConvertFloat4ToU32(ImVec4(popupBrickColor[0], popupBrickColor[1],
                    popupBrickColor[2], 1.0));
            bool r = wall->addBrick(b);
            if(!r) {
                LOG("ERROR> could not add brick {type: %d, start: 0x%llx, length: %lld}",
                    b.type, b.start, b.length);
                popupBrickErrorOpen = true;
            }

            // reset popup color
            popupBrickColor[0] = 1;
            popupBrickColor[1] = 0;
            popupBrickColor[2] = 0;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel", ImVec2(120,0))) {
            // reset popup color
            popupBrickColor[0] = 1;
            popupBrickColor[1] = 0;
            popupBrickColor[2] = 0;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if(popupBrickErrorOpen) {
        ImGui::OpenPopup(POPUP_BRICK_ADD_ERROR);
        popupBrickErrorOpen = false;
    }

    ImGui::PushStyleColor(ImGuiCol_PopupBg, 0xff3a3aff);
    if(ImGui::BeginPopupModal(POPUP_BRICK_ADD_ERROR, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextUnformatted("Can't overlap bricks");
        if(ImGui::Button("Ok", ImVec2(180,0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleColor(1);
}
