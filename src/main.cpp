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

	// Search
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Search", nullptr, 0/*|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|
				 ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoScrollbar*/);
	ImGui::PopStyleVar(1);

		// search results here
		u64 searchGotoOffset;
		if(toolsSearchResults(lastSearchParams, searchResults, &searchGotoOffset)) {
			dataPanels.goTo(searchGotoOffset);
			dataPanels.select(searchGotoOffset, searchGotoOffset + lastSearchParams.dataSize - 1);
		}
		ImGuiWindow* searchWindow = ImGui::GetCurrentWindowRead();

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
		ImGui::DockBuilderDockWindow("Bricks", dockspaceMainRight);
		ImGui::DockBuilderDockWindow("Scripts", dockspaceMainRight);
		ImGui::DockBuilderDockWindow("Search", dockspaceMainRight);
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

    if(doSearchPopup(openSearch, &searchParams)) {
        searchResults.clear();
        lastSearchParams = searchParams;
        searchNewRequest(lastSearchParams, &searchResults);
		ImGui::FocusWindow(searchWindow);
    }

    // TODO: remove
    if(popupBrickWantOpen) {
        popupBrickWantOpen = false;
        ImGui::OpenPopup(POPUP_BRICK_ADD);
    }

    ui_brickPopup(POPUP_BRICK_ADD, popupBrickSelStart, popupBrickSelLength, &brickWall);

	//ImGui::ShowDemoWindow();
}

