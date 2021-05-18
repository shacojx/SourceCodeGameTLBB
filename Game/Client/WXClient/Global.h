#pragma once

#include "TDKernel.h"

class tGfxSystem;

//数据核心
extern tKernel			g_theKernel;

//主窗口
extern HWND				g_hMainWnd;
//当前主程序模块句柄
extern HINSTANCE		g_hInstance;
//当前版本号
extern const CHAR		VERSION_INFO[];
//标题
extern const CHAR		GAME_TITLE[];

#define MAX_ERROR_INFO 1024