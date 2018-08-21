#pragma once
#include "base.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

namespace ImGui {

void DoScrollbarVertical(i64* outScrollVal, i64 scrollPageSize, i64 scrollTotalSize);

f32 GetComboHeight();

void Tabs(const char* label, const char** names, const i32 count, i32* selected);

void SplitVBeginLeft(const char* label, f32* leftWidth, f32* rightWidth,
                     const i32 separatorWidth = 6, ImGuiWindowFlags extraFlags = 0);
void SplitVBeginRight(const char* label, f32* leftWidth, f32* rightWidth,
                      const i32 separatorWidth = 6, ImGuiWindowFlags extraFlags = 0);
void SplitVEnd();

bool IsAnyPopupOpen();

void TextBox(ImU32 frameColor, ImU32 textColor, ImVec2 size, ImVec2 align, ImVec2 textOffset,
                    const char* fmt, ...);

void ButtonListOne(const char* strId, const char* names[], const i32 count, i32* selected,
                      ImVec2 size_ = {-1, 0});

}
