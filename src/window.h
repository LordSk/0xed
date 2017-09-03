#pragma once
#include <SDL2/SDL.h>
#include "base.h"
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

    struct nk_context* nkCtx;

    FileBuffer curFileBuff;
    DataPanels dataPanels;

    bool init();
    i32 run();
    void cleanUp();

    void doUI();

    void handleEvent(const SDL_Event& event);
    void update();

    bool loadFile(const char* path);
};

enum theme {THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK};
void setStyle(struct nk_context* ctx, enum theme theme);
void setStyleWhite(nk_context* ctx);
