#pragma once
#include "data_panel.h"

void toolsDoInspector(const u8* fileBuffer, const i64 fileBufferSize, const SelectionState& selection);
void toolsDoTemplate(struct BrickWall* brickWall);
void toolsDoOptions(i32* columnCount);
void toolsDoScript(struct Script* script, struct BrickWall* brickWall);
