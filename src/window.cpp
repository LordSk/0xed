#include <stdio.h>
#include <assert.h>
#include "window.h"
#include "imgui.h"

#define GL3W_IMPLEMENTATION
#include "gl3w.h"

#define WINDOW_TITLE "0xed [v0.0002]"
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

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

    imguiSetup = imguiInit(WINDOW_WIDTH, WINDOW_HEIGHT, "0xed_imgui");
    assert(imguiSetup);

    fontTimes = imguiLoadFont("C:\\Windows\\Fonts\\tahoma.ttf", 15);
    ImGui::GetIO().FontDefault = fontTimes;

    /*if(!loadFile("C:\\Program Files (x86)\\NAMCO BANDAI Games\\DarkSouls\\"
                 "dvdbnd0.bhd5.extract\\map\\MapStudio\\m18_01_00_00.msb")) {
        return false;
    }*/

    /*if(!loadFile("C:/Prog/Projects/0xed/build/genmu.mp3")) {
        return false;
    }*/

    if(!loadFile("C:/Prog/Projects/0xed/build/0xed.qbs")) {
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
        while(SDL_PollEvent(&event)) {
            eventCount++;
            handleEvent(event);
            imguiHandleInput(imguiSetup, event);
        }

        /*if(eventCount == 0) {
            SDL_Delay(1);
            continue;
        }*/

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

    imguiDeinit(imguiSetup);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
}

static void setStyleLight()
{
    ImGui::StyleColorsLight();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
}

void AppWindow::doUI()
{
    setStyleLight();

    constexpr i32 menubarRowHeight = 22;
    const ImColor colorContextMenuBg = ImColor(242, 242, 242);
    const ImColor colorContextMenuBorder = ImColor(204, 204, 204);
    constexpr i32 statusBarRowHeight = 22;

    i32 winWidth, winHeight;
    SDL_GL_GetDrawableSize(window, &winWidth, &winHeight);

    i32 menubarWinHeight = 0;
    i32 statusbarWinHeight = 0;

    ImGuiIO& io = ImGui::GetIO();
    io.FontDefault->FontSize;
    auto& style = ImGui::GetStyle();
    style.FramePadding.y;

    const i32 menuBarHeight = io.FontDefault->FontSize + style.FramePadding.y * 2.0;

    // menu bar
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("Open", "CTRL+O")) {

            }
            if(ImGui::MenuItem("Exit", "CTRL+X")) {

            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("About")) {
            if(ImGui::MenuItem("About 0xed", "")) {

            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // TODO: status bar?
#if 1
    // MAIN FRAME BEGIN
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    const ImVec2 mainFrameSize(winWidth, winHeight - menuBarHeight);
    ImGui::SetNextWindowSize(mainFrameSize);
    ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
    ImGui::SetNextWindowSizeConstraints(mainFrameSize, mainFrameSize);
    ImGui::Begin("Mainframe", nullptr,
                 ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|
                 ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImVec2 wsize = ImGui::GetWindowSize();
    dataPanels.doUi(Rect{0, 0, wsize.x, wsize.y});

    // MAIN FRAME END
    ImGui::End();
    ImGui::PopStyleVar(2);
#endif

    ImGui::ShowDemoWindow();
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
    ImGui::GetIO().DisplaySize = ImVec2(winWidth, winHeight);

    imguiUpdate(imguiSetup, 0);

    doUI();

    glClearColor(1, 1, 1, 1);
    glViewport(0, 0, winWidth, winHeight);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::Render();
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
