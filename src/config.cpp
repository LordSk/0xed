#include "config.h"
#include <stdio.h>
#include <string.h>

// Basic config file saving/loading
// WILL break if anything is out of order (default config is loaded then)

inline void skipGarbage(char** str)
{
    while(**str && **str == '\n') {
        (*str)++;
    }
}

inline void nextLine(char** str)
{
    while(**str && **str != '\n') {
        (*str)++;
    }
    (*str)++; // skip new line symbol
}

inline bool match(char** src, char* toMatch)
{
    const i32 len = strlen(toMatch);
    char* cursor = *src;
    for(i32 i = 0; i < len; ++i) {
        if(cursor[i] != toMatch[i]) {
            assert(0);
            return false;
        }
    }

    *src += len;
    return true;
}

inline bool matchLine(char** src, char* toMatch)
{
    bool r = match(src, toMatch);
    if(r) nextLine(src);
    return r;
}

#define parseLine(cursor, expectedCount, fmt, ...) if(sscanf(cursor, fmt, __VA_ARGS__) == expectedCount) {\
    nextLine(&cursor); } else { assert(0); return false; }

bool loadConfigFile(const char* path, Config* config)
{
    FileBuffer fb;
    if(!openFileReadAll(path, &fb)) {
        return false;
    }

    char* cursor = (char*)fb.data;
    matchLine(&cursor, "[Window]");
    parseLine(cursor, 1, "monitor=%d", &config->windowMonitor);
    parseLine(cursor, 1, "width=%d", &config->windowWidth);
    parseLine(cursor, 1, "height=%d", &config->windowHeight);
    parseLine(cursor, 1, "maximized=%d", &config->windowMaximized);
    parseLine(cursor, 1, "panelCount=%d", &config->panelCount);

    config->windowMonitor = clamp(config->windowMonitor, 0, 1);
    config->windowMaximized = clamp(config->windowMaximized, 0, 1);
    config->panelCount = clamp(config->panelCount, 1, 10);
    config->windowWidth = max(100, config->windowWidth);
    config->windowHeight = max(100, config->windowHeight);

    return true;
}

#define appendf(cursor, fmt, ...) cursor += sprintf(cursor, fmt, __VA_ARGS__)

bool saveConfigFile(const char* path, const Config& config)
{
    // TODO: replace this eventually
    char output[10000];
    char* cursor = output;

    appendf(cursor, "[Window]\n");
    appendf(cursor, "monitor=%d\n", config.windowMonitor);
    appendf(cursor, "width=%d\n", config.windowWidth);
    appendf(cursor, "height=%d\n", config.windowHeight);
    appendf(cursor, "maximized=%d\n", config.windowMaximized);
    appendf(cursor, "panelCount=%d\n", config.panelCount);

    appendf(cursor, "\n", config.windowHeight);

    FILE* f = fopen(path, "wb");
    if(!f) {
        LOG("ERROR: could not write file %s", path);
        return false;
    }

    fwrite(output, 1, cursor - output, f);
    fclose(f);

    return true;
}
