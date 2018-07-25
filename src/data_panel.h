#pragma once
#include "base.h"
#include "utils.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

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

struct Gradient
{
    byte8 gmin; // reinterpret this to whatever we like
    byte8 gmax;
    Color3 color1;
    Color3 color2;

    template<typename T>
    inline void setBounds(T bmin, T bmax) {
        *(T*)&gmin = bmin;
        *(T*)&gmax = bmax;
    }

    inline void setColors(u32 colorU32_1, u32 colorU32_2) {
        color1 = {(colorU32_1 & 0xff) / 255.f,
                  ((colorU32_1 & 0xff00) >> 8) / 255.f,
                  ((colorU32_1 & 0xff0000) >> 16) / 255.f};
        color2 = {(colorU32_2 & 0xff) / 255.f,
                  ((colorU32_2 & 0xff00) >> 8) / 255.f,
                  ((colorU32_2 & 0xff0000) >> 16) / 255.f};
    }

    inline Color3 lerpColor(f32 alpha) const {
        alpha = clamp(alpha, 0.0f, 1.0f);
        return rgbLerp(color1, color2, alpha);
    }

    template<typename T>
    inline f32 getLerpVal(T v) const {
        const T tmin = *(T*)&gmin;
        const T tmax = *(T*)&gmax;
        return (f64(v) - tmin) / (tmax - tmin);
    }
};

Gradient getDefaultTypeGradient(PanelType::Enum ptype);

struct ColorDisplay
{
    enum Enum: i32 {
        GRADIENT = 0, // default
        PLAIN,
        BRICK_COLOR
    };
};

struct PanelParams
{
    ColorDisplay::Enum colorDisplay = ColorDisplay::GRADIENT;
    Gradient grads[PanelType::Enum::_COUNT];
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

    void processMouseInput(ImRect winRect);
    inline void selectionProcessMouseInput(const i32 panelId, ImVec2 mousePos, ImRect rect,
                               const i32 columnWidth_, const i32 rowHeight_,
                               const i32 startLine, const i32 hoverLen);
    inline bool selectionInHoverRange(i64 dataOffset);
    inline bool selectionInSelectionRange(i64 dataOffset);
    bool selectionIsEmpty();
    void deselect();

    DataPanels();
    void setFileBuffer(u8* buff, i64 buffSize);
    void addNewPanel();
    void removePanel(const i32 pid);
    void goTo(i32 offset);
    i32 getSelectedInt();

    void calculatePanelWidth();
    void doRowHeader();

    void doUi();

    void doHexPanel(i32 pid, f32 panelWidth, const i32 startLine);
    void doAsciiPanel(i32 pid, f32 panelWidth, const i32 startLine);

    template<typename T>
    void doFormatPanel(i32 pid, f32 panelWidth, const i32 startLine, const char* format);
};
