#include "imgui_extended.h"
#include "bricks.h"
#include <gl3w.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

namespace ImGui {

void DoScrollbarVertical(i64* outScrollVal, i64 scrollPageSize, i64 scrollTotalSize)
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
    window->ContentsRegionRect.Max.x += style.ScrollbarSize;

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

    const f32 minGrabHeight = style.GrabMinSize;
    if(height < minGrabHeight) {
        scrollSurfaceSizeY -= minGrabHeight - height;
        height = minGrabHeight;
    }

    //LOG("height=%.5f pageSize=%llu contentSize=%llu", height, pageSize, contentSize);
    f32 yOffset = scrollSurfaceSizeY * ((f64)scrollVal / scrollTotalSize);
    bb = ImRect(bbBg.Min.x + 2.0f, bbBg.Min.y + yOffset, bbBg.Max.x - 2.0f, bbBg.Min.y + yOffset + height);
    /*bb.Expand(ImVec2(-ImClamp((float)(int)((bb.Max.x - bb.Min.x - 2.0f) * 0.5f), 0.0f, 3.0f),
                     -ImClamp((float)(int)((bb.Max.y - bb.Min.y - 2.0f) * 0.5f), 0.0f, 3.0f)));*/
    bool held = false;
    bool hovered = false;
    const bool previouslyHeld = (g.ActiveId == id);
    ButtonBehavior(bb, id, &hovered, &held);

    static f32 mouseGrabDeltaY = 0;

