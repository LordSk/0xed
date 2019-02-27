#pragma once
#include "base.h"
#include "utils.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h" // ImVec2
#include "imgui_internal.h" // ImRect, ImGuiWindow

#define PANEL_MAX_COUNT 10

struct SelectionState
{
    i64 hoverStart = -1;
    i64 hoverEnd = -1;
    i64 selectStart = -1;
    i64 selectEnd = -1;
    i32 lockedPanelId = -1;
};

struct PanelType
{
    enum Enum {
        HEX = 0,
        ASCII,

        INT8,
        UINT8,
        INT16,
        UINT16,
        INT32,
        UINT32,
        INT64,
        UINT64,

        FLOAT32,
        FLOAT64,
        _COUNT,
    };
};

typedef i64 byte8;

struct GradientRange
{
    byte8 gmin; // reinterpret this to whatever we like
    byte8 gmax;

    template<typename T>
    inline void setBounds(T bmin, T bmax) {
        *(T*)&gmin = bmin;
        *(T*)&gmax = bmax;
    }

    template<typename T>
    inline f32 getLerpVal(T v) const {
        const T tmin = *(T*)&gmin;
        const T tmax = *(T*)&gmax;
        return (f64(v) - tmin) / (tmax - tmin);
    }
};

GradientRange getDefaultTypeGradientRange(PanelType::Enum ptype);

struct ColorDisplay
{
    enum Enum: i32 {
        GRADIENT = 0, // default
        PLAIN,
        BRICK_COLOR,
        SEARCH
    };
};

struct PanelParams
{
    ColorDisplay::Enum colorDisplay = ColorDisplay::GRADIENT;
    GradientRange gradRange[PanelType::Enum::_COUNT];
    Color3 gradColor1;
    Color3 gradColor2;

    inline void gradSetColors(u32 colorU32_1, u32 colorU32_2) {
        gradColor1 = {(colorU32_1 & 0xff) / 255.f,
                      ((colorU32_1 & 0xff00) >> 8) / 255.f,
                      ((colorU32_1 & 0xff0000) >> 16) / 255.f};
        gradColor2 = {(colorU32_2 & 0xff) / 255.f,
                      ((colorU32_2 & 0xff00) >> 8) / 255.f,
                      ((colorU32_2 & 0xff0000) >> 16) / 255.f};
    }

    inline Color3 gradLerpColor(f32 alpha) const {
        alpha = clamp(alpha, 0.0f, 1.0f);
        return rgbLerp(gradColor1, gradColor2, alpha);
    }

    inline void gradSetDefaultColors()
    {
        gradSetColors(0xff4c4c4c, 0xffffffff);
    }
};

struct DataPanels
{
    f32 childPanelWidth;
    f32 panelRectWidth[PANEL_MAX_COUNT];

    PanelType::Enum panelType[PANEL_MAX_COUNT] = {};
    PanelParams panelParams[PANEL_MAX_COUNT] = {};
    i32 panelCount = 3;

    u8* fileBuffer;
    i64 fileBufferSize;
    i64 scrollCurrentLine = 0;
    i64 goToLine = -1;
    struct BrickWall* brickWall = nullptr;

    i32 columnCount = 16;
    const i32 columnWidth = 22;
    const f32 rowHeight = 22;
    const i32 columnHeaderHeight = 24;
    i32 rowHeaderWidth = 32;
    const i32 panelSpacing = 10;
    const f32 panelCloseButtonWidth = 20;
    const f32 panelColorButtonWidth = 30;

    const i32 asciiCharWidth = 14;
    const i32 intColumnWidth = 34;

    const ImU32 hoverFrameColor = 0xffff9c4c;
    const ImU32 selectedFrameColor = 0xffff7200;
    const ImU32 selectedTextColor = 0xffffffff;

    struct ImFont* fontMono;

    SelectionState selectionState;

    ImGuiWindow* mainWindow;

    inline bool selectionInHoverRange(i64 dataOffset);
    inline bool selectionInSelectionRange(i64 dataOffset);
    bool selectionIsEmpty();
    void deselect();
    void select(i64 start, i64 end);

    DataPanels();
    void setFileBuffer(u8* buff, i64 buffSize);
    void addNewPanel();
    void removePanel(const i32 pid);
    void goTo(i32 offset);
    i32 getSelectedInt();

    void calculatePanelWidth();

    void doUi();

    void doHexPanel(i32 pid, f32 panelWidth, const i32 startLine);
    void doAsciiPanel(i32 pid, f32 panelWidth, const i32 startLine);

    template<typename T>
    void doFormatPanel(i32 pid, f32 panelWidth, const i32 startLine, const char* format);

    void doPanelParamPopup(bool open, i32* panelId, ImVec2 popupPos);
};

struct UiStyle
{
	const i32 columnWidth = 22;
	const f32 rowHeight = 22;
	const i32 columnHeaderHeight = 24;
	const i32 panelSpacing = 10;
	const f32 panelCloseButtonWidth = 20;
	const f32 panelColorButtonWidth = 30;

	const i32 asciiCharWidth = 14;
	const i32 intColumnWidth = 34;

	const u32 textColor = 0xff000000;
	const u32 hoverFrameColor = 0xffff9c4c;
	const u32 selectedFrameColor = 0xffff7200;
	const u32 selectedTextColor = 0xffffffff;

	const u32 headerBgColorOdd = 0xffd8d8d8;
	const u32 headerBgColorEven = 0xffe5e5e5;
	const u32 headerTextColorOdd = 0xff808080;
	const u32 headerTextColorEven = 0xff737373;
};

extern UiStyle* g_uiStyle;
void SetUiStyleLight();
inline const UiStyle& GetUiStyle() { assert(g_uiStyle); return *g_uiStyle; }

void UiHexRowHeader(i64 firstRow, i32 rowStep, f32 textOffsetY, const SelectionState& selection);
void UiHexColumnHeader(i32 columnCount, const SelectionState& selection);
bool UiHexPanelDoSelection(i32 panelID, i32 panelType, SelectionState* outSelectionState, i32 firstLine, i32 columnCount);
void UiHexPanelTypeDoSelection(SelectionState* outSelectionState, i32 panelId, ImVec2 mousePos, ImRect rect, i32 columnWidth_, i32 rowHeight_, i32 startLine, i32 columnCount, i32 hoverLen);
