//---------------------------------------------------------------------------
// Desc:	Percompiled header files of Win32 Platform
//---------------------------------------------------------------------------
#ifndef FoxWin32_H
#define FoxWin32_H 

	

#ifndef __linux

		#ifdef SCRIPTENGINE_EXPORTS
		#define ENGINE_API __declspec(dllexport)
		#else
		#define ENGINE_API __declspec(dllimport)
		#endif

		#ifndef _ASSERT
		#define _ASSERT(x)
		#endif

        #ifndef _WIN32_WINNT
        #define _WIN32_WINNT  0x0400
        #endif

		#include <windows.h>
		#define DIRECTINPUT_VERSION 0x800
	//---------------------------------------------------------------------------
	//---------------------------------------------------------------------------
		#if _MSC_VER > 1000
			#pragma once
		#endif // _MSC_VER > 1000

		#include <windows.h>
		//#include <mmsystem.h>
		//#include <dsound.h>
		//#include <dinput.h>
		//#include <ddraw.h>

	//	#define SUCCEEDED(x)		((x) > 0)
#else
	#define ENGINE_API
    #define LONG long
	#define INT int
	#define LPINT int *
    #define HWND unsigned long
    #define HANDLE unsigned long
    #define LPSTR char *
    #define LPTSTR char *
    #define LPCSTR const char *
	#define LPCTSTR const char *
    #define DWORD unsigned long
    #define LPVOID void *
    #define PVOID void *
    #define BOOL int
    #define TRUE 1
    #define FALSE 0
    #define BYTE unsigned char
    #define WORD unsigned short
	#define UINT unsigned int
	#define PBYTE unsigned char *
	#define LPBYTE unsigned char *
    #define LONG long
	#define IN
	#define OUT

typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECT, *LPRECT;

typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT;

typedef struct _GUID {          // size is 16
    DWORD Data1;
    WORD   Data2;
    WORD   Data3;
    BYTE  Data4[8];
} GUID;

#define MAX_PATH 300
#define CALLBACK
#include <stdarg.h> 
#include <stdio.h>
#include <string.h>
#define ZeroMemory(x,y) memset(x, 0, y)
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define FILE_CURRENT	1
#define FILE_END		2
#define FILE_BEGIN		0

#include <pthread.h>
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

	
#define SUCCEEDED(x)		((long)x >= 0)



#include <unistd.h>
#define __stdcall
#define LPDWORD unsigned long *
#define SOCKET_ERROR -1

#endif

//---------------------------------------------------------------------------
// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>


	#ifndef ITOA
		#define ITOA(NUMBER)  #NUMBER
	#endif

	#ifndef __TEXT_LINE__
		#define __TEXT_LINE__(LINE) ITOA(LINE)
	#endif

	#ifndef KSG_ATTENTION
		#define KSG_ATTENTION(MSG) __FILE__"("__TEXT_LINE__(__LINE__)") : ATTENTION "#MSG
	#endif


//---------------------------------------------------------------------------
#endif
