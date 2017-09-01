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
                              SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);

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
    fontCfg.pixel_snap = 1;
    fontCfg.oversample_h = 1;
    fontCfg.oversample_v = 1;

    nk_font* defaultFont = nk_font_atlas_add_from_file(atlas, "C:\\Windows\\Fonts\\tahoma.ttf",
                                                       13.5, &fontCfg);
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

    return true;
}

i32 AppWindow::run()
{
    while(running) {
        SDL_Event event;
        nk_input_begin(nkCtx);
        SDL_WaitEvent(&event);
        handleEvent(event);
        nk_sdl_handle_event(&event);
        nk_input_end(nkCtx);

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

void uiDrawHexByte(nk_context* ctx, u8 val, const struct nk_rect& rect, const struct nk_color& color)
{
    constexpr char base16[] = "0123456789ABCDEF";
    char hexStr[3];
    hexStr[0] = base16[val >> 4];
    hexStr[1] = base16[val & 15];
    hexStr[2] = 0;

    struct nk_window *win;
    const struct nk_style *style;

    struct nk_vec2 item_padding;
    struct nk_text text;

    win = ctx->current;
    style = &ctx->style;

    item_padding = style->text.padding;

    text.padding.x = item_padding.x;
    text.padding.y = item_padding.y;
    text.background = style->window.background;
    text.text = color;
    nk_widget_text(&win->buffer, rect, hexStr, 2, &text, NK_TEXT_ALIGN_CENTERED|NK_TEXT_ALIGN_MIDDLE,
                   style->font);
}

void AppWindow::doUI()
{
    constexpr i32 menubarHeight = 22;
    const nk_color colorContextMenuBg = nk_rgb(242, 242, 242);
    const nk_color colorContextMenuBorder = nk_rgb(204, 204, 204);

    constexpr i32 columnCount = 16;
    constexpr i32 columnWidth = 20;
    constexpr i32 columnHeaderHeight = 20;
    constexpr i32 rowCount = 100;
    constexpr i32 rowHeight = 20;
    constexpr i32 rowHeaderWidth = 22;

    if(nk_begin(nkCtx, "", nk_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), NK_WINDOW_BACKGROUND)) {

    // menu bar
    nk_menubar_begin(nkCtx);
    nk_layout_row_begin(nkCtx, NK_STATIC, menubarHeight, 2);

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

    i32 offY = menubarHeight;
    const i32 hexViewHeight = WINDOW_HEIGHT - offY;

    // hex view
    nk_fill_rect(&nkCtx->current->buffer, nk_rect(0, offY, rowHeaderWidth + columnWidth * columnCount,
                 columnHeaderHeight), 0, nk_rgb(240, 240, 240));
    nk_fill_rect(&nkCtx->current->buffer, nk_rect(0, offY, rowHeaderWidth, hexViewHeight),
                 0, nk_rgb(240, 240, 240));

    const struct nk_rect dataRect = nk_rect(rowHeaderWidth, offY + columnHeaderHeight,
                                            columnWidth * columnCount, rowHeight * 100);

    // column header horizontal line
    nk_stroke_line(&nkCtx->current->buffer, 0, dataRect.y,
                   dataRect.x + dataRect.w, dataRect.y,
                   1.0, nk_rgb(200, 200, 200));

    // vertical end line
    nk_stroke_line(&nkCtx->current->buffer, dataRect.x + dataRect.w,
                   0, dataRect.x + dataRect.w,
                   dataRect.h, 1.0, nk_rgb(200, 200, 200));

    // horizontal lines
    for(i32 r = 1; r < 100; ++r) {
        nk_stroke_line(&nkCtx->current->buffer, dataRect.x, dataRect.y + r * rowHeight,
                       dataRect.x + dataRect.w, dataRect.y + r * rowHeight,
                       1.0, nk_rgb(200, 200, 200));
    }

    // vertical lines
    for(i32 c = 0; c < columnCount; c += 4) {
        nk_stroke_line(&nkCtx->current->buffer, dataRect.x + c * columnWidth,
                       dataRect.y, dataRect.x + c * columnWidth,
                       dataRect.y + dataRect.h, 1.0, nk_rgb(200, 200, 200));
    }

    // line number
    for(i32 r = 0; r < 100; ++r) {
        uiDrawHexByte(nkCtx, r,
           nk_rect(0, offY + columnHeaderHeight + r * rowHeight, rowHeaderWidth, rowHeight),
           nk_rgb(150, 150, 150));
    }

    // column number
    for(i32 c = 0; c < columnCount; ++c) {
        uiDrawHexByte(nkCtx, c,
           nk_rect(c * columnWidth + rowHeaderWidth, offY, columnWidth, columnHeaderHeight),
           nk_rgb(150, 150, 150));
    }

    // data text
    for(i32 r = 0; r < 100; ++r) {
        for(i32 c = 0; c < columnCount; ++c) {
            uiDrawHexByte(nkCtx, curFileBuff.data[r * columnCount + c],
               nk_rect(c * columnWidth + rowHeaderWidth, r * rowHeight + columnHeaderHeight + offY,
                       columnWidth, rowHeight),
               nk_rgb(0, 0, 0));
        }
    }

    } nk_end(nkCtx);

    overview(nkCtx);
}

void AppWindow::handleEvent(const SDL_Event& event)
{
    if(event.type == SDL_QUIT) {
        running = false;
    }

    if(event.type == SDL_KEYDOWN) {
        if(event.key.keysym.sym == SDLK_ESCAPE) {
            running = false;
        }
    }
}

void AppWindow::update()
{
    doUI();

    glClearColor(1, 1, 1, 1);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
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
