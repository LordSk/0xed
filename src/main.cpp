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
#include "search.h"
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
SearchParams searchParams = {};
SearchParams lastSearchParams = {};
Array<SearchResult> searchResults;

bool init()
{
    // TODO: remove this
    /*if(!script.openAndCompile("../test_script.0")) {
        return false;
    }
    return false;*/

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
		// TODO: onFileLoaded
    }

    searchResults.reserve(1024);
    searchStartThread();
    searchSetNewFileBuffer(curFileBuff);

    lastSearchParams.dataType = SearchDataType::Integer;
    lastSearchParams.intSigned = true;
    lastSearchParams.vint = 0;
    lastSearchParams.dataSize = 4;
    lastSearchParams.strideKind = SearchParams::Stride::Even;
    searchNewRequest(lastSearchParams, &searchResults);

    /*u8* fileData = (u8*)malloc(256);
    curFileBuff.data = fileData;
    curFileBuff.size = 256;
    dataPanels.setFileBuffer(curFileBuff.data, curFileBuff.size);

    for(i32 i = 0; i < 256; i++) {
        fileData[i] = i;
    }*/

    /*if(!script.execute(&brickWall)) {
        return false;
    }*/

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
			searchSetNewFileBuffer(curFileBuff);
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
    style.GrabMinSize = 20.0f;
	style.GrabRounding = 7.5f;
    style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 7.5f;
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.88, 0.88, 0.9, 1.0);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.5, 0.5, 0.5, 1.0);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(102/255.0, 178/255.0, 1.0, 1.0);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(36/255.0, 134/255.0, 1.0, 1.0);
}

void doUI()
{
    setStyleLight();

    ImGuiIO& io = ImGui::GetIO();

    // TODO: status bar?
    // MAIN FRAME BEGIN
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	ImGui::Begin("Mainframe", nullptr, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspaceMain = ImGui::GetID("Dockspace_main");
	ImGui::DockSpace(dockspaceMain, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
	const ImVec2 mainDockSize = ImGui::GetContentRegionAvail();

	// menu bar
	bool openGoto = false;
	bool openSearch = false;

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
			if(ImGui::MenuItem("Search", "CTRL+F")) {
				openSearch = true;
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

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Hex view", nullptr, 0/*|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|
				 ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar*/);
	ImGui::PopStyleVar(1);

        dataPanels.doUi();

	ImGui::End();

	// Tool windows
	// Inspector
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Inspector", nullptr, 0/*|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|
				 ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar*/);
	ImGui::PopStyleVar(1);

		toolsDoInspector(dataPanels.fileBuffer, dataPanels.fileBufferSize,
						 dataPanels.selectionState);
	ImGui::End();

	// Search
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Search", nullptr, 0/*|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|
				 ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar*/);
	ImGui::PopStyleVar(1);

		// search params
		if(toolsSearchParams(&searchParams)) {
			searchResults.clear();
			lastSearchParams = searchParams;
			searchNewRequest(lastSearchParams, &searchResults);
		}
		// search results here
		u64 searchGotoOffset;
		if(toolsSearchResults(lastSearchParams, searchResults, &searchGotoOffset)) {
			dataPanels.goTo(searchGotoOffset);
			dataPanels.select(searchGotoOffset, searchGotoOffset + lastSearchParams.dataSize - 1);
		}
		ImGuiWindow* searchWindow = ImGui::GetCurrentWindowRead();

	ImGui::End();

	// Brick wall
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Bricks", nullptr, 0/*|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|
				 ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar*/);
	ImGui::PopStyleVar(1);

		ui_brickWall(&brickWall, curFileBuff.data);

	ImGui::End();

	// Scripts
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Scripts", nullptr, 0/*|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|
				 ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar*/);
	ImGui::PopStyleVar(1);

		toolsDoOptions(&dataPanels.columnCount);
		toolsDoScript(&script, &brickWall);

	ImGui::End();

	// Default layout (applies after first frame (sizes are correct afer the first frame))
	if(!ImGui::DockBuilderGetNode(dockspaceMain)->IsSplitNode() && ImGui::GetFrameCount() > 1) {
		ImGuiID dockspaceMainLeft, dockspaceMainRight;
		/*ImGui::DockBuilderRemoveNode(dockspaceMain); // Clear out existing layout
		ImGui::DockBuilderAddNode(dockspaceMain, ImGuiDockNodeFlags_Dockspace); // Add empty node*/
		ImGui::DockBuilderSetNodeSize(dockspaceMain, mainDockSize);

		ImGui::DockBuilderSplitNode(dockspaceMain, ImGuiDir_Right, 0.30f,
									&dockspaceMainRight, &dockspaceMainLeft);

		ImGui::DockBuilderDockWindow("Hex view", dockspaceMainLeft);
		ImGui::DockBuilderDockWindow("Inspector", dockspaceMainRight);
		ImGui::DockBuilderDockWindow("Search", dockspaceMainRight);
		ImGui::DockBuilderDockWindow("Bricks", dockspaceMainRight);
		ImGui::DockBuilderDockWindow("Scripts", dockspaceMainRight);
		ImGui::DockBuilderFinish(dockspaceMain);
	}

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

    // TODO: remove
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
