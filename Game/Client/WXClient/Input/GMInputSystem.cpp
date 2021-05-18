#include "StdAfx.h"
#include "GMInputSystem.h"
#include "..\Global.h"
#include "..\Procedure\GameProcedure.h"
#include "TDEventSystem.h"
#include "TDEventDefine.h"
#include "..\WxRender\RenderSystem.h"
#include "TDException.h"
#include "TDTimeSystem.h"
#include "TDUISystem.h"

#define _IsKeyDown(exp)		(GetAsyncKeyState(exp) & 0xff00)
#define _IsKeyDowned(exp)	(GetAsyncKeyState(exp) & 0x00ff)

WX_IMPLEMENT_DYNAMIC(CInputSystem, GETCLASS(tInputSystem));
CInputSystem* CInputSystem::s_pMe = NULL;
CInputSystem::CInputSystem()
{
	s_pMe = this;

	m_pDI = NULL;
	m_lpDIDKeyboard = NULL;
	m_hWnd = NULL;
	m_InputCapture = ICS_NONE;
	m_bCatchKeyDown = FALSE;
	m_dwMouseFlag = 0;
	m_dwMouseFlagOld = 0;
	m_dwLBPutdownTime = 0;

	m_bMouseInClient = TRUE;

	m_bFirstActive = TRUE;

	ZeroMemory(m_byCurKeys, sizeof(BYTE)*NUMDIKEYS);
	ZeroMemory(m_byOldKeys, sizeof(BYTE)*NUMDIKEYS);
	ZeroMemory(m_bKeyPresses, sizeof(BOOL)*NUMDIKEYS);
	ZeroMemory(m_bKeyPresseds, sizeof(BOOL)*NUMDIKEYS);

}

CInputSystem::~CInputSystem()
{
	Release();
}

VOID CInputSystem::Initial(VOID*)
{
	HRESULT hr;

	m_hWnd = g_hMainWnd;

    // Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&m_pDI, NULL ) ) )
	{
		TDThrow(_T("Create a DInput object error:%08X"), hr);
        return;
	}

	// Create the keyboard device
	hr = m_pDI->CreateDevice(GUID_SysKeyboard, &m_lpDIDKeyboard, NULL);
	if(hr == DI_OK)
	{
		m_lpDIDKeyboard->SetDataFormat(&c_dfDIKeyboard);
		m_lpDIDKeyboard->SetCooperativeLevel(g_hMainWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		AcquireKeyboard();
	}
}

VOID CInputSystem::AcquireKeyboard(VOID)
{
	if (m_lpDIDKeyboard)
	{
		HRESULT rval = m_lpDIDKeyboard->Acquire();
		if (rval == DI_OK || rval == S_FALSE)
		{
			KeyboardFlushData();
			return;
		}
	}	
}

VOID CInputSystem::KeyboardFlushData(VOID)
{
	ZeroMemory(m_byCurKeys, sizeof(BYTE)*NUMDIKEYS);
	ZeroMemory(m_byOldKeys, sizeof(BYTE)*NUMDIKEYS);
}

VOID CInputSystem::UnacquireKeyboard(VOID)
{
	KeyboardFlushData();
	if (m_lpDIDKeyboard) m_lpDIDKeyboard->Unacquire();
}

VOID CInputSystem::MouseSetPos(INT nX, INT nY)
{
	POINT ptClient;
	ptClient.x = nX;
	ptClient.y = nY;

	::ClientToScreen(g_hMainWnd, &ptClient);

	::SetCursorPos(ptClient.x, ptClient.y);

	m_ptMouse.x = m_ptMouseNow.x = nX;
	m_ptMouse.y = m_ptMouseNow.y = nY;
}

BOOL CInputSystem::MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	//滚轮消息
	case WM_MOUSEWHEEL:
		{
			if(GET_WHEEL_DELTA_WPARAM(wParam)>0)
			{
				m_dwMouseFlag |= MOUSE_WHEELUP;
			}
			else
			{
				m_dwMouseFlag |= MOUSE_WHEELDOWN;
			}
			return TRUE;
		}
		break;
	//键盘按下
	case WM_KEYDOWN:
		{
			//用来处理重复的键盘按下
			m_Buf_WM_KEYDOWN = std::make_pair(wParam, lParam);
			m_bCatchKeyDown = true;

			return FALSE;
		}
		break;

	case WM_KEYUP:
		{
			// 截屏操作要特殊处理 [4/24/2006]
			if(wParam == VK_SNAPSHOT)
			{
				if(CGameProcedure::s_pGfxSystem)
				{
					CHAR szFileName[MAX_PATH] = {0};
					if(CGameProcedure::s_pGfxSystem->PrintScreen(szFileName, MAX_PATH))
					{
						CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE, szFileName);
					}
				}

				return FALSE;
			}
		}
		break;
	//鼠标
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		{
			m_ptMouseNow.x = GET_X_LPARAM(lParam); 
			m_ptMouseNow.y = GET_Y_LPARAM(lParam); 
			return FALSE;
		}
		break;
	}

	return FALSE;
}

