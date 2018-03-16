#include "bricks.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_extended.h"

#define POPUP_BRICK_ADD_ERROR "Error adding brick"

constexpr i32 g_typeSize[BrickType__COUNT] = {
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

    1
};

const char* g_typeStr[BrickType__COUNT] = {
    "char",
    "wchar",

    "int8",
    "uint8",
    "int16",
    "uint16",
    "int32",
    "uint32",
    "int64",
    "uint64",

    "float32",
    "float64",

    "offset32",
    "offset64",

    "User structure",
};

BrickWall::BrickWall()
{
    typeCache.reserve(32);
    _rebuildTypeCache();
}

void BrickWall::_rebuildTypeCache()
{
    typeCache.clear();
    for(i32 i = 0; i < (i32)BrickType_USER_STRUCT; i++) {
        typeCache.push({ g_typeStr[i], g_typeSize[i] });
    }
    for(i32 i = 0; i < structs.count(); i++) {
        typeCache.push({ structs[i].name, structs[i]._size });
    }
}

bool BrickWall::insertBrick(Brick b)
{
    assert((i32)b.type >= 0 && b.type < typeCache.count());
    assert(b.size % typeCache[b.type].size == 0);

    const i32 count = bricks.count();
    i32 where = -1;

    // check for overlaps
    for(i32 i = 0; i < count; ++i) {
        const Brick& b1 = bricks[i];
        if(b.start >= b1.start && b.start < b1.start + b1.size) {
            return false;
        }
    }

    for(i32 i = 0; i < count; ++i) {
        const Brick& b1 = bricks[i];
        if(b1.start >= b.start + b.size) {
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

bool BrickWall::insertBrickStruct(const char* name, intptr_t where, i32 count, const BrickStruct& bstruct)
{
    Brick b;

    if(name[0] == 0) {
        b.name = "not_named";
    }
    else {
        b.name = name;
    }

    assert(&bstruct >= structs.data() && &bstruct < structs.data() + structs.count());
    b.type = (BrickType)(BrickType_USER_STRUCT + (&bstruct - structs.data())); // TODO: use id instead?
    b.size = bstruct._size * count;
    b.start = where;
    b.userStruct = &bstruct;
    b.color = bstruct.color;
    return insertBrick(b);
}

const Brick* BrickWall::getBrick(intptr_t offset)
{
    const i32 count = bricks.count();
    for(i32 i = 0; i < count; ++i) {
        const Brick& b1 = bricks[i];
        if(b1.start <= offset && b1.start + b1.size > offset) {
            return &b1;
        }
    }
    return nullptr;
}

BrickStruct* BrickWall::newStructDef(const char* name, u32 color)
{
    BrickStruct brickStruct;
    const i32 len = strlen(name);
    brickStruct.name.len = len;
    assert(len < 32);
    memmove(brickStruct.name.str, name, sizeof(brickStruct.name.str[0]) * len);
    brickStruct.name.str[len] = 0;
    brickStruct._size = 0;
    brickStruct.color = color;
    return &structs.push(brickStruct);
}

void ui_brickPopup(const char* popupId, intptr_t selStart, i64 selLength, BrickWall* wall)
{
    static i32 popupType = 0;
    static i32 popupArraySize = -1;
    static f32 popupBrickColor[3] = {1};
    static char popupBrickName[32] = {0};
    static bool popupOverrideSelLimit = false;

    constexpr auto popupResetDefault = []() {
        popupType = 0;
        popupArraySize = -1;
        popupOverrideSelLimit = false;
    };

    const BrickWall::TypeCache* typeCache = wall->typeCache.data();
    const i32 typeCacheCount = wall->typeCache.count();

    bool popupBrickErrorOpen = false;

    if(ImGui::BeginPopupModal(popupId, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Start: 0x%llx, size: %lld", selStart, selLength);
        ImGui::Separator();

        ImGui::Checkbox("Override selection limit", &popupOverrideSelLimit);

        // name
        ImGui::InputText("Name", popupBrickName, sizeof(popupBrickName));

        // type
        if(ImGui::BeginCombo("Type", typeCache[popupType].name.str)) {
            for(i32 t = 0; t < typeCacheCount; t++) {
                if(!popupOverrideSelLimit && typeCache[t].size > selLength) continue;

                bool is_selected = t == popupType;
                if(ImGui::Selectable(typeCache[t].name.str, is_selected)) {
                    popupType = t;
                    popupArraySize = -1; // reset array size
                }
                if(is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        // array size
        const i32 maxArraySize = selLength / typeCache[popupType].size;
        if(popupArraySize == -1) {
            popupArraySize = maxArraySize;
        }
        if(popupOverrideSelLimit) {
            ImGui::InputInt("Array size", &popupArraySize);
        }
        else {
            ImGui::SliderInt("Array size", &popupArraySize, 1, maxArraySize);
        }

        ImGuiColorEditFlags flags = ImGuiColorEditFlags_RGB|ImGuiColorEditFlags_PickerHueBar;
        ImGui::ColorPicker3("Color", popupBrickColor, flags);


        if(ImGui::Button("OK", ImVec2(120,0))) {
            assert(popupArraySize > 0);

            // add brick
            const i64 typeSize = typeCache[popupType].size;

            if(popupType >= (i32)BrickType_USER_STRUCT) {
                bool r = wall->insertBrickStruct(popupBrickName, selStart, popupArraySize,
                                                 wall->structs[popupType - BrickType_USER_STRUCT]);
                if(!r) {
                    LOG("ERROR> could not add brick struct {type: %d, start: 0x%llx, length: %lld}",
                        popupType, selStart, typeSize);
                    popupBrickErrorOpen = true; // TODO: unfify error popup message
                }
            }
            else {
                Brick b;

                if(popupBrickName[0] == 0) {
                    b.name.set("not_named");
                }
                else {
                    b.name.set(popupBrickName);
                }

                b.start = selStart;
                b.size = popupArraySize * typeSize;
                b.color = ImGui::ColorConvertFloat4ToU32(ImVec4(popupBrickColor[0], popupBrickColor[1],
                        popupBrickColor[2], 1.0));
                b.type = (BrickType)popupType;

                bool r = wall->insertBrick(b);
                if(!r) {
                    LOG("ERROR> could not add brick {type: %d, start: 0x%llx, length: %lld}",
                        b.type, b.start, b.size);
                    popupBrickErrorOpen = true;
                }
            }

            popupResetDefault();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel", ImVec2(120,0))) {
            popupResetDefault();
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

void ui_oneBrick(const Brick& brick, f32 width, const Array<BrickWall::TypeCache>& typeCache)
{
    const ImGuiStyle& style = ImGui::GetStyle();
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size(width, ImGui::GetTextLineHeightWithSpacing());
    ImRect bb(pos, pos + size);
    const ImVec2 paddingX(style.FramePadding.x, 0);

    ImGui::ItemSize(size);

    char typeStr[64];
    i32 typeStrLen = 0;
    i32 arrayCount = brick.size/typeCache[brick.type].size;

    if(arrayCount > 1) {
        typeStrLen = sprintf(typeStr, "%s[%d]", typeCache[brick.type].name.str, arrayCount);
    }
    else {
        typeStrLen = sprintf(typeStr, "%s", typeCache[brick.type].name.str);
    }

    f32 colAvg = colorAvgChannel(brick.color);

    if(colAvg < 80.0f) {
        ImGui::PushStyleColor(ImGuiCol_Text, 0xffffffff);
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_Text, 0xff000000);
    }

    ImGui::RenderFrame(bb.Min, bb.Max, brick.color, true, 3.0f);
    ImGui::RenderTextClipped(bb.Min, bb.Max - paddingX, brick.name.str, brick.name.str + brick.name.len,
                             NULL, ImVec2(1, 0.5), &bb);
    ImGui::RenderTextClipped(bb.Min + paddingX, bb.Max, typeStr, typeStr + typeStrLen, NULL,
                             ImVec2(0, 0.5), &bb);

    ImGui::PopStyleColor(1); // Text
}

inline f32 uiGetBrickHeight()
{
    return ImGui::GetTextLineHeightWithSpacing();
}

bool ui_brickStruct(const BrickStruct& brickStruct, f32 width, bool8* expanded,
                    const Array<BrickWall::TypeCache>& typeCache)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = ImGui::GetStyle();
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImVec2 pos = window->DC.CursorPos;
    const ImVec2 borderMin = pos;
    const ImVec2 paddingX(style.FramePadding.x, 0);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    const ImVec2 fullHeaderSize(width, ImGui::GetTextLineHeightWithSpacing() + 12);
    ImGui::ItemSize(fullHeaderSize);

    // expand button
    ImVec2 butSize(fullHeaderSize.y, fullHeaderSize.y);
    ImRect butBb(pos, pos + butSize);

    const u32 normalColor = brickStruct.color;
    const u32 hoverColor = colorAddAllChannels(normalColor, 50);
    const u32 heldColor =  colorAddAllChannels(normalColor, -50);
    const ImGuiID butId = window->GetID(&brickStruct);
    bool held = false;
    bool hovered = false;
    bool previouslyHeld = (g.ActiveId == butId);
    ImGui::ButtonBehavior(butBb, butId, &hovered, &held);

    u32 buttonColor = normalColor;
    if(held) {
        buttonColor = heldColor;
    }
    else if(hovered) {
        buttonColor = hoverColor;
    }

    if(held && !previouslyHeld) {
        *expanded ^= 1;
    }

    ImGui::RenderFrame(butBb.Min, butBb.Max, buttonColor, true, 0.0);
    ImGui::RenderTextClipped(butBb.Min, butBb.Max, *expanded ? "-":"+", NULL, NULL,
                             ImVec2(0.5, 0.5), &butBb);

    // header button
    pos.x += butSize.x;
    ImVec2 size(width - butSize.x, fullHeaderSize.y);
    ImRect bb(pos, pos + size);

    const ImGuiID headerId = window->GetID(&brickStruct.name.len);
    held = false;
    hovered = false;
    previouslyHeld = (g.ActiveId == headerId);
    ImGui::ButtonBehavior(bb, headerId, &hovered, &held);

    buttonColor = normalColor;
    if(held) {
        buttonColor = heldColor;
    }
    else if(hovered) {
        buttonColor = hoverColor;
    }

    char sizeStr[32];
    i32 sizeStrLen = sprintf(sizeStr, "%lld", brickStruct._size);

    ImGui::RenderFrame(bb.Min, bb.Max, buttonColor, false, 0.0);
    ImGui::RenderTextClipped(bb.Min + paddingX, bb.Max, brickStruct.name.str,
                             brickStruct.name.str + brickStruct.name.len, NULL,
                             ImVec2(0, 0.5), &bb);
    ImGui::RenderTextClipped(bb.Min, bb.Max - paddingX, sizeStr,
                             sizeStr + sizeStrLen, NULL,
                             ImVec2(1, 0.5), &bb);

    // structure members
    if(*expanded) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));

        const i32 brickCount = brickStruct.bricks.count();
        f32 childHeight = style.WindowPadding.y * 2 +
                          brickCount * (uiGetBrickHeight() + style.ItemSpacing.y);
        const ImGuiID childId = window->GetID(&brickStruct.bricks);

        ImGui::BeginChild(childId, ImVec2(width, childHeight), false,
                          ImGuiWindowFlags_AlwaysUseWindowPadding);

        const Brick* bricks = brickStruct.bricks.data();
        for(i32 i = 0; i < brickCount; ++i) {
            const Brick& b = bricks[i];
            ui_oneBrick(b, width - 10, typeCache);
        }

        window = ImGui::GetCurrentWindow();
        ImVec2 borderMax(borderMin.x + width, window->DC.CursorPos.y + style.WindowPadding.y);

        ImGui::EndChild();
        ImGui::PopStyleVar(2);

        window = ImGui::GetCurrentWindow();
        window->DrawList->AddRect(borderMin, borderMax, normalColor, 0,
                                  ImDrawCornerFlags_All, 3.0f);
    }

    ImGui::PopStyleVar(1); // ItemSpacing (0,0)

    ImGui::ItemSize(ImVec2(1, 1));

    if(held && !previouslyHeld) {
        return true;
    }

    return false;
}

void ui_brickStructList(BrickWall* brickWall)
{
    static i32 selectedStructId = -1;
    static bool8 expanded[1024];

    ImRect winRect = ImGui::GetCurrentWindow()->Rect();
    ImGui::BeginChild("#struct_list", ImVec2(-1, winRect.GetHeight() * 0.3));

    BrickStruct* structs = brickWall->structs.data();
    const i32 structCount = brickWall->structs.count();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, 1));

    for(i32 s = 0; s < structCount; ++s) {
        if(ui_brickStruct(structs[s], 300, &expanded[s], brickWall->typeCache)) {
            selectedStructId = s;
        }
    }

    ImGui::PopStyleVar(1); // ItemSpacing

    ImGui::EndChild();

    ImGui::Separator();

    if(ImGui::CollapsingHeader("Add structure")) {
        static char bsName[32] = {0};
        static f32 bsColor[3] = {1};
        ImGui::InputText("Name##struct", bsName, sizeof(bsName));

        ImGui::PushItemWidth(200);
        ImGuiColorEditFlags flags = ImGuiColorEditFlags_RGB|ImGuiColorEditFlags_PickerHueBar;
        ImGui::ColorPicker3("Color##struct", bsColor, flags);
        ImGui::PopItemWidth();

        if(ImGui::Button("Add", ImVec2(100, 0))) {
            brickWall->newStructDef(bsName, ImGui::ColorConvertFloat4ToU32(ImVec4(bsColor[0],
                       bsColor[1], bsColor[2], 1.0)));
        }
    }

    if(selectedStructId != -1) {
        BrickStruct& selectedStruct = structs[selectedStructId];
        char addMemToStr[128];
        sprintf(addMemToStr, "Add member to '%s'", selectedStruct.name.str);
        if(ImGui::CollapsingHeader(addMemToStr)) {
            static BrickType memType = {};
            static char memName[32] = {};
            static i32 memArrayCount = 1;
            static f32 memColor[3] = {1};

            ImGui::InputText("Name##member", memName, sizeof(memName));
            ImGui::Combo("Type", (int*)&memType, g_typeStr, IM_ARRAYSIZE(g_typeStr),
                         IM_ARRAYSIZE(g_typeStr));
            ImGui::InputInt("Array count", &memArrayCount);

            ImGui::PushItemWidth(200);
            ImGuiColorEditFlags flags = ImGuiColorEditFlags_RGB|ImGuiColorEditFlags_PickerHueBar;
            ImGui::ColorPicker3("Color", memColor, flags);
            ImGui::PopItemWidth();

            if(ImGui::Button("Add member")) {
                Brick b;
                b.type = memType;
                b.size = g_typeSize[(i32)memType] * memArrayCount;
                b.color = ImGui::ColorConvertFloat4ToU32(ImVec4(memColor[0], memColor[1],
                memColor[2], 1.0));
                b.name.set(memName);
                selectedStruct.bricks.push(b);
                selectedStruct.computeSize();
            }
        }
    }
}

void ui_brickWall(BrickWall* brickWall)
{
    const i32 brickCount = brickWall->bricks.count();
    const Brick* bricks = brickWall->bricks.data();

    for(i32 i = 0; i < brickCount; ++i) {
        const Brick& b = bricks[i];
        ui_oneBrick(b, 200, brickWall->typeCache);
    }
}
