#include <stdio.h>
#include "window.h"
#include "gl3w.h"

#define WINDOW_TITLE "0xed [v0.0002]"
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

bool AppWindow::init()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    window = SDL_CreateWindow(WINDOW_TITLE,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_OPENGL);

    if(!window) {
        LOG("ERROR: can't create SDL2 window (%s)",  SDL_GetError());
        return false;
    }

    glContext = SDL_GL_CreateContext(window);
    if(!glContext) {
        LOG("ERROR: can't create OpenGL 3.3 context (%s)",  SDL_GetError());
        return false;
    }

    SDL_GL_SetSwapInterval(0);

    if(gl3w_init()) {
        LOG("ERROR: can't init gl3w");
        return false;
    }

    if(!gl3w_is_supported(3, 3)) {
        LOG("ERROR: OpenGL 3.3 isn't available on this system");
        return false;
    }

    glClearColor(0.15f, 0.15f, 0.15f, 1.f);

    return true;
}

i32 AppWindow::run()
{
    while(running) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        handleEvent(event);
        update();

        SDL_GL_SwapWindow(window);
    }

    cleanUp();

    return 0;
}

void AppWindow::cleanUp()
{
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
}

void AppWindow::handleEvent(const SDL_Event& event)
{
    if(event.type == SDL_QUIT) {
        running = false;
    }
}

void AppWindow::update()
{
    glClear(GL_COLOR_BUFFER_BIT);
}
