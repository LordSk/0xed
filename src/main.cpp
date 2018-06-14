#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#ifdef _WIN32
    #include <windows.h>
#endif
#include <stdio.h>
#include "window.h"
#include "imgui_extended.h"
#include "data_panel.h"
#include "tools_panel.h"
#include "config.h"
#include "utils.h"
#include "bricks.h"
#include "script.h"
#include <easy/profiler.h>
#include <easy/reader.h>

#define GL3W_IMPLEMENTATION
#include "gl3w.h"

#define NOC_FILE_DIALOG_IMPLEMENTATION
#define NOC_FILE_DIALOG_WIN32
#include "noc_file_dialog.h"

#define CONFIG_FILENAME "0xed_config.ini"
#define WINDOW_BASE_TITLE "0xed"

#define POPUP_BRICK_ADD "Add brick"

struct Application {

AppWindow win;
Config config;
FileBuffer curFileBuff;
DataPanels dataPanels;
BrickWall brickWall;

f32 toolsPanelWidth = 400;

bool popupBrickWantOpen = false;
intptr_t popupBrickSelStart;
i64 popupBrickSelLength;

Script script;

bool init()
{
    loadConfigFile(CONFIG_FILENAME, &config);

    if(!win.init(WINDOW_BASE_TITLE, config.windowWidth, config.windowHeight,
                 config.windowMaximized, "0xed_imgui.ini")) {
        return false;
    }

    win.callbackUpdate = [this]() {
        this->update();
    };
    win.callbackEvent = [this](const SDL_Event& e) {
        this->handleEvent(e);
    };

    dataPanels.fileBuffer = 0;
    dataPanels.fileBufferSize = 0;
    dataPanels.fontMono = win.fontMono;
    dataPanels.panelCount = clamp(config.panelCount, 1, PANEL_MAX_COUNT);
    dataPanels.brickWall = &brickWall;

    if(openFileReadAll("C:\\Prog\\Projets\\sacred_remake\\sacred_data\\mixed.pak", &curFileBuff)) {
        dataPanels.setFileBuffer(curFileBuff.data, curFileBuff.size);
    }

    script.openAndCompile("../test_script.0");
    script.execute(&brickWall);

    return true;
}

void cleanUp()
{
    u32 winFlags = SDL_GetWindowFlags(win.sdlWin);

    config.windowMonitor = 0;
    config.windowMaximized = winFlags & SDL_WINDOW_MAXIMIZED ? 1:0;
    if(config.windowMaximized) {
        config.windowWidth = win.winBeforeMaxWidth;
        config.windowHeight = win.winBeforeMaxHeight;
    }
    else {
        config.windowWidth = win.winWidth;
        config.windowHeight = win.winHeight;
    }

    config.panelCount = dataPanels.panelCount;

    win.cleanUp();

    saveConfigFile(CONFIG_FILENAME, config);

    if(curFileBuff.data) {
        free(curFileBuff.data);
        curFileBuff.data = nullptr;
    }
}

i32 run()
{
    win.loop();
    cleanUp();
    return 0;
}

void update()
{
    EASY_FUNCTION(profiler::colors::Magenta);
    doUI();
}

void handleEvent(const SDL_Event& event)
{
    if(event.type == SDL_DROPFILE) {
        char* droppedFilepath = event.drop.file;

        if(openFileReadAll(droppedFilepath, &curFileBuff)) {
            dataPanels.setFileBuffer(curFileBuff.data, curFileBuff.size);
        }

        SDL_free(droppedFilepath);
        return;
    }

    if(event.type == SDL_KEYDOWN) {
        switch(event.key.keysym.sym) {
            case SDLK_b:
                userTryAddBrick();
                break;
        }
        return;
    }
}

void userTryAddBrick()
{
    if(dataPanels.selectionIsEmpty()) return;
    intptr_t selMin = min(dataPanels.selectionState.selectStart, dataPanels.selectionState.selectEnd);
    intptr_t selMax = max(dataPanels.selectionState.selectStart, dataPanels.selectionState.selectEnd);
    popupBrickSelStart = selMin;
    popupBrickSelLength = selMax - selMin + 1;
    popupBrickWantOpen = true;
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

    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
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
                win.exit();
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
    const ImRect winRect(0, menuBarHeight, win.winWidth, win.winHeight);
    ImGui::SetNextWindowPos(winRect.Min);
    ImGui::SetNextWindowSize(winRect.Max - winRect.Min);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::Begin("Mainframe", NULL, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|
                 ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar|
                 ImGuiWindowFlags_NoBringToFrontOnFocus);

    ImGui::SplitVBeginLeft("Mainframe_left", nullptr, &toolsPanelWidth);

        dataPanels.doUi();

    ImGui::SplitVBeginRight("Mainframe_right", nullptr, &toolsPanelWidth);

        const char* tabs[] = {
            "Inspector",
            "Bricks",
            "Scripts",
            "Options"
        };

        static i32 selectedTab = 0;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 5));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::Tabs("Inspector_tabs", tabs, IM_ARRAYSIZE(tabs), &selectedTab);
        ImGui::PopStyleVar(1);

        switch(selectedTab) {
            case 0:
                toolsDoInspector(dataPanels.fileBuffer, dataPanels.fileBufferSize,
                                 dataPanels.selectionState);
                break;
            case 1:
                ui_brickWall(&brickWall, curFileBuff.data);
                break;
            case 2:
                toolsDoScript(&script, &brickWall);
                break;
            case 3:
                toolsDoOptions(&dataPanels.columnCount);
                break;
        }

        ImGui::PopStyleVar(1); // ItemSpacing

    ImGui::SplitVEnd();

    ImGui::End();
    ImGui::PopStyleVar(3);

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

    if(popupBrickWantOpen) {
        popupBrickWantOpen = false;
        ImGui::OpenPopup(POPUP_BRICK_ADD);
    }

    ui_brickPopup(POPUP_BRICK_ADD, popupBrickSelStart, popupBrickSelLength, &brickWall);
    //ImGui::ShowDemoWindow();
}

};

#ifdef _WIN32
int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
#else
int main()
#endif
{
    SDL_SetMainReady();

    EASY_PROFILER_ENABLE;
    EASY_MAIN_THREAD;
    profiler::startListen();

    i32 sdl = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS);
    if(sdl != 0) {
        LOG("ERROR: could not init SDL2 (%s)", SDL_GetError());
        return 1;
    }

    Application app;
    if(!app.init()) {
        LOG("ERROR: could not init application.");
        return 1;
    }
    i32 r = app.run();

    SDL_Quit();

    //profiler::stopListen();
    profiler::dumpBlocksToFile("0XED_PROFILE.prof");
    return r;
}