VOID CInputSystem::Tick(VOID)
{
	//如果主窗口没有激活,返回
	HWND hWndActive = ::GetActiveWindow();
	if(hWndActive != m_hWnd) return;

	//----------------------------------------
	//获取输入设备状态,同时将输入事件保存到输入事件队列中
	
	//清空事件队列,正常情况应该到这里时已经是空的
	if(!m_vEventQueue.empty())
		m_vEventQueue.clear();

	//键盘
	KeybordTick();
	//鼠标
	MouseTick();

	//激活后的第一桢不处理输入
	if(m_bFirstActive)
	{
		m_bFirstActive = FALSE;
		return;
	}

	//----------------------------------------
	//调用相应输入处理,
	switch(GetCapture())
	{
	case ICS_NONE:
		{
			if(CGameProcedure::s_pUISystem)
				CGameProcedure::s_pUISystem->InjectInput();
			if(GetCapture() != ICS_UI)
			{
				CGameProcedure::ProcessActiveInput();
			}
		}
		break;

	case ICS_UI:
		{
			if(CGameProcedure::s_pUISystem)
				CGameProcedure::s_pUISystem->InjectInput();
		}
		break;

	case ICS_GAME:
		{
			CGameProcedure::ProcessActiveInput();
		}
		break;
	}
}

VOID CInputSystem::KeybordTick(VOID)
{
	if(!m_lpDIDKeyboard) return;

	//保留旧状态
	memcpy(m_byOldKeys, m_byCurKeys, NUMDIKEYS);

	HRESULT hRet = m_lpDIDKeyboard->GetDeviceState(NUMDIKEYS, m_byCurKeys);
	if(hRet != DI_OK) 
		AcquireKeyboard();
	else
	{
		for(INT i=0; i<NUMDIKEYS; i++)
		{
			m_bKeyPresses[i]  = (!(m_byOldKeys[i]&0x80) && (m_byCurKeys[i]&0x80)); //该键刚被按下
			m_bKeyPresseds[i] = ((m_byOldKeys[i]&0x80) && !(m_byCurKeys[i]&0x80)); //该键刚被释放

			//保存键盘输入事件
			if(m_bKeyPresses[i])
			{
				AddEvent(EVENT_KEY_DOWN, i);
			}
			if(m_bKeyPresseds[i])
			{
				AddEvent(EVENT_KEY_UP, i);
			}
		}
	}

	//检查是否有自动发送的RepeatKeyDown
	if(m_bCatchKeyDown && !IsEventExist(EVENT_KEY_DOWN))
	{
		switch (m_Buf_WM_KEYDOWN.first) 
		{
		case VK_LEFT:	AddEvent(EVENT_KEY_DOWN, KC_LEFT);	break;
		case VK_UP:		AddEvent(EVENT_KEY_DOWN, KC_UP);	break;
		case VK_RIGHT:	AddEvent(EVENT_KEY_DOWN, KC_RIGHT); break;
		case VK_DOWN:	AddEvent(EVENT_KEY_DOWN, KC_DOWN);	break;
		case VK_BACK:	AddEvent(EVENT_KEY_DOWN, KC_BACK);	break;
		}
	}
	m_bCatchKeyDown = FALSE;
}

