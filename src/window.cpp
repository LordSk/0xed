#include <SDL2/SDL.h>
#include <stdio.h>
#include <assert.h>
#include "window.h"
#include "imgui.h"
#include "imgui_sdl2_setup.h"
#include "imgui_extended.h"
#include "data_panel.h"
#include "tools_panel.h"
#include "config.h"

#define GL3W_IMPLEMENTATION
#include "gl3w.h"

#define NOC_FILE_DIALOG_IMPLEMENTATION
#define NOC_FILE_DIALOG_WIN32
#include "noc_file_dialog.h"

#define CONFIG_FILENAME "0xed_config.ini"
#define WINDOW_BASE_TITLE "0xed"

struct AppWindow {

SDL_Window* window;
SDL_GLContext glContext;
bool running = true;

Config config;

ImGuiGLSetup* imguiSetup;

FileBuffer curFileBuff;
DataPanels dataPanels;

i32 winX;
i32 winY;
i32 winWidth;
i32 winHeight;
i32 winBeforeMaxWidth;
i32 winBeforeMaxHeight;
i32 globalMouseX = -1;
i32 globalMouseY = -1;
u32 globalMouseState = 0;
bool focused = true;

ImFont* fontTimes;
ImFont* fontMono;

f32 toolsPanelWidth = 400;

bool init()
{
    loadConfigFile(CONFIG_FILENAME, &config);

    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow(WINDOW_BASE_TITLE,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              config.windowWidth, config.windowHeight,
                              SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|
                              SDL_WINDOW_ALLOW_HIGHDPI|
                              SDL_WINDOW_RESIZABLE|
                              (config.windowMaximized ? SDL_WINDOW_MAXIMIZED : 0));

    winWidth = config.windowWidth;
    winHeight = config.windowHeight;
    winBeforeMaxWidth = winWidth;
    winBeforeMaxHeight = winHeight;

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
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    imguiSetup = imguiInit(WINDOW_WIDTH, WINDOW_HEIGHT, "0xed_imgui.ini");
    assert(imguiSetup);

    // TODO: load multiple fonts at once
    // TODO: use free fonts (linux)?
    fontTimes = imguiLoadFont("C:\\Windows\\Fonts\\tahoma.ttf", 15);
    fontMono = imguiLoadFont("C:\\Windows\\Fonts\\consola.ttf", 15);
    ImGui::GetIO().FontDefault = fontTimes;

    dataPanels.fileBuffer = 0;
    dataPanels.fileBufferSize = 0;
    dataPanels.fontMono = fontMono;
    dataPanels.panelCount = clamp(config.panelCount, 1, PANEL_MAX_COUNT);

    if(openFileReadAll("C:\\Program Files (x86)\\NAMCO BANDAI Games\\DarkSouls\\"
                 "dvdbnd0.bhd5.extract\\map\\MapStudio\\m18_01_00_00.msb", &curFileBuff)) {
        dataPanels.setFileBuffer(curFileBuff.data, curFileBuff.size);
    }

    return true;
}

i32 run()
{
    u32 eventTick = SDL_GetTicks();

    while(running) {
        u32 frameStart = SDL_GetTicks();

        computeGlobalMouseState();

        i32 eventCount = 0;
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            eventCount++;
            handleEvent(event);
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
            update();
            SDL_GL_SwapWindow(window);

            // limit framerate
            u32 frameTime = SDL_GetTicks() - frameStart;
            if(frameTime < 15) {
                SDL_Delay(15 - frameTime);
            }
        }
    }

    cleanUp();

    return 0;
}

void cleanUp()
{
    u32 winFlags = SDL_GetWindowFlags(window);

    config.windowMonitor = 0;
    config.windowMaximized = winFlags & SDL_WINDOW_MAXIMIZED ? 1:0;
    if(config.windowMaximized) {
        config.windowWidth = winBeforeMaxWidth;
        config.windowHeight = winBeforeMaxHeight;
    }
    else {
        config.windowWidth = winWidth;
        config.windowHeight = winHeight;
    }

    config.panelCount = dataPanels.panelCount;

    saveConfigFile(CONFIG_FILENAME, config);

    imguiDeinit(imguiSetup);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);

    if(curFileBuff.data) {
        free(curFileBuff.data);
        curFileBuff.data = nullptr;
    }
}

