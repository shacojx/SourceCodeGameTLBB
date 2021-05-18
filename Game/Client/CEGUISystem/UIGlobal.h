#pragma once
class tKernel;
class tScriptSystem;
class tEventSystem;
class tActionSystem;
class tInputSystem;
class CRenderSystem;
class tObjectSystem;
class tWorldSystem;
class tCursorSystem;
class tDebuger;
class tTimeSystem;
class tFakeObjSystem;
class tVariableSystem;
class tGameInterfaceBase;
class tDataBaseSystem;

extern HINSTANCE			g_hInstance;
extern HWND					g_hMainWnd;
extern tKernel*				g_pKernel;
extern tScriptSystem*		g_pScriptSys;
extern tEventSystem*		g_pEventSys;
extern tActionSystem*		g_pActionSys;
extern tInputSystem*		g_pInputSys;
extern CRenderSystem*		g_pGfxSystem;
extern tObjectSystem*		g_pObjectSystem;
extern tWorldSystem*		g_pWorldSystem;
extern tCursorSystem*		g_pCursorSystem;
extern tDebuger*			g_pDebuger;
extern tTimeSystem*			g_pTimer;
extern tFakeObjSystem*		g_pFakeSystem;
extern tVariableSystem*		g_pVariableSystem;
extern tGameInterfaceBase*	g_pGameInterface;
extern tDataBaseSystem*		g_pDataBaseSystem;
//#define LOAD_UI_DYNAMIC

#ifndef SAFE_DELETE
#define SAFE_DELETE( p ) if( p ){ delete ( p ); ( p ) = NULL; }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE( p ) if( p ){ (p)->Release();delete ( p ); ( p ) = NULL; }
#endif