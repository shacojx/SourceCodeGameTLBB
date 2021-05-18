/****************************************\
*										*
* 			   输入管理器				*
*										*
*						 by jinchao		*
\****************************************/
#pragma once

#define DIRECTINPUT_VERSION         0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include "TDInputSystem.h"
#include <queue>

//输入管理器
class CInputManager : public tInputSystem
{
public:
	//节点初始化
	virtual VOID	Initial(VOID*);
	//逻辑轮循函数
	virtual VOID	Tick(VOID);
	//释放自己所所拥有的资源
	virtual VOID	Release(VOID);

public:
	//处理Windows消息
	virtual VOID			MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	//捕获状态
	virtual InputCapture	GetCapture(VOID) const	{ return m_InputCapture; }
	virtual	VOID			SetCapture(InputCapture cap);

public:
	 //某键是否出于按下状态
	virtual BOOL IsKeyDown(KeyCode kIndex)		{ return (m_byCurKeys[kIndex] & 0x80) !=0 ;	}
	//某键是否刚被按下
	virtual BOOL IsKeyPress(KeyCode kIndex)		{ return m_bKeyPresses[kIndex];				}
	//某键是否刚被抬起
	virtual BOOL IsKeyPressed(KeyCode kIndex)	{ return m_bKeyPresseds[kIndex];			}

public:
	virtual VOID	MouseSetPos(INT nX, INT nY);
	virtual POINT	MouseGetPos(VOID)				{ return m_ptMouse; }
	virtual POINT	MouseGetPosOld(VOID)			{ return m_ptMouseOld; }
	
	virtual RECT	MouseGetLBDragRect(VOID)		{ return m_rcLBDrag; }
	virtual RECT	MouseGetMBDragRect(VOID)		{ return m_rcMBDrag; }
	virtual RECT	MouseGetRBDragRect(VOID)		{ return m_rcRBDrag; }	

	virtual UINT	MouseGetFlag(VOID)				{ return m_dwMouseFlag; }
	virtual UINT	MouseGetFlagOld(VOID)			{ return m_dwMouseFlagOld; }

	virtual UINT	GetLPutdownTime(VOID)			{ return m_dwLBPutdownTime; }

public:
	//得到输入队列
	virtual InputEventQueue&	GetInputQueue(VOID) { return m_vEventQueue; }	
	//查询队列中是否有某事件存在
	virtual BOOL				IsEventExist(InputEventID id);

protected:
	//开始使用DINPUT键盘
	VOID			AcquireKeyboard(VOID);
	//释放DINPUT键盘
	VOID			UnacquireKeyboard(VOID);
	//清空键盘数据
	VOID			KeyboardFlushData(VOID);

	//键盘逻辑
	VOID			KeybordTick(VOID);
	//鼠标逻辑
	VOID			MouseTick(VOID);

	//加入事件
	VOID			AddEvent(InputEventID nEvent,INT nIndex = 0x00);

public:
	CInputManager();
	virtual ~CInputManager();
	static CInputManager* GetMe(VOID) { return s_pMe; }

protected:
	static CInputManager*		s_pMe;

	//所处理的窗口
	HWND						m_hWnd;
	//输入捕获
	InputCapture				m_InputCapture;

	//-------------------------------------------------------
	//键盘相关
	LPDIRECTINPUT8				m_pDI;
	LPDIRECTINPUTDEVICE8		m_lpDIDKeyboard;

	const static INT NUMDIKEYS	= 256;

	BYTE		m_byCurKeys[NUMDIKEYS];		// 当前键盘状态
	BYTE		m_byOldKeys[NUMDIKEYS];		// 上一桢键盘状态
	BOOL		m_bKeyPresses[NUMDIKEYS];	// 某键是否刚被按下
	BOOL		m_bKeyPresseds[NUMDIKEYS];	// 谋键是否刚被释放

	//WM_KEYDOWN保存区，用来处理键盘自动Repeat
	BOOL						m_bCatchKeyDown;
	std::pair< WPARAM, LPARAM > m_Buf_WM_KEYDOWN;

	//-------------------------------------------------------
	//鼠标键盘事件列表
	InputEventQueue		m_vEventQueue;

	//-------------------------------------------------------
	//鼠标相关
	POINT				m_ptMouse;
	POINT				m_ptMouseOld;

	UINT				m_dwMouseFlag;
	UINT				m_dwMouseFlagOld;

	RECT				m_rcLBDrag; // 拖动盒
	RECT				m_rcMBDrag; // 拖动盒
	RECT				m_rcRBDrag; // 拖动盒

	UINT				m_dwLBPutdownTime; // 鼠标左键按下的时间

private:
	POINT				m_ptMouseNow;

	POINT				m_ptMouseLDown;
	POINT				m_ptMouseMDown;
	POINT				m_ptMouseRDown;

protected:
	TD_DECLARE_DYNAMIC(CInputManager);
};