    if(held) {
        if(!previouslyHeld) {
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
        if(hovered && hovered != window) { // go up one level
            hovered = hovered->ParentWindow;
        }

        if(hovered && hovered == window && g.IO.MouseWheel != 0.0f) {
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

f32 GetComboHeight()
{
    const ImVec2 label_size = CalcTextSize("SomeText", NULL, true);
    return label_size.y + ImGui::GetStyle().FramePadding.y * 2.0f;
}

void Tabs(const char* label, const char** names, const i32 count, i32* selected)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = GetStyle();
    ImGuiWindow* window = GetCurrentWindow();
    const ImGuiID id = window->GetID(label);

    f32 buttonHeight = CalcTextSize(names[0], NULL, true).y + style.FramePadding.y * 2.0f;
    ImVec2 size = CalcItemSize(ImVec2(-1, buttonHeight), 100, buttonHeight);
    ImVec2 pos = window->DC.CursorPos;
    const ImRect rect(pos, pos + size);

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

void SplitVBeginLeft(const char* label, f32* leftWidth, f32* rightWidth, const i32 separatorWidth,
                     ImGuiWindowFlags extraFlags)
{
    assert(leftWidth || rightWidth);

    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    ImGuiWindow* window = GetCurrentWindow();
    ImVec2 size(separatorWidth, window->Rect().GetHeight());

    const f32 winWidth = window->Rect().GetWidth();


    f32 childLeftWidth = 100, childRightWidth = 100;
    if(leftWidth) {
        childLeftWidth = *leftWidth;
        childRightWidth = winWidth - separatorWidth - childLeftWidth;
    }
    else {
        childRightWidth = *rightWidth;
        childLeftWidth = winWidth - separatorWidth - childRightWidth;
    }

    ImVec2 pos = window->Rect().Min + ImVec2(childLeftWidth, 0);
    ImRect bb(pos, pos + size);

    const ImGuiID id = window->GetID(leftWidth ? leftWidth : rightWidth);
    bool held = false;
    bool hovered = false;
    ButtonBehavior(bb, id, &hovered, &held);

    constexpr i32 minWidth = 100;

    ImU32 buttonColor = 0xffcccccc;
    ImU32 textColor = 0xff505050;
    if(held) {
        buttonColor = 0xffff7200;
        textColor = 0xff000000;
        ImVec2 mousePos = GetIO().MousePos;
        mousePos.x -= bb.Min.x + separatorWidth * 0.5; // vertical center of button
        childLeftWidth += mousePos.x;
        childRightWidth = winWidth - childLeftWidth - separatorWidth;

        if(childLeftWidth < minWidth) {
            mousePos.x = 0;
            childLeftWidth = minWidth;
            childRightWidth = winWidth - childLeftWidth - separatorWidth;
        }
        else if(childRightWidth < minWidth) {
            mousePos.x = 0;
            childRightWidth = minWidth;
            childLeftWidth = winWidth - separatorWidth - childRightWidth;
        }

        if(leftWidth) {
            *leftWidth = childLeftWidth;
        }
        else {
            *rightWidth = childRightWidth;
        }

        bb.Translate(ImVec2(mousePos.x, 0));
    }
    else if(hovered) {
        buttonColor = 0xffffc5a3;
        textColor = 0xff000000;
    }
    RenderFrame(bb.Min, bb.Max, buttonColor, true);

    BeginChild(label, ImVec2(childLeftWidth, -1), false,
               ImGuiWindowFlags_AlwaysUseWindowPadding|extraFlags);
}

void SplitVBeginRight(const char* label, f32* leftWidth, f32* rightWidth, const i32 separatorWidth,
                      ImGuiWindowFlags extraFlags)
{
    assert(leftWidth || rightWidth);

    EndChild();

    ImGuiWindow* window = GetCurrentWindow();
    SameLine();
    ItemSize(ImVec2(separatorWidth, window->Rect().GetHeight()));
    SameLine();


    f32 childRightWidth = 100;
    if(leftWidth) {
        childRightWidth = window->Rect().GetWidth() - separatorWidth - *leftWidth;
    }
    else {
        childRightWidth = *rightWidth;
    }

    BeginChild(label, ImVec2(childRightWidth, -1), false,
               ImGuiWindowFlags_AlwaysUseWindowPadding|extraFlags);
}

void SplitVEnd()
{
    EndChild();
    PopStyleVar(1);
}

bool IsAnyPopupOpen()
{
    ImGuiContext& g = *GImGui;
    return !g.OpenPopupStack.empty();
}

void TextBox(ImU32 frameColor, ImU32 textColor, ImVec2 size, ImVec2 align, ImVec2 textOffset,
			 const char* fmt, ...)
{
    ImGuiWindow* window = GetCurrentWindow();
    ImVec2 pos = window->DC.CursorPos;
    if(size.x == 0) size.x = ImGui::GetContentRegionAvail().x;
    if(size.y == 0) size.y = ImGui::GetContentRegionAvail().y;
    ImRect bb(pos, pos + size);

    ItemSize(size);
    RenderFrame(bb.Min, bb.Max, frameColor, false, 0);

    char str[64];
    i32 strLen = 0;

    va_list args;
    va_start(args, fmt);
    strLen = vsprintf(str, fmt, args);
    va_end(args);

    ImVec2 labelSize = CalcTextSize(str, str + strLen);

    bb.Translate(textOffset);

    PushStyleColor(ImGuiCol_Text, textColor);
    RenderTextClipped(bb.Min, bb.Max, str, str + strLen,
                      &labelSize, align, &bb);
    PopStyleColor(1);
}

void ButtonListOne(const char* strId, const char* names[], const i32 count, i32* selected, ImVec2 size_)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = GetStyle();
    ImGuiWindow* window = GetCurrentWindow();
    const ImGuiID id = window->GetID(strId);

    const f32 buttonHeight = CalcTextSize(names[0], NULL, true).y + style.FramePadding.y * 2.0f;
    const ImVec2 widgetSize = CalcItemSize(ImVec2(size_.x, size_.y <= 0 ? buttonHeight: size_.y),
                                           100, buttonHeight);
    const f32 buttonWidth = widgetSize.x / count;
    ImVec2 pos = window->DC.CursorPos;
    const ImRect rect(pos, pos + widgetSize);

    ItemSize(rect, 0);
    if(!ItemAdd(rect, id)) {
        return;
    }

    // TODO: add more style colors
    const u32 textColorOff = GetColorU32(ImGuiCol_TextDisabled);
    const u32 textColorBase = GetColorU32(ImGuiCol_Text);
    const u32 colorOff = GetColorU32(ImGuiCol_Button);
    const u32 colorHovered = GetColorU32(ImGuiCol_ButtonHovered);
    const u32 colorOn = GetColorU32(ImGuiCol_ButtonActive);

    f32 offX = 0;
    for(i32 i = 0; i < count; ++i) {
        ImRect bb = rect;
        bb.Min.x += offX;
        bb.Max.x = bb.Min.x + buttonWidth;
        offX += buttonWidth;

        const ImGuiID butId = id + i;
        bool held = false;
        bool hovered = false;
        const bool previouslyHeld = (g.ActiveId == butId);
        ButtonBehavior(bb, butId, &hovered, &held);

        if(held) {
            *selected = i;
        }

        ImU32 buttonColor = colorOff;
        ImU32 textColor = textColorOff;
        if(*selected == i) {
            buttonColor = colorOn;
            textColor = textColorBase;
        }
        else if(hovered) {
            buttonColor = colorHovered;
            textColor = textColorBase;
        }
        RenderFrame(bb.Min, bb.Max, buttonColor, false);

        PushStyleColor(ImGuiCol_Text, textColor);
        RenderTextClipped(bb.Min, bb.Max, names[i], NULL,
                          NULL, ImVec2(0.5, 0.5), &bb);
        PopStyleColor();
    }
}

}
