#pragma once
#include "tdbasictype.h"
#include <list>
#include "TDEventSystem.h"
#include "ceguivector.h"
// 窗口管理类，在这里管理已经编辑好的类

namespace CEGUI
{
	class Window;
};
class tScriptEnvironment;
struct _DBC_UI_LAYOUTDEFINE;

namespace LuaPlus
{
	class LuaState;
}
namespace LUA_CONTROL
{
	class Window;
}

// 单独一个窗口
class CUIWindowItem
{
protected:
	DWORD				m_dwID;					// ID
	STRING				m_strWindowName;		// 窗口名字，独一无二的
	STRING				m_strLayoutFileName;	// layout文件名
	STRING				m_strScriptFileName;	// 脚本文件名
	int					m_nIsCanDemise;			// 是否可以自动滚动显示,屏幕最多可以显示两个可禅让窗口
	CEGUI::Window*		m_pWindow;				// 窗口指针
	BOOL				m_bShow;				//是否显示
	tScriptEnvironment*	m_pScriptEnv;			//脚本环境空间
	BOOL				m_bLayoutLoaded;		// 界面是否已经被加载

public:
	//预加载
	VOID			PreLoadWindow(VOID);
	// 加载窗口
	VOID			LoadWindow(VOID);
	// 显示/隐藏窗口
	VOID			Show(BOOL bShow);
	// 窗口是否已经打开了
	BOOL			IsWindowShow(VOID) const { return m_bShow; }
	//得到窗口名
	LPCTSTR			GetWindowName(VOID) const { return m_strWindowName.c_str(); }
	// 将窗口移动某一个位置
	VOID			MoveTo( CEGUI::Point&  newPos );
	// 自适应窗口位置
	void			PositionSelf();
	//加载窗口时属性回调函数
	static bool		callbackProperty(CEGUI::Window* window, CEGUI::String& propname, CEGUI::String& propvalue, void* userdata);

	//------------------------------------------------------------
	//脚本中调用的函数
public:
	//注册关注事件
	INT				LUA_RegisterEvent(LuaPlus::LuaState* pState);
	//显示
	INT				LUA_Show(LuaPlus::LuaState* pState);
	//隐藏
	INT				LUA_Hide(LuaPlus::LuaState* pState);
	//显示/隐藏
	INT				LUA_TogleShow(LuaPlus::LuaState* pState);
	//是否显示
	INT				LUA_IsVisible(LuaPlus::LuaState* pState);
	//转化所有窗口上的文字
	INT				LUA_TransAllWindowText(LuaPlus::LuaState* pState);
	//关心与某逻辑Obj的某些事件
	INT				LUA_CareObject(LuaPlus::LuaState* pState);

	//------------------------------------------------------------
	//响应窗口关闭
protected:
	void onWindowHidden(void);

	//------------------------------------------------------------
	//游戏事件通知
protected:
	static VOID	WINAPI	_OnGameEvent(const EVENT* pEvent, UINT dwOwnerData);
	
	//------------------------------------------------------------
	//控件<->脚本操作
protected:
	//注册控件
	VOID			_RegisterControlToScript(CEGUI::Window* pWindow);
	//保存脚本控制实例
	std::vector< LUA_CONTROL::Window* > m_vAllControl;

	//------------------------------------------------------------
	//文本转化
protected:
	VOID			_TransWindowText(CEGUI::Window* pWindow);

	//------------------------------------------------------------
	//处理用户操作事件
public:
	VOID			FireUIEvent(LPCTSTR szEventHandle, CEGUI::Window* pWindow);

public:
	CUIWindowItem(const _DBC_UI_LAYOUTDEFINE* pLayoutDef);
	virtual ~CUIWindowItem();


	BOOL			IsCanDemise(){ return m_nIsCanDemise > 1; };
	int				GetDemiseType() { return m_nIsCanDemise; };
};

//========================================================
class CUIWindowMng
{
public:
	static CEGUI::Window*	GetClientScreen(VOID) { return CUIWindowMng::m_pBackGroundSheet; };
	static CEGUI::Window*   m_pBackGroundSheet; // 客户端窗口底板
	
	//初始化
	VOID		Init(VOID);
	// 关闭所有窗口
	VOID		HideAllWindow(VOID); 
	//某组ui是否处于打开中
	BOOL		IsWindowShow(LPCTSTR szUIName);

public:
	CUIWindowMng();
	virtual ~CUIWindowMng();

	static CUIWindowMng* GetMe(void) { return s_pMe; }


	VOID		DemiseWindow( CUIWindowItem* pItem, BOOL bShow );
	void		DemiseWindowTest( CUIWindowItem* pItem, BOOL bShow );
	bool		CloseAllWindow();

	void		OnEscape(); // 响应esc键
protected:
	static CUIWindowMng* s_pMe;

	// 窗口管理器
	typedef std::list< CUIWindowItem* >  WINDOWLIST;
	typedef std::vector< CUIWindowItem* >  WINDOWVECTOR;
	// 所有窗口的列表
	WINDOWLIST   m_aWindowList; 
};
