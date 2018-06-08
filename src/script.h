#pragma once

#include "utils.h"

struct Script
{
    FileBuffer file;

    bool openAndParse(const char* path);
};
