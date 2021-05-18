#include "StdAfx.h"
#include "..\Resource.h"
#include "Gameprocedure.h"
#include "GamePro_Login.h"
#include "GamePro_CharSel.h"
#include "GamePro_CharCreate.h"
#include "GamePro_Enter.h"
#include "GamePro_Main.h"
#include "Gamepro_ChangeScene.h"

#include "..\Interface\GMGameInterface.h"

#include "..\Global.h"

#include "..\World\WorldManager.h"

#include "..\Network\NetManager.h"
#include "CGEnterScene.h"

#include "..\Object\ObjectManager.h"
#include "..\Object\Object.h"
#include "..\Object\Surface\Obj_Surface.h"
#include "..\Object\Surface\Obj_Effect.h"
#include "..\Object\Surface\Obj_Bullet.h"
#include "..\Object\Surface\Obj_SkillObj.h"
#include "..\Object\Surface\Obj_Special.h"
#include "..\Object\Obj_Map.h"
#include "..\Object\Obj_Static.h"
#include "..\Object\Obj_Building.h"
#include "..\Object\Obj_Dynamic.h"
#include "..\Object\Character\Obj_Character.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\Object\Character\Obj_PlayerNPC.h"
#include "..\Object\Character\Obj_PlayerOther.h"
#include "..\Object\ProjTex\Obj_ProjTex.h"
#include "..\Object\TripperObj\GMTripperObj_Transport.h"
#include "..\Object\TripperObj\GMTripperObj_ItemBox.h"
#include "..\Object\TripperObj\GMTripperObj_Resource.h"
#include "..\Object\TripperObj\GMTripperObj_Platform.h"

#include "..\Input\GMInputSystem.h"
#include "..\Sound\GMSoundSystem.h"
#include "..\DBC\GMDataBase.h"
#include "..\Variable\GMVariable.h"

#include "BuffImpactMgr.h"
#include "DirectlyImpactMgr.h"
#include "..\Object\BulletDataMgr.h"
#include "SkillDataMgr.h"
#include "..\Object\ActionSetMgr.h"

#include "TDTimeSystem.h"
#include "TDException.h"
#include "TDUISystem.h"
#include "..\DataPool\GMUIDataPool.h"
#include "..\DataPool\GMDataPool.h"
#include "TDdebuger.h"
#include "..\Cursor\CursorMng.h"
#include "..\DataPool\GMUIDataPool.h"
#include "..\Script\GMScriptSystem.h"
#include "..\Event\GMEventSystem.h"
#include "..\Action\GMActionSystem.h"
#include "TDProfile.h"
#include "AxProfile.h"
#include "..\FakeObject\GMFakeObjSystem.h"
#include "..\Helper\GMHelperSystem.h"
#include "..\Interface\GMInterface_Script_SystemSetup.h"

#include "..\WXRender\ResourceProvider.h"
#include "..\Sound\GMSoundSystem.h"
#include "..\WxRender\RenderSystem.h"

const CHAR MAINWINDOW_CLASS[]	= "WuXiaShiJie WndClass";

const INT DEFWINDOW_WIDTH		= 800;
const INT DEFWINDOW_HEIGHT		= 600;

const INT MINWINDOW_WIDTH		= 800;
const INT MINWINDOW_HEIGHT		= 600;

const UINT DEFWINDOW_STYLE		= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX ; 


//静态变量初始化
CGamePro_Login*			CGameProcedure::s_pProcLogIn		= NULL;
CGamePro_CharSel*		CGameProcedure::s_pProcCharSel		= NULL;	
CGamePro_CharCreate*	CGameProcedure::s_pProcCharCreate	= NULL;
CGamePro_ChangeScene*   CGameProcedure::s_pProcChangeScene  = NULL;
CGamePro_Enter*			CGameProcedure::s_pProcEnter		= NULL;
CGamePro_Main*			CGameProcedure::s_pProcMain			= NULL;
CGameProcedure*         CGameProcedure::s_pActiveProcedure  = NULL;


// 当前激活的游戏流程
CGameProcedure*			CGameProcedure::s_pProcActive		= NULL;
// 前一次激活的游戏流程.
CGameProcedure*			CGameProcedure::s_pProcPrev			= NULL;

tInputSystem*			CGameProcedure::s_pInputSystem		= NULL;
CNetManager*			CGameProcedure::s_pNetManager		= NULL;
tWorldSystem*			CGameProcedure::s_pWorldManager		= NULL;
tObjectSystem*			CGameProcedure::s_pObjectManager	= NULL;
tDebuger*				CGameProcedure::s_pDebuger 			= NULL;
CRenderSystem*			CGameProcedure::s_pGfxSystem		= NULL;
tUISystem*				CGameProcedure::s_pUISystem			= NULL;
tTimeSystem*			CGameProcedure::s_pTimeSystem		= NULL;
tSoundSystem*			CGameProcedure::s_pSoundSystem		= NULL;
tDataBaseSystem*		CGameProcedure::s_pDataBaseSystem	= NULL;
tVariableSystem*		CGameProcedure::s_pVariableSystem	= NULL;
tGameInterfaceBase*		CGameProcedure::s_pGameInterface    = NULL;
tDataPool*				CGameProcedure::s_pDataPool			= NULL;
tDataPool_*				CGameProcedure::s_pDataPool_		= NULL;
tCursorSystem*			CGameProcedure::s_pCursorMng		= NULL;
tScriptSystem*			CGameProcedure::s_pScriptSystem		= NULL;
tEventSystem*			CGameProcedure::s_pEventSystem		= NULL;
tActionSystem*			CGameProcedure::s_pActionSystem		= NULL;
tFakeObjSystem*			CGameProcedure::s_pFakeObjSystem	= NULL;
tHelperSystem*			CGameProcedure::s_pHelperSystem		= NULL;
CResourceProvider*      CGameProcedure::s_pResourceProvider = NULL;

