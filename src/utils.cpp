#include "utils.h"

bool openFileReadAll(const char* path, FileBuffer* out_fb)
{
    FILE* file = fopen(path, "rb");
    if(!file) {
        LOG("ERROR: Can not open file %s", path);
        return false;
    }

    if(out_fb->data) {
        free(out_fb->data);
        out_fb->data = nullptr;
        out_fb->size = 0;
    }

    FileBuffer fb;

    i64 start = ftell(file);
    fseek(file, 0, SEEK_END);
    i64 len = ftell(file) - start;
    fseek(file, start, SEEK_SET);

    fb.data = (u8*)malloc(len + 1);
    assert(fb.data);
    fb.size = len;

    // read
    fread(fb.data, 1, len, file);
    fb.data[len] = 0;

    fclose(file);
    *out_fb = fb;

    LOG("file loaded path=%s size=%llu", path, fb.size);
    return true;
}

static char g_basePath[256];

void pathSetBasePath(const char *pBasePath)
{
	strcpy_s(g_basePath, sizeof(g_basePath), pBasePath);
}

const char* pathGetFilename(const char *pPath)
{
	const i32 len = strlen(pPath);
	const char* pLast = pPath;

	for(i32 i = 0; i < len; i++) {
		if(pPath[i] == '\\' || pPath[i] == '/') {
			pLast = pPath + i + 1;
		}
	}

	return pLast;
}

void pathAppend(char *pPath, i32 pathBuffSize, const char *toAppend)
{
	const i32 len = strlen(pPath);
#ifdef _WIN32
	if(pPath[len-1] != '\\') {
		strcat_s(pPath, pathBuffSize, "\\");
	}
	strcat_s(pPath, pathBuffSize, toAppend);
#else
	if(pPath[len-1] != '/') {
		strcat_s(pPath, pathBuffSize, "/");
	}
	strcat_s(pPath, pathBuffSize, toAppend);
#endif
}

void pathRelative(char *pDest, i32 destSize, const char *pPath)
{
	strcpy_s(pDest, destSize, g_basePath);
	pathAppend(pDest, destSize, pPath);
}
