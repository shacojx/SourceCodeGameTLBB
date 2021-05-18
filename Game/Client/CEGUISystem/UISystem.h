#pragma once
#include "UIGlobal.h"
#include "TDUISystem.h"
#include <OgreRoot.h>
#include <imm.h>
#include "CEGUIString.h"

namespace Ogre
{
	class Root;
};

namespace CEGUI
{
	class System;
	class OgreCEGUIRenderer;
	class CGameUIScript;
	class FalagardActionButton;
	class EventArgs;
	class Window;
};

class CUIWindowMng;
class CUIIconsManager;
class CUIStringSystem;
class CUICreatureBoardSystem;

class CUISystem : public tUISystem
{
public:
	// 窗口大小发生改变
	virtual VOID			OnSizeChange(UINT message, WPARAM wParam, LPARAM lParam);
	// 处理输入
	virtual VOID			InjectInput(VOID);
	// 文字输入消息
	virtual BOOL			MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	//鼠标是否在ui上空
	virtual BOOL			IsMouseHover(VOID);
	//鼠标是否在聊天信息上空
	virtual BOOL			IsMouseHover_ChatBoard(LONG x, LONG y);
	//获得聊天信息里的超链中的信息
	virtual STRING			getChatBoard_HyplinkContex(LONG x, LONG y);
	//创建一个新的角色信息板
	virtual tCreatureBoard*	CreateCreatureBoard(VOID);
	virtual VOID			AddNewBeHitBoard( bool bDouble, LPCTSTR szInfo, FLOAT nStartX, FLOAT nStartY,int nType = 0, int nMoveType = 0);
	//某一组UI是否处于打开中
	virtual BOOL			IsWindowShow(LPCTSTR szUIName);
	virtual BOOL			CloseAllWindow();
	//通知UI创建一组UI显示模型
	virtual VOID			FakeObj_Show(LPCTSTR szWinName, LPCTSTR szName);
	//通知UI销毁一组UI显示模型
	virtual VOID			FakeObj_Close(LPCTSTR szWinName, LPCTSTR szName);
	
	//--- FOR DEBUG
	virtual VOID		Debug_PushChatString(LPCTSTR szName, LPCTSTR szMsg)	{};
	virtual VOID		Debug_SaveFontTexture(VOID);
	//--- FOR DEBUG
	virtual VOID			ChangeDragFlag(bool Flag);// {m_bIsDragEnd = Flag;}
	virtual BOOL			IsDragFlag(VOID); //{return m_bIsDragEnd;}

	//逻辑层添加系统通知信息

	//查找字典文件中strID对应的字串，并设置其中的可变参数（%s,%d,....etc）
	virtual STRING			ParserString_VarParam(LPCTSTR strID, ... );
	//查找字典文件中strID对应的字串，并设置其中的可变参数（%s,%d,....etc），然后去掉字串中的颜色控制符
	virtual STRING			ParserString_NoColor_VarParam(LPCTSTR strID, ... );

	//将strSrc字串中的颜色控制符(#B,#Y.....#c00FF00FF....etc)去掉，将结果放到strOut
	virtual VOID			ParserString_NoColor(const STRING& strSrc, STRING& strOut, BOOL bControl = FALSE);

	//查找字典文件中strID对应的字串，但不修改其中的可变参数（%s,%d,....etc）
	virtual STRING			ParserString_NoVarParam(LPCTSTR strID);
	//查找字典文件中strID对应的字串，但不修改其中的可变参数（%s,%d,....etc），然后去掉字串中的颜色控制符
	virtual STRING			ParserString_NoColor_NoVarParam(LPCTSTR strID);

	//聊天模板字串的取得
	virtual STRING			getTalkTemplate(const STRING& strKey, UINT index);
	virtual STRING			ModifyChatTemplate(const STRING& strTemp, const STRING& strTalker, const STRING& strTarget);

	virtual STRING			getTalkRandHelpMsg();

	//查看当前获取输入的EditBox
	virtual BOOL			IsIMEEditboxActive(LPCTSTR szWinName);
	virtual BOOL			IsIMEEditboxEmpty(VOID);
	//输入法状态
	virtual INT				GetCurrentIMEState();

