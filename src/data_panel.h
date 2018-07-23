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
    f32 hsv1[3];
    f32 hsv2[3];

    template<typename T>
    inline void setBounds(T bmin, T bmax) {
        *(T*)&gmin = bmin;
        *(T*)&gmax = bmax;
    }

    inline void setColors(u32 colMin, u32 colMax) {
        const f32 normRgb1[3] = {(colMin & 0xff) / 255.f,
                                 ((colMin & 0xff00) >> 8) / 255.f,
                                 ((colMin & 0xff0000) >> 16) / 255.f};
        const f32 normRgb2[3] = {(colMax & 0xff) / 255.f,
                                 ((colMax & 0xff00) >> 8) / 255.f,
                                 ((colMax & 0xff0000) >> 16) / 255.f};
        rgbToHsv(normRgb1, hsv1);
        rgbToHsv(normRgb2, hsv2);
    }

    inline u32 lerpColor(f32 alpha, f32* brightness, f32* hsvOut) const {
        alpha = clamp(alpha, 0.0f, 1.0f);
        f32 hsvMix[3];
        f32 rgbOut[3];

        hsvLerp(hsv1, hsv2, alpha, hsvMix);
        hsvToRgb(hsvMix, rgbOut);

        memmove(hsvOut, hsvMix, sizeof(hsvMix));
        *brightness = 0.299f * rgbOut[0] + 0.587f * rgbOut[1] + 0.114f * rgbOut[2];

        return 0xff000000 | (i32(rgbOut[2] * 0xff) << 16) |
                (i32(rgbOut[1] * 0xff) << 8) | i32(rgbOut[0] * 0xff);
    }

    template<typename T>
    inline f32 getLerpVal(T v) const {
        const T tmin = *(T*)&gmin;
        const T tmax = *(T*)&gmax;
        return (f64(v) - tmin) / (tmax - tmin);
    }
};

Gradient getDefaultTypeGradient(PanelType::Enum ptype);

struct DataPanels
{
    enum class ColorDisplay: i32 {
        PLAIN = 0, // default
        GRADIENT,
        BRICK_COLOR
    };

    f32 childPanelWidth;
    f32 panelRectWidth[PANEL_MAX_COUNT];
    PanelType::Enum panelType[PANEL_MAX_COUNT];
    ColorDisplay panelColorDisplay[PANEL_MAX_COUNT] = {};
    Gradient panelGradient[PANEL_MAX_COUNT][PanelType::Enum::_COUNT];
    i32 panelCount = 2;

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
    void goTo(i32 offset);
    i32 getSelectedInt();

    void calculatePanelWidth();
    void doRowHeader();

    void doUi();

    void doHexPanel(i32 pid, f32 panelWidth, const i32 startLine, ColorDisplay colorDisplay);
    void doAsciiPanel(i32 pid, f32 panelWidth, const i32 startLine, ColorDisplay colorDisplay);

    template<typename T>
    void doFormatPanel(i32 pid, f32 panelWidth, const i32 startLine, const char* format,
                       ColorDisplay colorDisplay);
};
