#include "search.h"
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>

struct SearchQueue
{
    bool8 running = true;
    u32 searchHashCurrent = 0;
    u32 searchHashRequest = 0;
    Array<SearchResult>* resultListCurrent = nullptr;
    Array<SearchResult>* resultListRequest = nullptr;
    SearchParams paramsCurrent;
    SearchParams paramsRequest;
    FileBuffer fileBuff = {};
};

static SDL_Thread* g_searchThread;
static SearchQueue* g_searchQueue;

static i32 thread_search(void* ptr)
{
    LOG("Search> thread started.");
    SearchQueue& sq = *g_searchQueue;

    while(sq.running) {
        if(sq.searchHashRequest == 0) {
            SDL_Delay(1);
        }
        else {
            LOG("Search> new request, hash=%x", sq.searchHashRequest);
            sq.searchHashCurrent = sq.searchHashRequest;
            sq.resultListCurrent = sq.resultListRequest;
            sq.paramsCurrent = sq.paramsRequest;

            const i64 fileSize = sq.fileBuff.size;
            const u8* fileData = sq.fileBuff.data;
            const i32 cmpDataSize = sq.paramsCurrent.dataSize;
            const i32 strideEq[] = { 1, cmpDataSize/2, cmpDataSize };
            const i64 stride = strideEq[sq.paramsCurrent.strideKind];
            u8 cmpData[64];
            assert(cmpDataSize > 0);
            assert(cmpDataSize < 64);

            switch(sq.paramsCurrent.dataType) {
                case SearchDataType::ASCII_String: {
                    memmove(cmpData, sq.paramsCurrent.str, cmpDataSize);
                } break;

                case SearchDataType::Integer: {
                    if(sq.paramsCurrent.intSigned) {
                        memmove(cmpData, &sq.paramsCurrent.vint, cmpDataSize);
                    }
                    else{
                        memmove(cmpData, &sq.paramsCurrent.vuint, cmpDataSize);
                    }
                } break;

                case SearchDataType::Float: {
                    if(cmpDataSize == 4) {
                        memmove(cmpData, &sq.paramsCurrent.vf32, 4);
                    }
                    else {
                        memmove(cmpData, &sq.paramsCurrent.vf64, 8);
                    }
                } break;

                default: assert(0); break;
            }

            i64 foundCount = 0;
            for(i64 i = 0; i < fileSize; i += stride) {
                if(sq.searchHashRequest != sq.searchHashCurrent) {
                    break; // cancel we have a new request
                }
                // check if found
                if(memcmp(fileData + i, cmpData, cmpDataSize) == 0) {
                    SearchResult r;
                    r.offset = i;
                    r.len = cmpDataSize;
                    sq.resultListCurrent->push(r);
                    foundCount++;
                    i += cmpDataSize-stride;
                }
            }

            LOG("Search> [%x] done, %lld found.", sq.searchHashCurrent, foundCount);
            if(sq.searchHashRequest == sq.searchHashCurrent) {
                sq.searchHashRequest = 0;
            }
            sq.searchHashCurrent = 0;
        }
    }
    return 0;
}

bool searchStartThread()
{
    static SearchQueue sq;
    g_searchQueue = &sq;
    g_searchThread = SDL_CreateThread(thread_search, "Search", nullptr);
    return true;
}

void searchTerminateThread()
{
	g_searchQueue->running = false;
	i32 status;
	SDL_WaitThread(g_searchThread, &status);
}

void searchSetNewFileBuffer(FileBuffer nfb)
{
    SearchQueue& sq = *g_searchQueue;
    sq.searchHashCurrent = 0;
    sq.searchHashRequest = 0;
    sq.fileBuff = nfb;
}

void searchNewRequest(const SearchParams& params, Array<SearchResult>* results)
{
    SearchQueue& sq = *g_searchQueue;
    sq.paramsRequest = params;
    sq.resultListRequest = results;
    sq.searchHashRequest = hash32(&params, sizeof(params));
}
