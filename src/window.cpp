#include <SDL2/SDL.h>
#include <stdio.h>
#include <assert.h>
#include "window.h"
#include "imgui.h"
#include "imgui_sdl2_setup.h"

void loadWindowIcon(SDL_Window* window)
{
#ifdef _WIN32
    const unsigned int mask_r = 0x00ff0000;
    const unsigned int mask_g = 0x0000ff00;
    const unsigned int mask_b = 0x000000ff;
    const unsigned int mask_a = 0xff000000;
    const int res_id = 101;
    const int size = 32;
    const int bpp = 32;

    HICON icon = (HICON)LoadImage(
        GetModuleHandle(NULL),
        MAKEINTRESOURCE(res_id),
        IMAGE_ICON,
        size, size,
        LR_SHARED
    );

    if(icon) {
        ICONINFO ici;

        if(GetIconInfo(icon, &ici)) {
            HDC dc = CreateCompatibleDC(NULL);

            if(dc) {
                SDL_Surface* surface = SDL_CreateRGBSurface(0, size, size, bpp, mask_r,
                                                            mask_g, mask_b, mask_a);

                if(surface) {
                    BITMAPINFO bmi;
                    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                    bmi.bmiHeader.biWidth = size;
                    bmi.bmiHeader.biHeight = -size;
                    bmi.bmiHeader.biPlanes = 1;
                    bmi.bmiHeader.biBitCount = bpp;
                    bmi.bmiHeader.biCompression = BI_RGB;
                    bmi.bmiHeader.biSizeImage = 0;

                    SelectObject(dc, ici.hbmColor);
                    GetDIBits(dc, ici.hbmColor, 0, size, surface->pixels, &bmi, DIB_RGB_COLORS);
                    SDL_SetWindowIcon(window, surface);
                    SDL_FreeSurface(surface);
                }
                DeleteDC(dc);
            }
            DeleteObject(ici.hbmColor);
            DeleteObject(ici.hbmMask);
        }
        DestroyIcon(icon);
    }
#endif
}

bool AppWindow::init(const char* title, i32 width, i32 height, bool maximized, const char* imgui_ini)
{
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    sdlWin = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              width, height,
                              SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|
                              SDL_WINDOW_ALLOW_HIGHDPI|
                              SDL_WINDOW_RESIZABLE|
                              (maximized ? SDL_WINDOW_MAXIMIZED : 0));

    winWidth = width;
    winHeight = height;
    winBeforeMaxWidth = winWidth;
    winBeforeMaxHeight = winHeight;

    if(!sdlWin) {
        LOG("ERROR: can't create SDL2 window (%s)",  SDL_GetError());
        return false;
    }

    loadWindowIcon(sdlWin);

    glContext = SDL_GL_CreateContext(sdlWin);
    if(!glContext) {
        LOG("ERROR: can't create OpenGL 3.3 context (%s)",  SDL_GetError());
        return false;
    }

    SDL_GL_SetSwapInterval(0);
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

    if(gl3w_init()) {
        LOG("ERROR: can't init gl3w");
        return false;
    }

    if(!gl3w_is_supported(3, 3)) {
        LOG("ERROR: OpenGL 3.3 isn't available on this system");
        return false;
    }

    glClearColor(0.15f, 0.15f, 0.15f, 1.f);
    glViewport(0, 0, width, height);

    imguiSetup = imguiInit(width, height, imgui_ini);
    assert(imguiSetup);

    // TODO: load multiple fonts at once
    // TODO: use free fonts (linux)?
    fontTimes = imguiLoadFont("C:\\Windows\\Fonts\\tahoma.ttf", 15);
    fontMono = imguiLoadFont("C:\\Windows\\Fonts\\consola.ttf", 15);
    ImGui::GetIO().FontDefault = fontTimes;

    return true;
}