CBuffImpactMgr			*CGameProcedure::s_pBuffImpactMgr	= NULL;
CDirectlyImpactMgr		*CGameProcedure::s_pDirectlyImpactMgr	= NULL;
CBulletDataMgr			*CGameProcedure::s_pBulletDataMgr	= NULL;
CSkillDataMgr			*CGameProcedure::s_pSkillDataMgr	= NULL;
CMissionDataMgr			*CGameProcedure::s_pMissionDataMgr	= NULL;
CampAndStandDataMgr_T	*CGameProcedure::s_pCampDataMgr		= NULL;
CActionSetMgr			*CGameProcedure::s_pActionSetMgr	= NULL;
BOOL					CGameProcedure::m_bMinimized		= FALSE;
BOOL					CGameProcedure::m_bMaximized		= FALSE;
BOOL					CGameProcedure::m_bFullScreen		= FALSE;
BOOL					CGameProcedure::m_bActive			= true;
RECT					CGameProcedure::m_rectWindow;
RECT					CGameProcedure::m_rectFCOffset;
FLOAT					CGameProcedure::m_fWindowFOV		= (FLOAT)DEFWINDOW_HEIGHT/(FLOAT)DEFWINDOW_WIDTH;
BOOL					CGameProcedure::m_bRenderingPaused	= FALSE;
BOOL					CGameProcedure::m_bUIHandleInput	= FALSE;
MMRESULT				CGameProcedure::m_hEventTimer		= NULL;
HANDLE					CGameProcedure::m_hTickEvent		= NULL;
bool					CGameProcedure::m_bWaitNeedFreshMinimap = false;
bool					CGameProcedure::m_bNeedFreshMinimap		= false;


////  不可使用的字符串表
//UNUSE_STRING_VECTOR		CGameProcedure::m_UnUseStrVector;

VOID CGameProcedure::InitStaticMemeber(VOID)
{
	//-------------------------------------------------------------------
	//初始化随即变量种子
	srand( (unsigned)timeGetTime() );

	//-------------------------------------------------------------------
	// 注册本地类
	g_theKernel.ReisgerClass(GETCLASS(tNode));
	//网络管理器
	g_theKernel.ReisgerClass(GETCLASS(CNetManager));
	//场景管理器
	g_theKernel.ReisgerClass(GETCLASS(CWorldManager));
	//物体管理器
	g_theKernel.ReisgerClass(GETCLASS(CObjectManager));
	g_theKernel.ReisgerClass(GETCLASS(CObject));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Surface));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Effect));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Bullet));
	//g_theKernel.ReisgerClass(GETCLASS(CObject_SkillObj));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Special));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Map));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Static));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Building));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Dynamic));
	g_theKernel.ReisgerClass(GETCLASS(CObject_Character));
	g_theKernel.ReisgerClass(GETCLASS(CObject_PlayerMySelf));
	g_theKernel.ReisgerClass(GETCLASS(CObject_PlayerNPC));
	g_theKernel.ReisgerClass(GETCLASS(CObject_PlayerOther));
	g_theKernel.ReisgerClass(GETCLASS(CObject_ProjTex));
	g_theKernel.ReisgerClass(GETCLASS(CObject_ProjTex_MouseTarget));
	g_theKernel.ReisgerClass(GETCLASS(CObject_ProjTex_AuraDure));
	g_theKernel.ReisgerClass(GETCLASS(CTripperObject_Transport));
	g_theKernel.ReisgerClass(GETCLASS(CTripperObject_ItemBox));
	g_theKernel.ReisgerClass(GETCLASS(CTripperObject_Resource));
	g_theKernel.ReisgerClass(GETCLASS(CTripperObject_Platform));

	//输入管理器
	g_theKernel.ReisgerClass(GETCLASS(CInputSystem));
	//计时器
	g_theKernel.ReisgerClass(GETCLASS(tTimeSystem));
	//声音管理器
	g_theKernel.ReisgerClass(GETCLASS(CSoundSystemFMod));
	//数据库管理器
	g_theKernel.ReisgerClass(GETCLASS(CDataBaseSystem));
	//数据库管理器
	g_theKernel.ReisgerClass(GETCLASS(CVariableSystem));
	//UI操作接口
	g_theKernel.ReisgerClass(GETCLASS(CGameInterface));
	//UI数据池
	g_theKernel.ReisgerClass(GETCLASS(CUIDataPool));
	g_theKernel.ReisgerClass(GETCLASS(CDataPool));
	//UI鼠标操作类
	g_theKernel.ReisgerClass(GETCLASS(CCursorMng));
	//脚本系统
	g_theKernel.ReisgerClass(GETCLASS(CScriptSystem));
	//事件系统
	g_theKernel.ReisgerClass(GETCLASS(CEventSystem));
	//操作管理
	g_theKernel.ReisgerClass(GETCLASS(CActionSystem));
	//UI模型显示管理
	g_theKernel.ReisgerClass(GETCLASS(CFakeObjSystem));
	//外接帮助系统
	g_theKernel.ReisgerClass(GETCLASS(CHelperSystem));

	//-------------------------------------------------------------------
	// 加载插件中的类
//	PROFILE_PUSH(LoadPluginDll);
//	g_theKernel.LoadPlugin(_T("Debuger.dll"), &g_theKernel);
	g_theKernel.LoadPlugin(_T("WXRender.dll"), &g_theKernel);
	g_theKernel.LoadPlugin(_T("UISystem.dll"), &g_theKernel);