bool doSearchPopup(bool open, SearchParams* params)
{
    bool doSearch = false;

    if(open) {
        ImGui::OpenPopup("Search data item");
        ImGui::SetNextWindowPos(ImVec2(200, 200)); // TODO: does not work
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15, 15));
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));

    if(ImGui::BeginPopupModal("Search data item", nullptr,
                    ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoCollapse)) {

        const char* dataTypeComboItems[] = {
            "ASCII String",
            "Integer",
            "Float",
        };

        ImGui::ButtonListOne("##comboDataType", dataTypeComboItems, arr_count(dataTypeComboItems),
                             (i32*)&params->dataType);
        ImGui::Separator();

        switch(params->dataType) {
            case SearchDataType::ASCII_String: {
                ImGui::Text("String:");
                ImGui::InputText("##searchString", params->str, sizeof(params->str));
                params->dataSize = strlen(params->str);
                params->strideKind = SearchParams::Stride::Full;
            } break;

            case SearchDataType::Integer: {
                static i32 bitSizeItemId = 2;
                static i32 searchStrideId = 2;


                ImGui::Text("Integer size (in bits):");
                const char* bitSizeList[] = {
                    "8",
                    "16",
                    "32",
                    "64",
                };

                ImGui::ButtonListOne("byte_size_select", bitSizeList, arr_count(bitSizeList),
                                     &bitSizeItemId, ImVec2(200, 0));

                ImGui::Text("Search stride:");
                const char* strideSelectList[] = {
                    "Full",
                    "Twice",
                    "Even",
                };
                ImGui::ButtonListOne("stride_select", strideSelectList, arr_count(strideSelectList),
                                     &searchStrideId, ImVec2(200, 0));

                static bool signed_ = true;
                ImGui::Checkbox("Signed", &signed_);

                ImGui::Text("Integer:");
                const i32 step = 1;
                const i32 stepFast = 100;

                ImGuiDataType_ intType = bitSizeItemId == 3 ? ImGuiDataType_S64: ImGuiDataType_S32;
                char* format = bitSizeItemId == 3 ? "%lld": "%d";
                void* searchInt = &params->vint;
                if(!signed_) {
                    intType = bitSizeItemId == 3 ? ImGuiDataType_U64: ImGuiDataType_U32;
                    format = bitSizeItemId == 3 ? "%llu": "%u";
                    searchInt = &params->vuint;
                }

                params->dataSize = 1 << bitSizeItemId;
                params->strideKind = (SearchParams::Stride)searchStrideId;

                ImGui::InputScalar("##int_input", intType, searchInt, &step,
                                   &stepFast, format);
            } break;

            case SearchDataType::Float: {
                static i32 bitSizeItemId = 0;
                static i32 searchStrideId = 2;


                ImGui::Text("Float size (in bits):");
                const char* bitSizeList[] = {
                    "32",
                    "64",
                };

                ImGui::ButtonListOne("byte_size_select", bitSizeList, arr_count(bitSizeList),
                                     &bitSizeItemId, ImVec2(100, 0));

                ImGui::Text("Search stride:");
                const char* strideSelectList[] = {
                    "Full",
                    "Twice",
                    "Even",
                };
                ImGui::ButtonListOne("stride_select", strideSelectList, arr_count(strideSelectList),
                                     &searchStrideId, ImVec2(200, 0));

                ImGui::Text("Float:");

                const f64 step = 1;
                const f64 stepFast = 10;
                ImGuiDataType_ fltType = bitSizeItemId == 0 ? ImGuiDataType_Float: ImGuiDataType_Double;
                char* format = bitSizeItemId == 0 ? "%.5f": "%.10f";
                void* fltVal = bitSizeItemId == 0 ? &params->vf32: &params->vf64;

                params->dataSize = bitSizeItemId == 0 ? 4 : 8;
                params->strideKind = (SearchParams::Stride)searchStrideId;

                ImGui::InputScalar("##flt_input", fltType, fltVal, &step,
                                   &stepFast, format);
            } break;

            default: assert(0); break;
        }

        if(ImGui::Button("Search", ImVec2(120,0))) {
            doSearch = true;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel", ImVec2(120,0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::PopStyleVar(3);
    return doSearch;
}

bool toolsSearchResults(const SearchParams& params, const Array<SearchResult>& results, u64* gotoOffset)
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    switch(params.dataType) {
        case SearchDataType::ASCII_String: {
            const f32 typeFrameLen = ImGui::CalcTextSize("ASCII").x + 20.0f;
            const f32 searchTermFrameLen = ImGui::GetContentRegionAvail().x - typeFrameLen;

            ImGui::TextBox(0xffdfdfdf, 0xff000000, ImVec2(typeFrameLen, 35), ImVec2(0.5, 0.5), ImVec2(0, 0),
                           "ASCII");
            ImGui::SameLine();
            ImGui::TextBox(0xffffefef, 0xffff0000, ImVec2(searchTermFrameLen, 35),
                           ImVec2(0, 0.5), ImVec2(10, 0),
                           "%.*s", params.dataSize, params.str);
        } break;

        case SearchDataType::Integer: {
            if(params.intSigned) {
                const f32 typeFrameLen = ImGui::CalcTextSize("Integer").x + 20.0f;
                const f32 searchTermFrameLen = ImGui::GetContentRegionAvail().x - typeFrameLen;

                ImGui::TextBox(0xffdfdfdf, 0xff000000, ImVec2(typeFrameLen, 35), ImVec2(0.5, 0.5),
                               ImVec2(0, 0), "Integer");
                ImGui::SameLine();
                ImGui::TextBox(0xffffefef, 0xffff0000, ImVec2(searchTermFrameLen, 35), ImVec2(0, 0.5),
                               ImVec2(10, 0),
                               "%d", params.vint);
            }
            else {
                const f32 typeFrameLen = ImGui::CalcTextSize("Unsigned integer").x + 20.0f;
                const f32 searchTermFrameLen = ImGui::GetContentRegionAvail().x - typeFrameLen;

                ImGui::TextBox(0xffdfdfdf, 0xff000000, ImVec2(typeFrameLen, 35), ImVec2(0.5, 0.5),
                               ImVec2(0, 0), "Unsigned integer");
                ImGui::SameLine();
                ImGui::TextBox(0xffffefef, 0xffff0000, ImVec2(searchTermFrameLen, 35),
                               ImVec2(0, 0.5), ImVec2(10, 0),
                               "%llu", params.vuint);
            }
        } break;

        case SearchDataType::Float: {
            const f32 typeFrameLen = ImGui::CalcTextSize("Float").x + 20.0f;
            const f32 searchTermFrameLen = ImGui::GetContentRegionAvail().x - typeFrameLen;

            ImGui::TextBox(0xffdfdfdf, 0xff000000, ImVec2(typeFrameLen, 35), ImVec2(0.5, 0.5), ImVec2(0, 0),
                           "Float");
            ImGui::SameLine();
            ImGui::TextBox(0xffffefef, 0xffff0000, ImVec2(searchTermFrameLen, 35), ImVec2(0, 0.5),
                           ImVec2(10, 0),
                           "%g", params.dataSize == 4 ? params.vf32 : params.vf64);
        } break;
    }

    ImGui::TextBox(0xffffffff, 0xff000000, ImVec2(0, 30), ImVec2(0, 0.5), ImVec2(10, 0),
                   "%d found", results.count());

    const i32 count = results.count();
    if(count <= 0) {
        return false;
    }

    bool clicked = false;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::BeginChild("search_results", {0,0}, false, ImGuiWindowFlags_AlwaysUseWindowPadding);

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    const ImVec2 padding = {12, 3};
    const ImVec2 textSize = ImGui::CalcTextSize("AAAAAAAAAAA");
    ImGuiListClipper clipper(count, textSize.y + padding.y * 2);

    for(i32 i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
        const u64 itemDataOffset = results[i].offset;
        const ImVec2 pos = window->DC.CursorPos;
        const ImVec2 size = {ImGui::GetContentRegionAvail().x, textSize.y + padding.y * 2};
        const ImRect frameBb = {pos, pos + size};
        bool hovered, held;
        ImGui::ButtonBehavior(frameBb, window->GetID(((u8*)&results) + i), &hovered, &held);

        u32 textColor = 0xff000000;
        u32 frameColor = (i&1) ? 0xfff0f0f0 : 0xffe0e0e0;

        if(held) {
            frameColor = 0xffff7200;
            textColor = 0xffffffff;
            *gotoOffset = itemDataOffset;
        }
        else if(hovered) {
            frameColor = 0xffffb056;
            textColor = 0xffff0000;
        }
        clicked |= held;

        ImGui::TextBox(frameColor, textColor, size,
                       ImVec2(0, 0.5), ImVec2(padding.x, 0),
                       "%llx", itemDataOffset);
    }

    // TODO: add pages
    clipper.End();

    ImGui::ItemSize(ImVec2(10, 30));

    ImGui::EndChild();
	ImGui::PopStyleVar(2); // ItemSpacing, WindowPadding
    return clicked;
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