void AppWindow::loop()
{
    u32 eventTick = SDL_GetTicks();

    while(running) {
        u32 frameStart = SDL_GetTicks();

        _computeGlobalMouseState();

        i32 eventCount = 0;
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            eventCount++;
            _handleEvent(event);
            imguiHandleInput(imguiSetup, event);
        }

        // window is not focused, sleep
        if(!focused) {
            SDL_Delay(15);
            continue;
        }

        // if the window is focused but we're idle, sleep
        if(eventCount > 0) {
            eventTick = SDL_GetTicks();
        }
        else {
            u32 now = SDL_GetTicks();
            if(now - eventTick > 10000) {
                SDL_Delay(100);
                continue;
            }
        }

        if(focused) {
            // update
            SDL_GL_GetDrawableSize(sdlWin, &winWidth, &winHeight);
            ImGui::GetIO().DisplaySize = ImVec2(winWidth, winHeight);
            imguiUpdate(imguiSetup, 0);

            assert(callbackUpdate);
            callbackUpdate();

            glViewport(0, 0, winWidth, winHeight);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui::Render();

            SDL_GL_SwapWindow(sdlWin);

            // limit framerate
            u32 frameTime = SDL_GetTicks() - frameStart;
            if(frameTime < 15) {
                SDL_Delay(15 - frameTime);
            }
        }
    }
}

void AppWindow::cleanUp()
{
    imguiDeinit(imguiSetup);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(sdlWin);
}

void AppWindow::_computeGlobalMouseState()
{
    if(!focused) {
        return;
    }

    i32 winx, winy;
    SDL_GetWindowPosition(sdlWin, &winx, &winy);
    i32 gmx, gmy;
    u32 gmstate = SDL_GetGlobalMouseState(&gmx, &gmy);

    defer(
        imguiSetMouseState(imguiSetup, globalMouseX - winx, globalMouseY - winy, globalMouseState);
    );

    if(gmx >= winx && gmx < winx+winWidth && gmy >= winy && gmy < winy+winHeight) {
        globalMouseX = gmx;
        globalMouseY = gmy;
        globalMouseState = gmstate;
        return;
    }

    if(gmx != globalMouseX || gmy != globalMouseY) {
        SDL_Event mouseEvent;
        mouseEvent.type = SDL_MOUSEMOTION;
        mouseEvent.motion.x = gmx - winx;
        mouseEvent.motion.y = gmy - winy;
        mouseEvent.motion.xrel = gmx - globalMouseX;
        mouseEvent.motion.yrel = gmy - globalMouseY;
        SDL_PushEvent(&mouseEvent);
    }
    globalMouseX = gmx;
    globalMouseY = gmy;

    // mouse left up outside window
    if((gmstate&SDL_BUTTON(SDL_BUTTON_LEFT)) != (globalMouseState&SDL_BUTTON(SDL_BUTTON_LEFT))) {
        SDL_Event mouseEvent;
        mouseEvent.type = gmstate&SDL_BUTTON(SDL_BUTTON_LEFT) > 0 ? SDL_MOUSEBUTTONDOWN: SDL_MOUSEBUTTONUP;
        mouseEvent.button.button = SDL_BUTTON_LEFT;
        SDL_PushEvent(&mouseEvent);
    }
    globalMouseState = gmstate;


    //LOG("mx=%d my=%d", gmx, gmy);
}

void AppWindow::_handleEvent(const SDL_Event& event)
{
    if(event.type == SDL_QUIT) {
        running = false;
        return;
    }

    if(event.type == SDL_KEYDOWN) {
#ifdef CONF_DEBUG
        if(event.key.keysym.sym == SDLK_ESCAPE) {
            running = false;
            return;
        }
#endif
    }

    if(event.type == SDL_WINDOWEVENT) {
        if(event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
            focused = true;
            return;
        }
        else if(event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
            focused = false;
            return;
        }
        if(event.window.event == SDL_WINDOWEVENT_MAXIMIZED) {
            winBeforeMaxWidth = winWidth;
            winBeforeMaxHeight = winHeight;
            return;
        }
    }

    assert(callbackEvent);
    callbackEvent(event);
}