//	PROFILE_POP(LoadPluginDll);

	//-------------------------------------------------------------------
	//初始化所有的循环实例
	s_pProcLogIn			= new CGamePro_Login();			//!< 登录循环
	s_pProcCharSel			= new CGamePro_CharSel();		//!< 人物选择流程
	s_pProcCharCreate		= new CGamePro_CharCreate();	//!< 人物创建流程
	s_pProcEnter			= new CGamePro_Enter();			//!< 等待进入场景流程
	s_pProcMain				= new CGamePro_Main();			//!< 主游戏循环
	s_pProcChangeScene		= new CGamePro_ChangeScene();	//!< 服务器切换流程



	
	// 初始化数据核心
//0. 输入管理器
	s_pInputSystem	= (tInputSystem*)	g_theKernel.NewNode(_T("CInputSystem"), _T("bin"), _T("input"));  
//1. 计时器
	s_pTimeSystem	= (tTimeSystem*)	g_theKernel.NewNode(_T("tTimeSystem"), _T("bin"), _T("time"));  
//2. 网络管理器
	s_pNetManager	= (CNetManager*)	g_theKernel.NewNode(_T("CNetManager"), _T("bin"), _T("netman"));  
//3. 音效管理器
	s_pSoundSystem  = (tSoundSystem*)	g_theKernel.NewNode(_T("CSoundSystemFMod"), _T("bin"), _T("snd"));
//4. 渲染器节点
	s_pGfxSystem	= (CRenderSystem*)	g_theKernel.NewNode(_T("CRenderSystem"), _T("bin"), _T("gfx"));
//5. 调试器节点
//	s_pDebuger		= (tDebuger*)		g_theKernel.NewNode(_T("TDDebuger"), _T("bin"), _T("debuger"));
//6. 物体管理器
	s_pObjectManager= (tObjectSystem*)	g_theKernel.NewNode(_T("CObjectManager"), _T("bin"), _T("objman"));  
//7. UI管理器
	s_pUISystem		= (tUISystem*)		g_theKernel.NewNode(_T("CUISystem"), _T("bin"), _T("ui_"));
//8. 世界管理器
	s_pWorldManager = (tWorldSystem*)	g_theKernel.NewNode(_T("CWorldManager"), _T("bin"), _T("worldman"));  
//9. 数据库管理器
	s_pDataBaseSystem	= (tDataBaseSystem*)	g_theKernel.NewNode(_T("CDataBaseSystem"), _T("bin"), _T("dbc"));
//10. 系统变量管理器
	s_pVariableSystem	= (tVariableSystem*)	g_theKernel.NewNode(_T("CVariableSystem"), _T("bin"), _T("var"));
//11. 接口管理器
	s_pGameInterface	= ( tGameInterfaceBase* ) g_theKernel.NewNode( _T("CGameInterface"), _T("bin"), _T("interface"));
//12. UI数据池
	s_pDataPool			= ( tDataPool* ) g_theKernel.NewNode( _T( "CUIDataPool" ), _T( "bin" ), _T( "datapool" ) );
	s_pDataPool_		= ( tDataPool_* ) g_theKernel.NewNode( _T( "CDataPool" ), _T( "bin" ), _T( "datapool_" ) );
//13. 鼠标指针管理器
	s_pCursorMng		= ( tCursorSystem* ) g_theKernel.NewNode( _T( "CCursorMng"), _T( "bin" ), _T("cursor" ) );
//14. 脚本系统
	s_pScriptSystem		= ( tScriptSystem* ) g_theKernel.NewNode( _T( "CScriptSystem"), _T( "bin" ), _T("script" ) );
//15. 事件系统
	s_pEventSystem		= ( tEventSystem* ) g_theKernel.NewNode( _T( "CEventSystem"), _T( "bin" ), _T("event" ) );
//16. 操作管理
	s_pActionSystem		= ( tActionSystem* ) g_theKernel.NewNode( _T( "CActionSystem"), _T( "bin" ), _T("action" ) );
//17. UI模型显示管理
	s_pFakeObjSystem	= ( tFakeObjSystem* ) g_theKernel.NewNode( _T( "CFakeObjSystem"), _T( "bin" ), _T("fake" ) );
//18. 外接帮助系统
	s_pHelperSystem		= ( tHelperSystem* ) g_theKernel.NewNode( _T( "CHelperSystem"), _T( "bin" ), _T("helper" ) );

