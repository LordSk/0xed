#pragma once
#include <SDL2/SDL.h>
#include "base.h"
#include "imgui_sdl2_setup.h"
#include "data_panel.h"

struct FileBuffer
{
    u8* data = nullptr;
    i64 size = 0;
};

struct AppWindow
{
    SDL_Window* window;
    SDL_GLContext glContext;
    bool running = true;

    ImGuiGLSetup* imguiSetup;

    FileBuffer curFileBuff;
    DataPanels dataPanels;

    i32 winWidth;
    i32 winHeight;
    i32 globalMouseX = -1;
    i32 globalMouseY = -1;
    u32 globalMouseState = 0;
    bool focused = true;

    ImFont* fontTimes;
    ImFont* fontMono;

    bool init();
    i32 run();
    void cleanUp();

    void doUI();

    void pushGlobalEvents();
    void handleEvent(const SDL_Event& event);
    void update();

    bool loadFile(const char* path);
};