void computeGlobalMouseState()
{
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

void handleEvent(const SDL_Event& event)
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

    if(event.type == SDL_DROPFILE) {
        char* droppedFilepath = event.drop.file;

        if(openFileReadAll(droppedFilepath, &curFileBuff)) {
            dataPanels.setFileBuffer(curFileBuff.data, curFileBuff.size);
        }

        SDL_free(droppedFilepath);
        return;
    }
}

void update()
{
    ImGui::GetIO().DisplaySize = ImVec2(winWidth, winHeight);

    imguiUpdate(imguiSetup, 0);

    doUI();

    glViewport(0, 0, winWidth, winHeight);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::Render();
}


static void setStyleLight()
{
    ImGui::StyleColorsLight();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
}

void doUI()
{
    setStyleLight();

    SDL_GL_GetDrawableSize(window, &winWidth, &winHeight);

    ImGuiIO& io = ImGui::GetIO();
    auto& style = ImGui::GetStyle();
    const i32 menuBarHeight = io.FontDefault->FontSize + style.FramePadding.y * 2.0;

    bool openGoto = false;

    // menu bar
    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("Open", "CTRL+O")) {
                const char* filepath = noc_file_dialog_open(NOC_FILE_DIALOG_OPEN, "*", "", "");
                if(filepath) {
                    if(openFileReadAll(filepath, &curFileBuff)) {
                        dataPanels.setFileBuffer(curFileBuff.data, curFileBuff.size);
                    }
                }
            }
            if(ImGui::MenuItem("Exit", "CTRL+X")) {
                running = false;
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("About")) {
            if(ImGui::MenuItem("About 0xed", "")) {

            }
            ImGui::EndMenu();
        }

        if(ImGui::Button("Add panel")) {
            dataPanels.addNewPanel();
        }

        if(ImGui::Button("Goto")) {
            openGoto = true;
        }

        ImGui::EndMainMenuBar();
    }

    // TODO: status bar?
    // MAIN FRAME BEGIN
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    const ImVec2 mainFrameSize(winWidth, winHeight - menuBarHeight);
    ImGui::SetNextWindowSize(mainFrameSize);
    ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
    ImGui::SetNextWindowSizeConstraints(mainFrameSize, mainFrameSize);
    ImGui::Begin("Mainframe", nullptr,
                 ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|
                 ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoBringToFrontOnFocus/*|
                 ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoScrollWithMouse*/);

        // START MAIN LEFT (DATA PANELS)
        ImGui::SplitVBeginLeft("Mainframe_left", nullptr, &toolsPanelWidth);

            dataPanels.doUi();

        ImGui::SplitVBeginRight("Mainframe_right", nullptr, &toolsPanelWidth);

            const char* tabs[] = {
                "Inspector",
                "Template",
                "Output"
            };

            static i32 selectedTab = 0;
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 5));
            ImGui::Tabs("Inspector_tabs", tabs, IM_ARRAYSIZE(tabs), &selectedTab);
            ImGui::PopStyleVar(1);

            ImGui::BeginChild("#tab_content", ImVec2(0, 0));

            switch(selectedTab) {
                case 0:
                    toolsDoInspector(dataPanels.fileBuffer, dataPanels.fileBufferSize,
                                     dataPanels.selectionState);
                    break;
                case 1:
                    toolsDoTemplate();
                    break;
                case 2:
                    ImGui::Text("tab2 selected");
                    break;
            }

            ImGui::EndChild();

        ImGui::SplitVEnd();

    // MAIN FRAME END
    ImGui::End();
    ImGui::PopStyleVar(2);

    static i32 gotoOffset = 0;
    if(openGoto) {
        ImGui::OpenPopup("Go to file offset");
        gotoOffset = dataPanels.getSelectedInt();
    }
    if(ImGui::BeginPopupModal("Go to file offset", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Go to");
        ImGui::Separator();

        ImGui::InputInt("##offset", &gotoOffset);

        if(ImGui::Button("OK", ImVec2(120,0))) {
            dataPanels.goTo(gotoOffset);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel", ImVec2(120,0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }

    ImGui::ShowDemoWindow();
}

};

static AppWindow app;

bool applicationInit()
{
    return app.init();
}

i32 applicationRun()
{
    return app.run();
}
