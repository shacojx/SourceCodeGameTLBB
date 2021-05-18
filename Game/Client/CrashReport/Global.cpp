#include "StdAfx.h"
#include "Global.h"

HINSTANCE g_hInstance;
HWND	g_hMainWnd = NULL;

CHAR g_szCurrentDir[MAX_PATH] = {0};
CHAR g_szSmallInfoFile[MAX_PATH] = {0};
CHAR g_szBigInfoFile[MAX_PATH] = {0};
CHAR g_szDumpFile[MAX_PATH] = {0};


CHAR g_szWinDbg[MAX_PATH] = {0};