#pragma once
#include "data_panel.h"
#include "search.h"

void toolsDoInspectorWindow(const u8* fileBuffer, const i64 fileBufferSize, const SelectionState& selection);
void toolsDoTemplate(struct BrickWall* brickWall);
void toolsDoOptions(i32* columnCount);
void toolsDoScript(struct Script* script, struct BrickWall* brickWall);
bool toolsSearchParams(SearchParams* params);
bool toolsSearchResults(const SearchParams& params, const Array<SearchResult>& results, u64* gotoOffset);
