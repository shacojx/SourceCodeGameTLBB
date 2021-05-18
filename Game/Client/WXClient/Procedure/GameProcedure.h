/****************************************\
*										*
* 			   游戏流程基类				*
*										*
\****************************************/

#pragma once
#include "CampAndStand.h"
#include "TDUISystem.h"
#include <vector>
#include <string>

class CGamePro_Login;
class CGamePro_CharSel;
class CGamePro_CharCreate;
class CGamePro_Enter;
class CGamePro_Main;
class CGamePro_ChangeScene;
class tNode;
class CNetManager;
class tInputSystem;
class tWorldSystem;
class tObjectSystem;
class CRenderSystem;
class tTimeSystem;
class tSoundSystem;
class tDataBaseSystem;
class tVariableSystem;
class tGameInterfaceBase;
class CMissionDataMgr;
class CTextResMgr;
class CBuffImpactMgr;
class CDirectlyImpactMgr;
class CBulletDataMgr;
class CSkillDataMgr;
class CActionSetMgr;
class tDebuger;
class tUISystem;
class tDataPool;
class tDataPool_;
class tCursorSystem;
class tScriptSystem;
class tEventSystem;
class tActionSystem;
class tFakeObjSystem;
class tHelperSystem;
class CResourceProvider;
class tSoundSource;

//must use STRING as szMsg's type
#define ADDNEWDEBUGMSG( szMsg )	\
	if(CGameProcedure::s_pEventSystem) \
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, (szMsg)##.c_str());

/*LOGMSGFUNC_EXAMPLE
 |	BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
 |	STRING format("log:%s"), msg("funcname");
 |
 |	if(bLog)
 |	{
 |		NOCOLORMSGFUNC(format.c_str(), msg.c_str());
 |		COLORMSGFUNC(format.c_str(), msg.c_str());
 |	}
 |  ......
 */

//care use!!!! CGameProcedure::s_pUISystem must not NULL see LOGMSGFUNC_EXAMPLE
#define NOCOLORMSGFUNC	\
	CGameProcedure::s_pUISystem->ParserString_NoColor_VarParam
		
//care use!!!! CGameProcedure::s_pUISystem must not NULL see LOGMSGFUNC_EXAMPLE
#define COLORMSGFUNC \
	CGameProcedure::s_pUISystem->ParserString_VarParam

#define NOPARAMMSGFUNC( szMsg )	\
	(CGameProcedure::s_pUISystem)?(CGameProcedure::s_pUISystem->ParserString_NoColor_NoVarParam( (STRING(szMsg).c_str()) )):"";



// 不合法字符串定义
typedef std::vector<std::string>  UNUSE_STRING_VECTOR;

class CGameProcedure 
{
public:
	/// 初始化静态变量
	static VOID		InitStaticMemeber(VOID);
	///	将一个游戏循环激活
	static VOID		SetActiveProc(CGameProcedure* pToActive);
	/// 进入当前游戏循环的数据逻辑函数
	static VOID		TickActive(VOID);
	/// 让事件系统处理游戏事件
	static VOID		ProcessGameEvent(VOID);
	/// 键盘事件和鼠标事件的处理
	static VOID		ProcessActiveInput(VOID);
	/// 玩家请求退出程序事件
	static VOID		ProcessCloseRequest(VOID);
	/// 进入当前游戏循环的渲染函数
	static VOID		RenderActive(VOID);
	/// 释放静态变量
	static VOID		ReleaseStaticMember(VOID);
	/// 消息主循环
	static VOID		MainLoop(VOID);
	///处理窗口大小发生改变的情况
	static VOID		HandlePossibleSizeChange(UINT message, WPARAM wParam, LPARAM lParam);

	/// 得到当前激活的循环
	static CGameProcedure*	GetActiveProcedure(VOID) { return s_pProcActive; }
	/// 主窗口是否处于激活状态
	static BOOL				IsWindowActive(VOID) { return m_bActive; }
	
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 游戏运行的流程.
	//
	
	/// 登录游戏循环
	static CGamePro_Login*			s_pProcLogIn;
	/// 人物选择流程
	static CGamePro_CharSel*		s_pProcCharSel;
	/// 人物创建流程
	static CGamePro_CharCreate*		s_pProcCharCreate;
	/// 等待进入场景流程
	static CGamePro_Enter*			s_pProcEnter;
	/// 主游戏循环
	static CGamePro_Main*			s_pProcMain;
	/// 当前激活的流程.
	static CGameProcedure*          s_pActiveProcedure;
	
	// 切换服务器流程
	static CGamePro_ChangeScene*	s_pProcChangeScene;

