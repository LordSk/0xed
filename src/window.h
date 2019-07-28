#pragma once
#include "base.h"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>
#include <functional>

struct AppWindow
{
    struct SDL_Window* sdlWin;
    SDL_GLContext glContext;
    bool running = true;

    struct ImGuiGLSetup* imguiSetup;
    struct ImFont* fontTimes;
    struct ImFont* fontMono;

    i32 winX;
    i32 winY;
    i32 winWidth;
    i32 winHeight;
    i32 winBeforeMaxWidth;
    i32 winBeforeMaxHeight;
    i32 globalMouseX = -1;
    i32 globalMouseY = -1;
    u32 globalMouseState = 0;
    bool focused = true;

    std::function<void()> callbackUpdate = nullptr;
    std::function<void(const SDL_Event&)> callbackEvent = nullptr;

	SDL_Cursor* cursorDefault;
	SDL_Cursor* cursorWait;

    bool init(const char* title, i32 width, i32 height, bool maximized, const char* imgui_ini);
    void cleanUp();
    void loop();

	void setTitle(const char* pTitle);
	void setCursorDefault();
	void setCursorWait();

    inline void exit() {
        running = false;
    }

    void _computeGlobalMouseState();
    void _handleEvent(const SDL_Event& event);
};
