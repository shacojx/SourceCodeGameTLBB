// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")

// TODO: reference additional headers your program requires here
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Standard C++ Library
#include <string>
#include <vector>
#include <list>
#include <map>
#include <hash_map>
#include <algorithm>

extern HINSTANCE g_hInstance;
