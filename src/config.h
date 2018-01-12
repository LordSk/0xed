#pragma once
#include "base.h"

#ifdef CONF_DEBUG
    #define WINDOW_WIDTH 1600
    #define WINDOW_HEIGHT 900
#else
    #define WINDOW_WIDTH 1000
    #define WINDOW_HEIGHT 750
#endif

struct Config
{
    i32 windowMonitor = 0;
    i32 windowWidth = WINDOW_WIDTH;
    i32 windowHeight = WINDOW_HEIGHT;
    i32 windowMaximized = 0;
    i32 panelCount = 2;
};

bool loadConfigFile(const char* path, Config* config);
bool saveConfigFile(const char* path, const Config& config);
