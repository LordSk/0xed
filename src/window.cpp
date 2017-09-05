#include <stdio.h>
#include <assert.h>
#include "window.h"

#define GL3W_IMPLEMENTATION
#include "gl3w.h"

#define NK_IMPLEMENTATION
#include "nuklear.h"

#define NK_SDL_GL3_IMPLEMENTATION
#include "nuklear_sdl_gl3.h"
#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#define WINDOW_TITLE "0xed [v0.0002]"
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

#include "overview.c"

bool AppWindow::init()
{
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(WINDOW_TITLE,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|
                              SDL_WINDOW_ALLOW_HIGHDPI|
                              SDL_WINDOW_RESIZABLE);

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
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    nkCtx = nk_sdl_init(window);
    assert(nkCtx);

    nk_font_atlas* atlas;
    nk_sdl_font_stash_begin(&atlas);

    struct nk_font_config fontCfg = nk_font_config(14);
    fontCfg.pixel_snap = 0;
    fontCfg.oversample_h = 2;
    fontCfg.oversample_v = 2;

    nk_font* defaultFont = nk_font_atlas_add_from_file(atlas, "C:\\Windows\\Fonts\\tahoma.ttf",
                                                       14, &fontCfg);
    if(!defaultFont) {
        LOG("ERROR: could not load segoeui.ttf");
        return false;
    }

    nk_sdl_font_stash_end();
    nk_style_set_font(nkCtx, &defaultFont->handle);

    setStyleWhite(nkCtx);

    if(!loadFile("C:\\Program Files (x86)\\NAMCO BANDAI Games\\DarkSouls\\"
              "dvdbnd0.bhd5.extract\\map\\MapStudio\\m18_01_00_00.msb")) {
        return false;
    }

    dataPanels.data = curFileBuff.data;
    dataPanels.dataSize = curFileBuff.size;

    return true;
}

i32 AppWindow::run()
{
    while(running) {
        SDL_GetWindowSize(window, &winWidth, &winHeight);
        pushGlobalEvents();

        i32 eventCount = 0;
        SDL_Event event;
        nk_input_begin(nkCtx);
        while(SDL_PollEvent(&event)) {
            eventCount++;
            handleEvent(event);
            nk_sdl_handle_event(&event);
        }
        nk_input_end(nkCtx);

        if(eventCount == 0) {
            SDL_Delay(1);
            continue;
        }

        update();

        SDL_GL_SwapWindow(window);
    }

    cleanUp();

    return 0;
}

void AppWindow::cleanUp()
{
    if(curFileBuff.data) {
        free(curFileBuff.data);
        curFileBuff.data = nullptr;
    }

    nk_sdl_shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
}

