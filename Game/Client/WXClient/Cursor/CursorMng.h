#pragma once
#include "..\GameCommand.h"
#include "TDMath.h"
#include "TDCursorSystem.h"

class CObject;
class tActionItem;
class CCursorMng : public tCursorSystem
{
public:
	virtual  VOID		Initial(VOID *);
	
	//--------------------------------------------------
	//强制设置鼠标光标
	virtual VOID		SetCursor( ENUM_CURSOR_TYPE nType );
	virtual HCURSOR		GetCursor(VOID);
	//响应WM_SETCURSOR
	virtual VOID		OnSetCursor(VOID);
	//进入UI控制模式
	virtual VOID		EnterUICursor(HCURSOR hCursor) { m_hUICursor = hCursor; }
	virtual VOID		LeaveUICursor(VOID) { m_hUICursor = NULL; } 
	//显示/隐藏鼠标
	virtual VOID		ShowCursor(BOOL bShow);
	//取鼠标状态
	INT	GetCursor_State(VOID);

	//--------------------------------------------------
	//在鼠标上存储命令
	virtual VOID					MouseCommand_Set(BOOL bHoverInUI, CObject* pObj, const fVector3& fvPos, tActionItem* pActiveSkill);
	//得到命令
	virtual SCommand_Mouse&			MouseCommand_GetLeft(VOID) { return m_cmdCurrent_Left; }
	virtual SCommand_Mouse&			MouseCommand_GetRight(VOID) { return m_cmdCurrent_Right; }
	//激活鼠标命令
	virtual VOID					MouseCommand_Active(SCommand_Mouse& cmd);

	//清空
	virtual VOID					MouseCommand_Clear(VOID) { 	m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_NULL; }

public:
	CCursorMng(VOID);
	virtual ~CCursorMng(VOID);
	static CCursorMng* GetMe(VOID)	{ return s_pMe; }

protected:
	static CCursorMng*		s_pMe;

protected:
	BOOL		m_bShow;							// 是否显示光标
	INT			m_nCursorState;						// 光标状态
	HCURSOR		m_hCursor[ CURSOR_NUMBER ];			// 光标
	HCURSOR		m_hUICursor;						//UI控制光标

	//鼠标上储存的命令
	SCommand_Mouse	m_cmdCurrent_Left;				// 待触发指令_左键
	SCommand_Mouse	m_cmdCurrent_Right;				// 待触发指令_右键

	WX_DECLARE_DYNAMIC( CCursorMng );
};
