#pragma once
#include <gl3w.h>
#include <SDL2/SDL_events.h>
#include "base.h"

struct ImGuiGLSetup;

struct ImGuiGLSetup* imguiInit(u32 width, u32 height);
void imguiDeinit(struct ImGuiGLSetup* ims);
void imguiUpdate(struct ImGuiGLSetup* ims, f64 delta);
void imguiHandleInput(struct ImGuiGLSetup* ims, const SDL_Event& event);
struct ImFont* imguiLoadFont(const char* path, i32 sizePx);

