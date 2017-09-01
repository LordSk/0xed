#define SDL_MAIN_HANDLED

#define GL3W_IMPLEMENTATION
#include "gl3w.h"

#define NK_IMPLEMENTATION
#include "nuklear.h"

#define NK_SDL_GL3_IMPLEMENTATION
#include "nuklear_sdl_gl3.h"

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
    }
    i32 r = app.run();

    SDL_Quit();
    return r;
}
