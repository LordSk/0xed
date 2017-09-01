#pragma once
#include <SDL2/SDL.h>
#include "base.h"

struct AppWindow
{
    SDL_Window* window;
    SDL_GLContext glContext;
    bool running = true;

    bool init();
    i32 run();
    void cleanUp();

    void handleEvent(const SDL_Event& event);
    void update();
};