VOID CInputSystem::OnActiveApp(BOOL bActive)
{
	m_bFirstActive = TRUE;	
}

VOID CInputSystem::MouseTick(VOID)
{
	//--------------------------------------------------------
	//判断是否是Click的数据标准
	const INT MAX_TIME_DRAG			= 500;		//ms
	//开始自动行走
	const INT MOUSE_REPEAT_DOWN		= 1000;


	//--------------------------------------------------------
	//如果鼠标超出Client
	RECT rectClient;
	GetClientRect(g_hMainWnd, &rectClient);

	POINT ptTemp;
	::GetCursorPos(&ptTemp);
	::ScreenToClient(g_hMainWnd, &ptTemp);

	if(!::PtInRect(&rectClient, ptTemp) && ::GetCapture() != g_hMainWnd)
	{
		m_bMouseInClient = FALSE;
		return;
	}
	m_bMouseInClient = TRUE;

	//--------------------------------------------------------
	// 保存原鼠标位置
	m_ptMouseOld = m_ptMouse; 
	//取得鼠标位置
	m_ptMouse = m_ptMouseNow;

	{
		//保存原鼠标状态
		m_dwMouseFlagOld = m_dwMouseFlag;
		m_dwMouseFlag = 0;
		//当前时间
		UINT dwTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();

		//处理MouseMove
		BOOL bMouseMove = FALSE;

		if(m_ptMouseOld.x != m_ptMouseNow.x || m_ptMouseOld.y != m_ptMouseNow.y)
		{
			bMouseMove = TRUE;
			//保存鼠标 “移动” 事件
			AddEvent(EVENT_MOUSE_MOVE);
		}

		//察看按键
		if(_IsKeyDown(VK_LBUTTON)) m_dwMouseFlag |= MOUSE_LBDOWN;
		if(_IsKeyDown(VK_MBUTTON)) m_dwMouseFlag |= MOUSE_MBDOWN;
		if(_IsKeyDown(VK_RBUTTON)) m_dwMouseFlag |= MOUSE_RBDOWN;

		//左键按下的时间
		static UINT s_timeMouseLDownTimeBegin = 0;
		static UINT s_timeMouseMDownTimeBegin = 0;
		static UINT s_timeMouseRDownTimeBegin = 0;
		static BOOL s_bLRepeatDownMode = FALSE;

		//----------------------------------------------------------------
		//察看是否是Down
		if((m_dwMouseFlagOld & MOUSE_LBDOWN) == FALSE && (m_dwMouseFlag & MOUSE_LBDOWN)) 
		{
			//保存鼠标DOWN事件
			AddEvent(EVENT_MOUSE_LBDOWN);
			m_dwMouseFlag |= MOUSE_LBCLICK;

			m_ptMouseLDown = m_ptMouseNow;
			
			//记录时间
			s_timeMouseLDownTimeBegin = dwTimeNow;
			
			s_bLRepeatDownMode = FALSE;
			//CGameProcedure::s_pGfxSystem->PushDebugString("MouseDown");
		}
		if((m_dwMouseFlagOld & MOUSE_MBDOWN) == FALSE && (m_dwMouseFlag & MOUSE_MBDOWN)) 
		{		
			//保存鼠标DOWN事件
			AddEvent(EVENT_MOUSE_MBDOWN);
			m_dwMouseFlag |= MOUSE_MBCLICK;

			m_ptMouseMDown = m_ptMouseNow;

			//记录时间
			s_timeMouseMDownTimeBegin = dwTimeNow;
		}
		if((m_dwMouseFlagOld & MOUSE_RBDOWN) == FALSE && (m_dwMouseFlag & MOUSE_RBDOWN)) 
		{
			//保存鼠标DOWN事件
			AddEvent(EVENT_MOUSE_RBDOWN);
			m_dwMouseFlag |= MOUSE_RBCLICK;
	
			m_ptMouseRDown = m_ptMouseNow;
			//记录时间
			s_timeMouseRDownTimeBegin = dwTimeNow;
		}

		//----------------------------------------------------------------
		//察看是否是Clicked
		if((m_dwMouseFlagOld & MOUSE_LBDOWN) && (m_dwMouseFlag & MOUSE_LBDOWN) == FALSE) 
		{
			//计算时间
			UINT dwTime = CGameProcedure::s_pTimeSystem->CalSubTime(s_timeMouseLDownTimeBegin, dwTimeNow);
			//如果鼠标位置没有移动，而且时间相隔较短(500ms)
			if(	dwTime < MAX_TIME_DRAG && 
				abs(m_ptMouseLDown.x-m_ptMouseNow.x) <= 4 && 
				abs(m_ptMouseLDown.y-m_ptMouseNow.y) <= 4)
			{
				//保存鼠标UP事件
				m_dwMouseFlag |= MOUSE_LBCLICKED;

				AddEvent(EVENT_MOUSE_LBCLICK);
			}

			AddEvent(EVENT_MOUSE_LBUP);

			s_bLRepeatDownMode = FALSE;
		}
		if((m_dwMouseFlagOld & MOUSE_MBDOWN) && (m_dwMouseFlag & MOUSE_MBDOWN) == FALSE) 
		{
			//计算时间
			UINT dwTime = CGameProcedure::s_pTimeSystem->CalSubTime(s_timeMouseMDownTimeBegin, dwTimeNow);
			//如果鼠标位置没有移动，而且时间相隔较短(500ms)
			if( dwTime < MAX_TIME_DRAG)
			{
				//保存鼠标UP事件
				m_dwMouseFlag |= MOUSE_MBCLICKED;

				AddEvent(EVENT_MOUSE_MBCLICK);
			}

			AddEvent(EVENT_MOUSE_MBUP);
		}
		if((m_dwMouseFlagOld & MOUSE_RBDOWN) && (m_dwMouseFlag & MOUSE_RBDOWN) == FALSE) 
		{
			//计算时间
			UINT dwTime = CGameProcedure::s_pTimeSystem->CalSubTime(s_timeMouseRDownTimeBegin, dwTimeNow);
			//如果时间相隔较短(500ms)
			if( dwTime < MAX_TIME_DRAG &&
					abs(m_ptMouseRDown.x-m_ptMouseNow.x) <= 4 && 
					abs(m_ptMouseRDown.y-m_ptMouseNow.y) <= 4)
			{
				//保存鼠标UP事件
				m_dwMouseFlag |= MOUSE_RBCLICKED;
				AddEvent(EVENT_MOUSE_RBCLICK);
			}

			AddEvent(EVENT_MOUSE_RBUP);
		}

	
		//----------------------------------------------------------------
		//处理拖动

		//计算鼠标按下的时间
		m_dwLBPutdownTime = 0;
		if((m_dwMouseFlag&MOUSE_LBDOWN)&&(m_dwMouseFlagOld&MOUSE_LBDOWN))
		{
			m_dwLBPutdownTime = CGameProcedure::s_pTimeSystem->CalSubTime(s_timeMouseLDownTimeBegin, dwTimeNow);
		}

		//左键拖动事件
		if( (m_dwMouseFlag & MOUSE_LBDOWN) && (m_dwMouseFlagOld & MOUSE_LBDOWN) ) //连续两帧以上保持按住状态
		{
			if(bMouseMove)  
				AddEvent(EVENT_MOUSE_LDRAG_MOVE);

			//左键长按
			if(m_dwLBPutdownTime > MOUSE_REPEAT_DOWN)
			{
				AddEvent(EVENT_MOUSE_LBDOWNREPEAT);
				s_bLRepeatDownMode = true;
			}
		}
				
		//右键拖动事件
		if( (m_dwMouseFlag & MOUSE_RBDOWN) && (m_dwMouseFlagOld & MOUSE_RBDOWN) )
		{
			if(bMouseMove)	
				AddEvent(EVENT_MOUSE_RDRAG_MOVE);
		}

		//审查是否是Double Click		
		static UINT dwDblClk = ::GetDoubleClickTime();
		if(m_dwMouseFlag & MOUSE_LBCLICKED)
		{
			static UINT dwCLicked = 0;
			if(dwTimeNow < dwCLicked + dwDblClk)
			{
				//保存鼠标 “双击” 事件
				AddEvent(EVENT_MOUSE_LDBCLICK);

				m_dwMouseFlag |= MOUSE_LBDBLCLK;
			}
			dwCLicked = dwTimeNow;
			m_dwLBPutdownTime = 0;
		}
		if(m_dwMouseFlag & MOUSE_MBCLICKED)
		{
			static UINT dwCLicked = 0;
			if(dwTimeNow < dwCLicked + dwDblClk)
			{
				//保存鼠标 “双击” 事件
				AddEvent(EVENT_MOUSE_MDBCLICK);

				m_dwMouseFlag |= MOUSE_MBDBLCLK;
			}
			dwCLicked = dwTimeNow;
		}
		if(m_dwMouseFlag & MOUSE_RBCLICKED)
		{
			static UINT dwCLicked = 0;
			if(dwTimeNow < dwCLicked + dwDblClk)
			{
				//保存鼠标 “双击” 事件
				AddEvent(EVENT_MOUSE_RDBCLICK);

				m_dwMouseFlag |= MOUSE_RBDBLCLK;
			}
			dwCLicked = dwTimeNow;
		}
		
		//计算托拽盒
		if(m_dwMouseFlag & MOUSE_RBCLICK)
		{
			//右键被按下 托拽框左上
			m_rcRBDrag.left = m_ptMouse.x; 
			m_rcRBDrag.top = m_ptMouse.y;	
		}

		if(m_dwMouseFlagOld & MOUSE_RBCLICKED)
		{	
			//右键抬起
			m_rcRBDrag.right = m_ptMouse.x;
			m_rcRBDrag.bottom = m_ptMouse.y;
		}		

		//滚轮
		if(m_dwMouseFlagOld & MOUSE_WHEELUP)
			AddEvent(EVENT_MOUSE_WHEEL_UP);

		if(m_dwMouseFlagOld & MOUSE_WHEELDOWN)
			AddEvent(EVENT_MOUSE_WHEEL_DOWN);
	}
}

BOOL CInputSystem::IsEventExist(InputEventID id)
{
	InputEventQueue::iterator it;
	for(it=m_vEventQueue.begin(); it!=m_vEventQueue.end(); it++)
	{
		if(it->nEvent == id) return true;
	}

	return FALSE;
}

VOID CInputSystem::Release(VOID)
{
	if(m_lpDIDKeyboard) 
	{
		m_lpDIDKeyboard->Unacquire();
		m_lpDIDKeyboard->Release();
		m_lpDIDKeyboard = NULL;
	}

    if( m_pDI )		m_pDI->Release(); m_pDI = NULL; 
}

VOID CInputSystem::SetCapture(InputCapture cap)
{
	m_InputCapture = cap;
}

VOID CInputSystem::AddEvent(InputEventID nEvent,INT nIndex)
{
	InputEvent inputEvent;
	inputEvent.nEvent = nEvent;
	inputEvent.kIndex = (KeyCode) nIndex;
	inputEvent.bProcessed = FALSE;
	m_vEventQueue.push_back(inputEvent);
}

