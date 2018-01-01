#define SDL_MAIN_HANDLED
#include <stdio.h>
#include "window.h"

int main()
{
    i32 sdl = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS);
    if(sdl != 0) {
        LOG("ERROR: could not init SDL2 (%s)", SDL_GetError());
        return 1;
    }

    AppWindow app;
    if(!app.init()) {
        LOG("ERROR: could not init application.");
        return 1;
    }
    i32 r = app.run();

    SDL_Quit();
    return r;
}
