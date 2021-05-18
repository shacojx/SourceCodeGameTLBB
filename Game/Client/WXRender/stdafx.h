// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
//
// _WIN32_WINNT version constants
//
#define _WIN32_WINNT_NT4     0x0400
#define _WIN32_WINNT_WIN2K     0x0500
#define _WIN32_WINNT_WINXP     0x0501
#define _WIN32_WINNT_WS03     0x0502
#define _WIN32_WINNT_WIN6     0x0600
#define _WIN32_WINNT_VISTA     0x0600
#define _WIN32_WINNT_WS08     0x0600
#define _WIN32_WINNT_LONGHORN    0x0600
#define _WIN32_WINNT_WIN7     0x0601

#define _WIN32_WINNT _WIN32_WINNT_WINXP

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#define _WIN32_WINDOWS	(0x0410)	//Windows 98
//#define _WIN32_WINNT	(0x0500)	//Windows 2000
#include <windows.h>

// TODO: reference additional headers your program requires here
#include <stdio.h>

#include "TDException.h"