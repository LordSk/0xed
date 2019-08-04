#pragma once
#include "hexview.h"
#include "search.h"

void toolsDoInspectorWindow(const u8* fileBuffer, const i64 fileBufferSize, const SelectionState& selection);
void toolsDoTemplate(struct BrickWall* brickWall);
void toolsDoOptions(i32* pColumnCount, i32 *pOutOffset);
void toolsDoScript(struct Script* script, struct BrickWall* brickWall);
bool toolsSearchParams(SearchParams* params);
bool toolsSearchResults(const SearchParams& params, const ArrayTS<SearchResult>& results, u64* gotoOffset);