	virtual void			OnPushEcsape();

	virtual void			SetPaoPaoActive( bool bActive ) { m_bActivePaoPao = bActive; };

	virtual bool			IsPaoPaoActive() { return m_bActivePaoPao; };

	//颜色的转化
//	FLOAT		OgreColor_CEGUIColor(const Ogre::ColourValue OgreColor);
	virtual UINT				Lumination_OgreColor( UINT OgreColor, INT Lumination );
	virtual INT					OgreColor_Lumination( UINT OgreColor );
//	virtual UINT				OgreColor_CEGUIColor_Limit_Lumination( UINT OgreColor,FLOAT fLumination);
public:
	//得到图标管理器
	CUIIconsManager*			GetIconManager(VOID) { return m_pIconsMng; }
	
	//-----------------------------------------------------------------------
	//ActionButton相关
public:
	//捕获拖动开始(ActionButton调用)
	bool						handleActionDragDropStarted(const CEGUI::EventArgs& e);
	//鼠标进入，显示SuperTooltip
	bool						handleActionButtonMouseEnter(const CEGUI::EventArgs& e);
	//鼠标离开，隐藏SuperTooltip
	bool						handleActionButtonMouseLeave(const CEGUI::EventArgs& e);
	//ActionButton的父窗口关闭
	bool						handleActionButtonParentHidden(const CEGUI::EventArgs& e);

	//-----------------------------------------------------------------------
	//ActionButton相关
public:
	//MeshWindow开始显示
	bool						handleMeshWindowShown(const CEGUI::EventArgs& e);
	//MeshWindow隐藏
	bool						handleMeshWindowHiden(const CEGUI::EventArgs& e);


	//-----------------------------------------------------------------------
	//超连接相关
public:
	//点击超链接后
	bool						handleHyperLinkActive(const CEGUI::EventArgs &e);
	bool						handleHyperLinkLeftActive(const CEGUI::EventArgs &e);
	bool						handleHyperLinkRightActive(const CEGUI::EventArgs &e);
	bool						handleHyperLinkInactive(const CEGUI::EventArgs &e);

public:
	//过滤字符串中的非法敏感字符 2006-3-22
	BOOL		CheckStringFilter(const STRING& strIn);
	//过滤截断字符的<ERROR>
	BOOL		CheckStringCode(const STRING& strIn, STRING& strOut);
	//完全匹配过滤
	virtual BOOL		CheckStringFullCompare(const STRING& strIn, const STRING& strFilterType = "all", BOOL bUseAllTable = TRUE);
	
protected:
	//拖动开始
	VOID		OnDragBeging(CEGUI::FalagardActionButton* pBtnDraging);
	//拖动结束，pBtnTarget为目标Btn如果为空，则表示拖动到空白
	VOID		OnDragEnd(CEGUI::Window* pWindowTarget);

public:
	virtual VOID Initial(VOID*);				// 初始化
	virtual VOID Release(VOID);					// 释放
	virtual VOID Tick(VOID);

	
protected:
	Ogre::Root*					m_pOgreRoot;
	CEGUI::System*				m_pCEGUISystem;
	CEGUI::OgreCEGUIRenderer*	m_pCEGUIRender;

	//脚本解析
	CEGUI::CGameUIScript*		m_pCEGUIScript;
	//窗口管理器
	CUIWindowMng*				m_pWindowMng;
	//图标管理器
	CUIIconsManager*			m_pIconsMng;
	//文本资源管理器
	CUIStringSystem*			m_pStringSystem;
	//角色信息板管理器
	CUICreatureBoardSystem*		m_pCreatureBoardSystem;

	//拖动中
	//CEGUI::FalagardActionButton*		m_pBtnInDragging;
	CEGUI::String						m_szNameBtnInDragging;
	bool								m_bIsDragEnd;
	HCURSOR								m_hDraggingCursor;
	bool								m_bActivePaoPao;

//	bool
//	HCURSOR
	
public:
	CUISystem(VOID);
	virtual ~CUISystem(VOID);
	static CUISystem*	GetMe(VOID) { return s_pMe; }

protected:
	static CUISystem* s_pMe;

	WX_DECLARE_DYNAMIC(CUISystem);
};