//19. 资源提供
	s_pResourceProvider	= ( CResourceProvider* ) g_theKernel.NewNode( _T( "CResourceProvider"), _T( "bin" ), _T("resprovider" ) );

	//-------------------------------------------------------------------
	//初始化工作节点
	s_pVariableSystem	->Initial(NULL);
	s_pEventSystem		->Initial(NULL);
	//-------------------------------------------------------------------
	// 创建主窗口
	CreateMainWnd();

	//-------------------------------------------------------------------
	s_pGfxSystem		->Initial(&g_hMainWnd);
	s_pScriptSystem		->Initial(NULL);
	s_pDataBaseSystem	->Initial(NULL);
	s_pTimeSystem		->Initial(NULL);
	s_pNetManager		->Initial(NULL); 
	s_pWorldManager		->Initial(NULL); 
	s_pObjectManager	->Initial(NULL); 
	if(s_pDebuger)		s_pDebuger->Initial(NULL); 
	if(s_pUISystem)		s_pUISystem->Initial(NULL);
	s_pInputSystem		->Initial(NULL); 
	s_pSoundSystem		->Initial(&g_hMainWnd);
	s_pDataPool			->Initial(NULL);
	s_pDataPool_		->Initial(NULL);
	s_pCursorMng		->Initial(&g_hInstance);
	s_pActionSystem		->Initial(NULL);
	s_pGameInterface	->Initial(NULL);
	s_pFakeObjSystem	->Initial(NULL);
	s_pHelperSystem		->Initial(NULL);
	s_pResourceProvider	->Initial(NULL);


	s_pSkillDataMgr	= new CSkillDataMgr;
	s_pSkillDataMgr->Init( s_pDataBaseSystem->GetDataBase( DBC_SKILL_DATA )->GetDBCFile() );

	s_pBuffImpactMgr	= new CBuffImpactMgr;
	s_pBuffImpactMgr->Init( s_pDataBaseSystem->GetDataBase( DBC_BUFF_IMPACT )->GetDBCFile() );

	s_pDirectlyImpactMgr	= new CDirectlyImpactMgr;
	s_pDirectlyImpactMgr->Init( s_pDataBaseSystem->GetDataBase( DBC_DIRECTLY_IMPACT )->GetDBCFile() );

	s_pBulletDataMgr	= new CBulletDataMgr;
	s_pBulletDataMgr->Init( s_pDataBaseSystem->GetDataBase( DBC_BULLET_DATA ) );
	//camp
	s_pCampDataMgr	= new CampAndStandDataMgr_T;
	s_pCampDataMgr->InitFromDBMemory(s_pDataBaseSystem->GetDataBase(DBC_CAMP_AND_STAND)->GetDBCFile());

	s_pActionSetMgr	= new CActionSetMgr;
	s_pActionSetMgr->Init( "ActionSet/" );

	//-------------------------------------------------------------------
	//创建Tick驱动
	INT nMaxFPS = s_pVariableSystem->GetAs_Int("System_MaxFPS");
	if(nMaxFPS < 5 || nMaxFPS >150) nMaxFPS = 30;

	m_hTickEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	::ResetEvent(m_hTickEvent);
	m_hEventTimer = ::timeSetEvent((INT)(1000.0f/nMaxFPS), (INT)(1000.0f/nMaxFPS), EventTimerProc, 0, TIME_PERIODIC|TIME_CALLBACK_FUNCTION); 

	// 2006-2-27
	// 初试化 人物创建过程, 在这里面创建了男 , 女人物主角, 避免在进入游戏过程中, 创建很慢.
	if(s_pProcCharCreate)
	{
		s_pProcCharCreate->CallInit();
	}//
}

VOID CGameProcedure::ReleaseStaticMember(VOID)
{
	//关闭驱动函数
	if(m_hEventTimer) ::timeKillEvent(m_hEventTimer);
	m_hEventTimer = NULL;
	CloseHandle(m_hTickEvent);
	m_hTickEvent = NULL;

	if ( s_pActionSetMgr != NULL )
	{
		s_pActionSetMgr->CleanUp();
		delete s_pActionSetMgr;
		s_pActionSetMgr = NULL;
	}
	delete s_pCampDataMgr;		s_pCampDataMgr = NULL;
	delete s_pSkillDataMgr;		s_pSkillDataMgr = NULL;
	delete s_pBulletDataMgr;	s_pBulletDataMgr = NULL;
	delete s_pBuffImpactMgr;	s_pBuffImpactMgr = NULL;
	delete s_pDirectlyImpactMgr;s_pDirectlyImpactMgr = NULL;

	//释放所有的循环实例
	if(s_pProcLogIn)			delete s_pProcLogIn;		s_pProcLogIn = NULL;
	if(s_pProcCharSel)			delete s_pProcCharSel;		s_pProcCharSel = NULL;
	if(s_pProcCharCreate)		delete s_pProcCharCreate;	s_pProcCharCreate = NULL;
	if(s_pProcEnter)			delete s_pProcEnter;		s_pProcEnter = NULL;
	if(s_pProcMain)				delete s_pProcMain;			s_pProcMain = NULL;
	if(s_pProcChangeScene)		delete s_pProcChangeScene;	s_pProcChangeScene = NULL;

	s_pProcPrev = s_pProcActive = NULL;

	//关闭所有工作的节点
//19. 资源提供
	SAFE_RELEASE(s_pResourceProvider);
//18. 外接帮助系统
	SAFE_RELEASE(s_pHelperSystem);
//17. UI模型显示管理
	SAFE_RELEASE(s_pFakeObjSystem);
//16. 操作管理
	SAFE_RELEASE( s_pActionSystem );
//15. 事件系统
	SAFE_RELEASE( s_pEventSystem );
//14. 脚本系统
	SAFE_RELEASE( s_pScriptSystem );
//13. 鼠标指针管理器
	SAFE_RELEASE( s_pCursorMng );
//12. UI数据池
	SAFE_RELEASE(s_pDataPool_);
	SAFE_RELEASE(s_pDataPool);
//11. 接口管理器
	SAFE_RELEASE( s_pGameInterface );
//10. 系统变量管理器
	SAFE_RELEASE( s_pVariableSystem );
//9. 数据库管理器
	SAFE_RELEASE( s_pDataBaseSystem );
//8. 世界管理器
	SAFE_RELEASE( s_pWorldManager );
//7. UI管理器
	SAFE_RELEASE( s_pUISystem );				
//6. 物体管理器
	SAFE_RELEASE( s_pObjectManager );
//5. 调试器节点
	SAFE_RELEASE( s_pDebuger );
//4. 渲染器节点
	SAFE_RELEASE( s_pGfxSystem );
//3. 音效管理器
	SAFE_RELEASE( s_pSoundSystem );
//2. 网络管理器
	SAFE_RELEASE( s_pNetManager );
//1. 计时器
	SAFE_RELEASE( s_pTimeSystem );
//0. 输入管理器
	SAFE_RELEASE( s_pInputSystem );

	//核心关闭
	g_theKernel.ShutDown();

	//释放主窗口
	DestroyMainWnd();
}

VOID CALLBACK CGameProcedure::EventTimerProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) 
{ 
	SetEvent(m_hTickEvent);
} 

VOID CGameProcedure::SetActiveProc(CGameProcedure* pProcToActive)
{
	if(NULL == pProcToActive || s_pProcActive == pProcToActive) return;

	s_pProcPrev = s_pProcActive;
	s_pProcActive = pProcToActive;
}