	//
	// 游戏运行的过程
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///输入管理器
	static tInputSystem*			s_pInputSystem;
	///网络管理器
	static CNetManager*				s_pNetManager;
	///场景管理器
	static tWorldSystem*			s_pWorldManager;
	///物体管理器
	static tObjectSystem*			s_pObjectManager;
	///调试器
	static tDebuger*				s_pDebuger;
	/// 渲染层指针
	static CRenderSystem*			s_pGfxSystem;
	/// UI系统
	static tUISystem*				s_pUISystem;
	/// 计时器
	static tTimeSystem*				s_pTimeSystem;
	// 声音管理器
	static tSoundSystem*			s_pSoundSystem;
	// 数据库管理器
	static tDataBaseSystem*			s_pDataBaseSystem;
	// 变量管理器
	static tVariableSystem*			s_pVariableSystem;
	// 逻辑接口管理器
	static tGameInterfaceBase*		s_pGameInterface;
	// UI数据池
	static tDataPool*				s_pDataPool;
	static tDataPool_*				s_pDataPool_;
	// 光标管理器
	static tCursorSystem*			s_pCursorMng;
	//脚本管理器
	static tScriptSystem*			s_pScriptSystem;
	//游戏事件管理器
	static tEventSystem*			s_pEventSystem;
	//游戏操作管理器
	static tActionSystem*			s_pActionSystem;
	//UI模型显示管理
	static tFakeObjSystem*			s_pFakeObjSystem;
	//外接帮助系统
	static tHelperSystem*			s_pHelperSystem;
	//资源提供
	static CResourceProvider*		s_pResourceProvider;

	static CBuffImpactMgr			*s_pBuffImpactMgr;
	static CDirectlyImpactMgr		*s_pDirectlyImpactMgr;
	static CBulletDataMgr			*s_pBulletDataMgr;
	static CSkillDataMgr			*s_pSkillDataMgr;
	static CMissionDataMgr			*s_pMissionDataMgr;
	//阵营
	static CampAndStandDataMgr_T	*s_pCampDataMgr;

	static CActionSetMgr			*s_pActionSetMgr;

protected:
	/// 当前激活的循环
	static CGameProcedure*			s_pProcActive;
	/// 上一个激活的循环，只在切换过程中使用
	static CGameProcedure*			s_pProcPrev;

public:

	// 得到当前激活的游戏流程.
	//static CGameProcedure*	GetActiveProcedure();

	// 得到前一个游戏流程.
	static CGameProcedure*	GetPreProcedure();

protected:
	virtual VOID	Init(VOID) = 0;
	virtual VOID	Tick(VOID);
	virtual VOID	Render(VOID) = 0;
	virtual VOID	Release(VOID) = 0;
	virtual VOID	ProcessInput(VOID);
	virtual VOID	CloseRequest(VOID);
	virtual LRESULT	MainWndProc(HWND, UINT, WPARAM, LPARAM);

protected:
	//-------------------------------------------
	//内部控制变量

	//窗口最小化
	static BOOL			m_bMinimized;
	//窗口最大化
	static BOOL			m_bMaximized;
	//窗口全屏化
	static BOOL			m_bFullScreen;
	//窗口位置
	static RECT			m_rectWindow;
	//窗口比例(3:4=0.75)
	static FLOAT		m_fWindowFOV;
	//Frame<->Client的差值
	static RECT			m_rectFCOffset;
	//窗口处于焦点状态
	static BOOL			m_bActive;
	//渲染暂停
	static BOOL			m_bRenderingPaused;
	//ui获得输入状态
	static BOOL			m_bUIHandleInput;

	//桢速控制
	static FLOAT		m_fFPS;
	//系统Tick驱动事件
	static MMRESULT		m_hEventTimer;
	//Tick通知Event
	static HANDLE		m_hTickEvent;
	//系统Tick驱动函数
	static VOID CALLBACK EventTimerProc(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
	//Render Event
	static HANDLE		m_hRenderEvent;

public:
	static bool			m_bNeedFreshMinimap;
	static bool			m_bWaitNeedFreshMinimap;

private:
	///创建主窗口
	static VOID		CreateMainWnd(VOID);
	///释放主窗口
	static VOID		DestroyMainWnd(VOID);
	///主窗口消息处理函数
	static LRESULT CALLBACK	_MainWndProc(HWND, UINT, WPARAM, LPARAM);
	///处理窗口比例发生改变时
	/* dwChanging-
	|		WMSZ_LEFT/WMSZ_RIGHT
	|		WMSZ_TOP/WMSZ_BOTTOM
	| dwAnchor-
	|		WMSZ_TOPLEFT    
	|		WMSZ_TOPRIGHT   
	|		WMSZ_BOTTOM     
	|		WMSZ_BOTTOMLEFT 
	|		WMSZ_BOTTOMRIGHT
	*/
	static VOID		KeepWindowFOV(RECT* pRect, UINT dwChanging, UINT dwAnchor);
};
