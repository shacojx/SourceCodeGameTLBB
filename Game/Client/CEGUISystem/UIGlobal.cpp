#include "StdAfx.h"
#include "UIGlobal.h"

HINSTANCE				g_hInstance;
HWND					g_hMainWnd;
tKernel*				g_pKernel = NULL;
tScriptSystem*			g_pScriptSys = NULL;
tEventSystem*			g_pEventSys = NULL;
tActionSystem*			g_pActionSys = NULL;
tInputSystem*			g_pInputSys = NULL;
CRenderSystem*			g_pGfxSystem = NULL;
tObjectSystem*			g_pObjectSystem = NULL;
tWorldSystem*			g_pWorldSystem = NULL;
tCursorSystem*			g_pCursorSystem = NULL;
tDebuger*				g_pDebuger = NULL;
tTimeSystem *			g_pTimer = NULL;
tFakeObjSystem*			g_pFakeSystem = NULL;
tVariableSystem*		g_pVariableSystem = NULL;
tGameInterfaceBase*		g_pGameInterface = NULL;
tDataBaseSystem*		g_pDataBaseSystem = NULL;