VOID CGameProcedure::MainLoop(VOID)
{
    MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (TRUE)
    {
		AUTO_SAMPLING theAutoSampling;

//		AxProfile::AxProfile_PushNode("Msg");
		//读取出消息队列中的所有消息并处理
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        { 
			//如果是退出消息,退出
            if (msg.message == WM_QUIT)  return;

            //处理其他消息
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		};
//		AxProfile::AxProfile_PopNode("Msg");

		// 等待有新的消息进入消息队列或者到达渲染时间
		UINT dwResult = MsgWaitForMultipleObjects(1, &m_hTickEvent, 
							FALSE, INFINITE, QS_ALLINPUT); 

        if (dwResult == WAIT_OBJECT_0)
        {
			//进入逻辑Tick和渲染
//			AxProfile::AxProfile_PushNode("Tick");
			TickActive();
//			AxProfile::AxProfile_PopNode("Tick");

//			AxProfile::AxProfile_PushNode("Render");
			RenderActive();
//			AxProfile::AxProfile_PopNode("Render");

//			AxProfile::AxProfile_PushNode("Event");
			ProcessGameEvent();
//			AxProfile::AxProfile_PopNode("Event");
        }
		//有新消息进入消息循环,处理消息
		else continue;
	}

}

VOID CGameProcedure::TickActive(VOID)
{
	//如果要转入新的游戏循环...
	if(s_pProcActive != s_pProcPrev) 
	{
		//调用旧循环的释放函数
		if(s_pProcPrev) s_pProcPrev->Release();
		//调用新循环的初始化函数
		if(s_pProcActive) s_pProcActive->Init();
		//开始新的循环
		s_pProcPrev = s_pProcActive;
	}
	//执行激活循环的数据逻辑
	if(s_pProcActive)	s_pProcActive->Tick();
}

VOID CGameProcedure::ProcessGameEvent(VOID)
{
	if(s_pEventSystem) 
		s_pEventSystem->ProcessAllEvent();
}

VOID CGameProcedure::RenderActive(VOID)
{
	//执行激活循环的渲染函数
	if(s_pProcActive == s_pProcPrev && s_pProcActive && !m_bMinimized && !m_bRenderingPaused)
	{
		//激活中，直接渲染
		if(m_bActive)
		{
			s_pProcActive->Render();
		}
		else
		{
			//使用窗口非法方式重绘
			InvalidateRect(g_hMainWnd, 0, FALSE);
			PostMessage(g_hMainWnd, WM_NCPAINT, TRUE, 0);
		}
	}
}

VOID CGameProcedure::Tick(VOID)
{
	//所有可执行节点的逻辑轮询

	s_pTimeSystem		->Tick();

//	AxProfile::AxProfile_PushNode("Tick_Input");
	s_pInputSystem		->Tick(); 
//	AxProfile::AxProfile_PopNode("Tick_Input");

//	AxProfile::AxProfile_PushNode("Tick_UI");
	if(s_pUISystem)s_pUISystem->Tick();
//	AxProfile::AxProfile_PopNode("Tick_UI");

//	AxProfile::AxProfile_PushNode("Tick_NetManager");
	s_pNetManager		->Tick(); 
//	AxProfile::AxProfile_PopNode("Tick_NetManager");

//	AxProfile::AxProfile_PushNode("Tick_World");
	s_pWorldManager		->Tick(); 
//	AxProfile::AxProfile_PopNode("Tick_World");

//	AxProfile::AxProfile_PushNode("Tick_Object");
	s_pObjectManager	->Tick(); 
//	AxProfile::AxProfile_PopNode("Tick_Object");

//	AxProfile::AxProfile_PushNode("Tick_Debug");
	if(s_pDebuger)s_pDebuger->Tick(); 
//	AxProfile::AxProfile_PopNode("Tick_Debug");

//	AxProfile::AxProfile_PushNode("Tick_Sound");
	s_pSoundSystem		->Tick();
//	AxProfile::AxProfile_PopNode("Tick_Sound");

//	AxProfile::AxProfile_PushNode("Tick_Gfx");
	s_pGfxSystem		->Tick();
//	AxProfile::AxProfile_PopNode("Tick_Gfx");

//	AxProfile::AxProfile_PushNode("Tick_DataPool");
	s_pDataPool			->Tick();
	s_pDataPool_		->Tick();
//	AxProfile::AxProfile_PopNode("Tick_DataPool");

//	AxProfile::AxProfile_PushNode("Tick_Action");
	s_pActionSystem		->Tick();
//	AxProfile::AxProfile_PopNode("Tick_Action");

//	AxProfile::AxProfile_PushNode("Tick_Fake");
	s_pFakeObjSystem	->Tick();
//	AxProfile::AxProfile_PopNode("Tick_Fake");

	s_pScriptSystem		->Tick();

//	PROFILE_POP(MainTick);

}

VOID CGameProcedure::ProcessActiveInput(VOID)
{
	//执行激活循环的键盘函数
	if(s_pProcActive == s_pProcPrev && s_pProcActive )
	{
		s_pProcActive->ProcessInput();
	}
}

VOID CGameProcedure::ProcessCloseRequest(VOID)
{
	//执行激活循环的渲染函数
	if(s_pProcActive == s_pProcPrev && s_pProcActive )
	{
		s_pProcActive->CloseRequest();
	}
}

VOID CGameProcedure::ProcessInput(VOID)
{

}

VOID CGameProcedure::CloseRequest(VOID)
{
}