void AppWindow::doUI()
{
    constexpr i32 menubarRowHeight = 22;
    const nk_color colorContextMenuBg = nk_rgb(242, 242, 242);
    const nk_color colorContextMenuBorder = nk_rgb(204, 204, 204);
    constexpr i32 statusBarRowHeight = 22;

    i32 winWidth, winHeight;
    SDL_GL_GetDrawableSize(window, &winWidth, &winHeight);
    winWidth += 1;
    winHeight += 1;

    i32 menubarWinHeight = 0;
    i32 statusbarWinHeight = 0;

    // menu bar
    if(nk_begin(nkCtx, "menu_bar", nk_rect(0, 0, winWidth, menubarRowHeight), NK_WINDOW_NO_SCROLLBAR)) {
        nk_menubar_begin(nkCtx);
        nk_layout_row_begin(nkCtx, NK_STATIC, menubarRowHeight, 2);

        nk_style_push_color(nkCtx, &nkCtx->style.window.background, colorContextMenuBg);
        nk_style_push_color(nkCtx, &nkCtx->style.window.border_color, colorContextMenuBorder);

        nk_layout_row_push(nkCtx, 50);
        if(nk_menu_begin_label(nkCtx, "File", NK_TEXT_CENTERED, nk_vec2(120, 200))) {
            nk_layout_row_dynamic(nkCtx, 25, 1);
            nk_menu_item_label(nkCtx, "  Open", NK_TEXT_LEFT);
            nk_menu_item_label(nkCtx, "  Exit", NK_TEXT_LEFT);
            nk_menu_end(nkCtx);
        }

        nk_layout_row_push(nkCtx, 50);
        if(nk_menu_begin_label(nkCtx, "About", NK_TEXT_CENTERED, nk_vec2(120, 200))) {
            nk_menu_end(nkCtx);
        }

        nk_style_pop_color(nkCtx); // colorContextMenuBorder
        nk_style_pop_color(nkCtx); // colorContextMenuBg

        nk_layout_row_end(nkCtx);
        nk_menubar_end(nkCtx);
        // --- menubar

        Rect r = nk_window_get_bounds(nkCtx);
        menubarWinHeight = r.h;

    } nk_end(nkCtx);

    // status bar
    Rect statusBarRect = nk_rect(0,
                                 winHeight - statusBarRowHeight,
                                 winWidth,
                                 statusBarRowHeight);

    nk_style* style = &nkCtx->style;
    nk_style_push_style_item(nkCtx, &style->window.fixed_background,
                             nk_style_item_color(nk_rgb(240, 240, 240)));

    if(nk_begin(nkCtx, "status_bar", statusBarRect, NK_WINDOW_NO_SCROLLBAR)) {
        nk_layout_row_begin(nkCtx, NK_STATIC, statusBarRowHeight, 2);

        nk_layout_row_push(nkCtx, 50);
        nk_label(nkCtx, "Ready.", NK_TEXT_ALIGN_LEFT|NK_TEXT_ALIGN_MIDDLE);

        nk_layout_row_end(nkCtx);

        Rect r = nk_window_get_bounds(nkCtx);
        statusbarWinHeight = r.h;

        // upper border
        struct nk_command_buffer* canvas = nk_window_get_canvas(nkCtx);
        nk_stroke_line(canvas, 0, statusBarRect.y+1,
                       statusBarRect.w, statusBarRect.y+1,
                       1.0, nk_rgb(215, 215, 215));

    } nk_end(nkCtx);

    nk_style_pop_style_item(nkCtx);

    const i32 middleHeight = winHeight - menubarWinHeight - statusbarWinHeight;
    dataPanels.uiDataPanels(nkCtx, nk_rect(0, menubarWinHeight, winWidth, middleHeight));

    //overview(nkCtx);
}

void AppWindow::pushGlobalEvents()
{
    // TODO: check if top most window
    if(!focused) {
        return;
    }

    i32 winx, winy;
    SDL_GetWindowPosition(window, &winx, &winy);
    i32 gmx, gmy;
    u32 gmstate = SDL_GetGlobalMouseState(&gmx, &gmy);

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

void AppWindow::handleEvent(const SDL_Event& event)
{
    if(event.type == SDL_QUIT) {
        running = false;
        return;
    }

    if(event.type == SDL_KEYDOWN) {
        if(event.key.keysym.sym == SDLK_ESCAPE) {
            running = false;
            return;
        }
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
    }
}

void AppWindow::update()
{
    doUI();

    glClearColor(1, 1, 1, 1);
    glViewport(0, 0, winWidth, winHeight);
    glClear(GL_COLOR_BUFFER_BIT);

    /* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
     * with blending, scissor, face culling, depth test and viewport and
     * defaults everything back into a default state.
     * Make sure to either a.) save and restore or b.) reset your own state after
     * rendering the UI. */
    nk_sdl_render(NK_ANTI_ALIASING_OFF, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
}

bool AppWindow::loadFile(const char* path)
{
    FILE* file = fopen(path, "rb");
    if(!file) {
        LOG("ERROR: Can not open file %s", path);
        return false;
    }

    if(curFileBuff.data) {
        free(curFileBuff.data);
        curFileBuff.data = nullptr;
        curFileBuff.size = 0;
    }

    FileBuffer fb;

    i64 start = ftell(file);
    fseek(file, 0, SEEK_END);
    i64 len = ftell(file) - start;
    fseek(file, start, SEEK_SET);

    fb.data = (u8*)malloc(len + 1);
    assert(fb.data);
    fb.size = len;

    // read
    fread(fb.data, 1, len, file);
    fb.data[len] = 0;

    fclose(file);
    curFileBuff = fb;

    LOG("file loaded path=%s size=%llu", path, curFileBuff.size);

    return true;
}
