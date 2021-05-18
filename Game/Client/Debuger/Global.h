#pragma once

class tKernel;
class tGameInterfaceBase;
class tGfxSystem;
class tTimeSystem;
class tEventSystem;
class tVariableSystem;
class tDataBaseSystem;

class CDlgDebuger;




extern tKernel*	g_pKernel;
extern tGameInterfaceBase*	g_pInterface;
extern tGfxSystem*	g_pGfxSystem;
extern tTimeSystem*	g_pTimeSystem;
extern tEventSystem* g_pEventSystem;
extern tVariableSystem* g_pVariableSystem;
extern tDataBaseSystem* g_pDBCSystem;

extern CDlgDebuger* g_pDlgDebug;