VOID CGameProcedure::CreateMainWnd(VOID)
{
	//------------------------------------------------------
	//计算窗口大小
	m_bMinimized		= FALSE;
	m_bFullScreen		= TRUE;
	fVector2 fResoution;
	if (CGameProcedure::s_pVariableSystem->GetAs_Int("View_FullScreen"))
	{
		fResoution.x  = (float)::GetSystemMetrics(SM_CXSCREEN);
		fResoution.y  = (float)::GetSystemMetrics(SM_CYSCREEN);	
	}
	else
	{
		fResoution = s_pVariableSystem->GetAs_Vector2("View_Resoution");
	}
	CGameProcedure::m_fWindowFOV = fResoution.x / fResoution.y;
	SetRect(&m_rectWindow, 0, 0, (int)fResoution.x, (int)fResoution.y);
	AdjustWindowRect(&m_rectWindow, DEFWINDOW_STYLE, FALSE);

	SetRect(&m_rectFCOffset, m_rectWindow.left, m_rectWindow.top, 
		m_rectWindow.right-(int)fResoution.x, m_rectWindow.bottom-(int)fResoution.y);

	UINT dwX = (::GetSystemMetrics(SM_CXFULLSCREEN)-(m_rectWindow.right-m_rectWindow.left))/2;
	UINT dwY = (::GetSystemMetrics(SM_CYFULLSCREEN)-(m_rectWindow.bottom-m_rectWindow.top))/2;
	OffsetRect(&m_rectWindow, -m_rectFCOffset.left, -m_rectFCOffset.top);
	OffsetRect(&m_rectWindow, dwX, dwY);

	m_bActive			= true;
	m_bRenderingPaused	= FALSE;

	//------------------------------------------------------
	//注册窗口类
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= (WNDPROC)_MainWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= g_hInstance;
	wcex.hIcon			= LoadIcon(g_hInstance, (LPCTSTR)IDD_GAME_DIALOG);
	wcex.hCursor		= LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground	= (HBRUSH)NULL; //GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName	= (LPCTSTR)NULL;
	wcex.lpszClassName	= MAINWINDOW_CLASS;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	RegisterClassEx(&wcex);

	//------------------------------------------------------
	//创建窗口
	CHAR szTitle[MAX_PATH];
	_snprintf(szTitle, MAX_PATH, "%s %s (%s %s)", GAME_TITLE, VERSION_INFO, __DATE__, __TIME__);

	HWND hWnd = CreateWindowEx(NULL, MAINWINDOW_CLASS, szTitle, 
		DEFWINDOW_STYLE,
		m_rectWindow.left, m_rectWindow.top, 
		m_rectWindow.right-m_rectWindow.left, 
		m_rectWindow.bottom-m_rectWindow.top,
		NULL, NULL, g_hInstance, NULL);

	if (!hWnd)
	{
		TDThrow(_T("Can't create main window!"));
	}

	if (CGameProcedure::s_pVariableSystem->GetAs_Int("View_FullScreen"))
	{
		s_pEventSystem->PushEvent(GE_VARIABLE_CHANGED, "View_FullScreen", "1");
		s_pEventSystem->PushEvent(GE_CHAT_ADJUST_MOVE_CTL, (INT)fResoution.x, (INT)fResoution.y);
	}else
	{
		const STRING& strVariable = CGameProcedure::s_pVariableSystem->GetAs_String("View_Resoution");
		s_pEventSystem->PushEvent(GE_VARIABLE_CHANGED,"View_Resoution", strVariable.c_str());
		s_pEventSystem->PushEvent(GE_CHAT_ADJUST_MOVE_CTL, (INT)fResoution.x, (INT)fResoution.y);
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	g_hMainWnd = hWnd;
}

VOID CGameProcedure::DestroyMainWnd(VOID)
{
	DestroyWindow( g_hMainWnd );
	UnregisterClass( MAINWINDOW_CLASS, NULL );
	g_hMainWnd = NULL;
}

//消息响应函数
LRESULT CALLBACK CGameProcedure::_MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//--------------------------------------------------------------
	//首先让输入管理器处理消息
	if(s_pInputSystem && s_pInputSystem->MessageProc( hWnd, message, wParam, lParam ))
		return 0;

	//--------------------------------------------------------------
	//UI处理文字输入
	if(s_pUISystem && s_pUISystem->MessageProc( hWnd, message, wParam, lParam ))
		return 0;

	//执行激活循环的消息响应函数
	if(s_pProcActive )
	{
		return s_pProcActive->MainWndProc(hWnd, message, wParam, lParam);
	}
	else return DefWindowProc(hWnd, message, wParam, lParam);
}

//缺省消息响应函数,可被相应的流程继承
LRESULT CGameProcedure::MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//--------------------------------------------------------------
	//处理一些缺省的消息
	switch (message) 
	{
	//窗口大小发生改变
	case WM_SIZE:
		{
			//最小化
            if( SIZE_MINIMIZED == wParam )
            {
                m_bRenderingPaused = true; // 最小化时渲染暂停

                m_bMinimized = true;
				m_bMaximized = FALSE;
            }
			//最大化
            else if( SIZE_MAXIMIZED == wParam )
            {
                m_bRenderingPaused = FALSE; //恢复渲染工作(可能上一个状态时最小化)

				m_bMinimized = FALSE;
				m_bMaximized = true;
				HandlePossibleSizeChange( message, wParam, lParam);
			}
			//恢复窗口
            else if( SIZE_RESTORED == wParam )
            {
                if( m_bMaximized )	//刚才是最大化
                {
                    m_bMaximized = FALSE;
                    HandlePossibleSizeChange(message, wParam, lParam);
                }
                else if( m_bMinimized )	//刚才是最小化
                {
                    m_bRenderingPaused = FALSE; 

					m_bMinimized = FALSE;
                    HandlePossibleSizeChange(message, wParam, lParam);
                }
                else
                {
					//如果上一个状态既不是最大化也不是最小化,显然这次的WM_SIZE
					//消息是因为用户拖动窗口边缘引起的，这种情况我们并不马上恢复
					//设备，我们等待用户停止拖动后在处理，也就是对WM_EXITSIZEMOVE
					//的处理...
                }
            }
		}
		break;

	//用户拖动窗口开始
	case WM_ENTERSIZEMOVE:
		{
			m_bRenderingPaused = true;
		}
		break;
	//用户拖动中
	case WM_SIZING:
		{
			RECT* pRect = (RECT*)lParam;

			switch(wParam)
			{
			case WMSZ_RIGHT:
			case WMSZ_BOTTOMRIGHT:
				{
					KeepWindowFOV(pRect, WMSZ_RIGHT, WMSZ_TOPLEFT);
				}
				break;
			case WMSZ_LEFT:
			case WMSZ_BOTTOMLEFT:
				{
					KeepWindowFOV(pRect, WMSZ_LEFT, WMSZ_TOPRIGHT);
				}
				break;

			case WMSZ_TOP:
 			case WMSZ_TOPRIGHT:
				{
					KeepWindowFOV(pRect, WMSZ_TOP, WMSZ_BOTTOMLEFT);
				}
				break;

			case WMSZ_BOTTOM:
				{
					KeepWindowFOV(pRect, WMSZ_BOTTOM, WMSZ_TOPLEFT);
				}
				break;

			case WMSZ_TOPLEFT:
				{
					KeepWindowFOV(pRect, WMSZ_TOP, WMSZ_BOTTOMRIGHT);
				}
				break;
			}
			return TRUE;
		}
		break;
	//窗口尺寸发生了改变后
	case WM_WINDOWPOSCHANGING:
		// fujia 2008.1.23 防止窗口模式窗口大小发生变化
		//{    
		//	WINDOWPOS* pPos = (WINDOWPOS*)lParam;
		//	//非大小改变
		//	if(pPos->flags & SWP_NOSIZE || pPos->flags & SWP_DRAWFRAME) break;
		//	//是“全屏”状态
		//	if(s_pVariableSystem && 
		//		s_pVariableSystem->GetAs_Int("View_FullScreen")) break;

		//	//计算新的Client
		//	RECT rectFrame;
		//	SetRect(&rectFrame, 0, 0, pPos->cx, pPos->cy);
		//	OffsetRect(&rectFrame, pPos->x, pPos->y);

		//	RECT rectNewFrame;
		//	CopyRect(&rectNewFrame, &rectFrame);
		//	KeepWindowFOV(&rectNewFrame, -1, WMSZ_TOPLEFT);

		//	//如果相同不处理
		//	if(EqualRect(&rectFrame, &rectNewFrame)) break;

		//	pPos->x = rectNewFrame.left;
		//	pPos->y = rectNewFrame.top;
		//	pPos->cx = rectNewFrame.right-rectNewFrame.left;
		//	pPos->cy = rectNewFrame.bottom-rectNewFrame.top;

		//	return 0;
		//}
		break;

	//桌面分辨率改变
	case WM_DISPLAYCHANGE:
		{
			if( s_pEventSystem && s_pVariableSystem->GetAs_Int("View_FullScreen"))
			{
				s_pEventSystem->PushEvent(GE_VARIABLE_CHANGED, "View_FullScreen", "1");
			}
			//重置分辨率
			if(m_bMaximized)
			{
				ShowWindow(g_hMainWnd, SW_RESTORE);
				ShowWindow(g_hMainWnd, SW_MAXIMIZE);
			}
			else 
			{
				s_pEventSystem->PushEvent(GE_VARIABLE_CHANGED, "View_Resoution", 
					s_pVariableSystem->GetAs_String("View_Resoution").c_str());
			}
			break;
		}
	//用户拖动窗口边缘结束
	case WM_EXITSIZEMOVE:
		{
			m_bRenderingPaused = FALSE;
			HandlePossibleSizeChange(message, wParam, lParam);

			//设置分辨率变量
			if(s_pVariableSystem)
			{
				RECT rect;
				GetClientRect(hWnd, &rect);
				char szTemp[MAX_PATH];
				_snprintf(szTemp, MAX_PATH, "%d,%d", rect.right-rect.left, rect.bottom-rect.top);

				s_pVariableSystem->SetVariable("View_Resoution", szTemp, FALSE, FALSE);
				
			}
		}
		break;

	//决定窗口最大最小尺寸
	case WM_GETMINMAXINFO:
		{
			MINMAXINFO* pMinMaxInfo = (MINMAXINFO*)lParam;

			pMinMaxInfo->ptMinTrackSize.x = MINWINDOW_WIDTH;
			pMinMaxInfo->ptMinTrackSize.y = MINWINDOW_HEIGHT;

			//最大尺寸为“窗口最大化”时的大小
			RECT rect;
			SetRect(&rect, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			AdjustWindowRect(&rect, GetWindowStyle(g_hMainWnd), FALSE);

			pMinMaxInfo->ptMaxSize.x = rect.right-rect.left;
			pMinMaxInfo->ptMaxSize.y = rect.bottom-rect.top;

			pMinMaxInfo->ptMaxTrackSize.x = rect.right-rect.left;
			pMinMaxInfo->ptMaxTrackSize.y = rect.bottom-rect.top;
		}
		break;

	//设置光标
	case WM_SETCURSOR:
		{
			switch(LOWORD(lParam))
			{
			case HTCLIENT:
				{
					if(s_pCursorMng)
					{
						s_pCursorMng->OnSetCursor();
						return TRUE;
					}
				}
				break;

			default:
				break;
			}
		}
		break;

	//窗口激活切换
	case WM_ACTIVATEAPP:
		if( wParam == TRUE )
		{
			m_bActive = TRUE;
		}
		else 
		{
			m_bActive = FALSE;
		}
		//通知输入器
		if(s_pInputSystem) ((CInputSystem*)s_pInputSystem)->OnActiveApp(m_bActive);
		break;
	//消息主循环移到菜单中
	case WM_ENTERMENULOOP:
		{
			m_bRenderingPaused = true;
		}
		break;

	//消息主循环移出菜单
	case WM_EXITMENULOOP:
		{
			m_bRenderingPaused = FALSE;
		}
		break;
	
	//系统按键
	case WM_SYSCOMMAND:
		{
			switch(wParam)
			{
			//屏蔽F10
			case SC_KEYMENU:
				return 1;

			default:
				break;
			}
		}
		break;
	
	//窗口重绘消息
	case WM_PAINT:
		{
			PAINTSTRUCT psStruct;
			::BeginPaint(hWnd, &psStruct);
			::EndPaint(hWnd, &psStruct);
			s_pGfxSystem->OnPaint();

		}
		break;

	//窗口背景刷
	case WM_ERASEBKGND:
		{
			return 1;
		}
		break;

	//处理玩家的退出请求
	case WM_CLOSE:
		{
			//主流程中,弹出设置菜单
			if(GetActiveProcedure() == s_pProcMain && s_pUISystem)
			{
				if(wParam==0xC0DE && lParam==0XC0DE)
				{
					ProcessCloseRequest();
					break;
				}
				else
				{
					s_pEventSystem->PushEvent( GE_TOGLE_SYSTEMFRAME );
					return TRUE;
				}
			}
		}
		break;
	//窗口销毁
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
	//输入法改变
	case WM_INPUTLANGCHANGE:
		{
			if(s_pEventSystem)
			{
				s_pEventSystem->PushEvent(GE_CHAT_INPUTLANGUAGE_CHANGE);
			}
		}
		break;
	case WM_IME_NOTIFY:
		{
			if(wParam == IMN_SETOPENSTATUS || wParam == IMN_SETCONVERSIONMODE)
			{
				if(s_pEventSystem)
				{
					s_pEventSystem->PushEvent(GE_CHAT_INPUTLANGUAGE_CHANGE);
				}
			}
		}
		break;
	default:
		break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

//用以保持窗口比例不发生改变
//最大化除外
VOID CGameProcedure::KeepWindowFOV(RECT* pRect, UINT dwChanging, UINT dwAnchor)
{
	RECT rectTemp;
	CopyRect(&rectTemp, pRect);

	rectTemp.left -= m_rectFCOffset.left;
	rectTemp.top -= m_rectFCOffset.top;
	rectTemp.right -= m_rectFCOffset.right;
	rectTemp.bottom -= m_rectFCOffset.bottom;

	if(WMSZ_LEFT == dwChanging || WMSZ_RIGHT == dwChanging)
	{
		//宽不变
		rectTemp.bottom = rectTemp.top + (INT)((rectTemp.right-rectTemp.left)*m_fWindowFOV);
	}
	else if(WMSZ_TOP == dwChanging || WMSZ_BOTTOM == dwChanging)
	{
		//高不变
		rectTemp.right = rectTemp.left + (INT)((rectTemp.bottom-rectTemp.top)/m_fWindowFOV);
	}
	else
	{
		//根据比例自动调节
		FLOAT fFov = (FLOAT)(rectTemp.bottom-rectTemp.top)/(FLOAT)(rectTemp.right-rectTemp.left);
		if(fFov > m_fWindowFOV)
		{
			rectTemp.bottom = rectTemp.top + (INT)((rectTemp.right-rectTemp.left)*m_fWindowFOV);
		}
		else
		{
			rectTemp.right = rectTemp.left + (INT)((rectTemp.bottom-rectTemp.top)/m_fWindowFOV);
		}
	}

	AdjustWindowRect(&rectTemp, DEFWINDOW_STYLE, FALSE);

	switch(dwAnchor)
	{
	case WMSZ_TOPLEFT:
		{
			pRect->right = pRect->left + (rectTemp.right-rectTemp.left);
			pRect->bottom = pRect->top + (rectTemp.bottom-rectTemp.top);
		}
		break;

	case WMSZ_TOPRIGHT:
		{
			pRect->left = pRect->right - (rectTemp.right-rectTemp.left);
			pRect->bottom = pRect->top + (rectTemp.bottom-rectTemp.top);
		}
		break;

	case WMSZ_BOTTOMLEFT:
		{
			pRect->right = pRect->left + (rectTemp.right-rectTemp.left);
			pRect->top = pRect->bottom - (rectTemp.bottom-rectTemp.top);
		}
		break;

	case WMSZ_BOTTOMRIGHT:
		{
			pRect->left = pRect->right - (rectTemp.right-rectTemp.left);
			pRect->top = pRect->bottom - (rectTemp.bottom-rectTemp.top);
		}
		break;
	}
}

VOID CGameProcedure::HandlePossibleSizeChange(UINT message, WPARAM wParam, LPARAM lParam)
{
	//------------------------------------------------------
	//检查合法性
	if(!g_hMainWnd || !s_pGfxSystem) return;

	RECT rect;
	GetClientRect(g_hMainWnd, &rect);

	//------------------------------------------------------
	//恢复设备
	s_pGfxSystem->OnSizeChange(message, wParam, lParam);
	if(s_pUISystem)s_pUISystem->OnSizeChange(message, wParam, lParam);

	if(s_pEventSystem)
	{
		s_pEventSystem->PushEvent(GE_VIEW_RESOLUTION_CHANGED, (INT)message);
	}

	if(s_pObjectManager)
	{
		((CObjectManager*)s_pObjectManager)->OnScreenSizeChanged();
	}

	if(s_pDebuger)
	{
		if(rect.right>rect.left && rect.bottom>rect.top)
		{
			s_pDebuger->InfoTrack_Reset(rect.right-rect.left, rect.bottom-rect.top);
		}
	}
}

// 得到前一个游戏流程.
CGameProcedure*	CGameProcedure::GetPreProcedure()
{
	return s_pProcPrev;
}
