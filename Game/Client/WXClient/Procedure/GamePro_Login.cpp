#include "StdAfx.h"
#include "Gamepro_login.h"
#include "GamePro_Enter.h"
#include "GamePro_CharSel.h"
#include "..\Global.h"
#include "..\Network\NetManager.h"
#include "..\World\WorldManager.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "CGConnect.h"
#include "CGEnterScene.h"
#include "TDVariable.h"
#include "TDInputSystem.h"
#include "TDEventDefine.h"
#include "TDUISystem.h"
#include "AxProfile.h"
#include "..\Event\GMEventSystem.h"
#include <string>


#include "CLConnect.h"					// 登录__服务器同步消息
#include "CLAskLogin.h"					// 登录__发送验证用户名密码
#include "CLAskCharList.h"				// 登录__发送得到角色消息
#include "..\WxRender\RenderSystem.h"


CGamePro_Login::CGamePro_Login()
{
	//m_Status = LOGIN_DEBUG_SETTING;
	m_Status = LOGIN_SELECT_SERVER;
	m_bReLogin = FALSE;
	m_pSoundSource = NULL;

	// 读取服务器配置信息.
	LoadLoginServerInfo();

	// 构造自动选择服务器查找表.
	BuildAutoSelTable();

	// 用户名
	m_strUserName = "";

	// 密码
	m_strPassword = "";

	for(int i = 0; i < PROVIDE_COUNT; i++ )
	{
		// 当前选择的区域
		m_iAutoAreaIndex[i] = -1;

		// 当前选择的login server 索引
		m_iAutoLoginServerIndex[i] = -1;
	}

	// 前一次找到网络状态空闲的索引.
	m_iPreFindIdle = -1;

	// 前一次找到网络状态正常的索引.
	m_iPreFindNor  = -1;

	// 前一次找到网络状态拥挤的索引.
	m_iPreFindBusy = -1;

	// 前一次找到网络状态爆满的索引.
	m_iPreFindFull = -1;

}

CGamePro_Login::~CGamePro_Login()
{
}

VOID CGamePro_Login::FirstLogin(VOID)
{
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Mode", (INT)LOGIN_FIRST_LOGIN);
}

VOID CGamePro_Login::LoginForChangeScene(VOID)
{
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Mode", (INT)LOGIN_CHANGE_SCENE);
}

VOID CGamePro_Login::Init(VOID)
{
	//fujia 2008.1.16
	/*if (CGameProcedure::s_pVariableSystem->GetAs_Int("View_FullScreen"))
	{
		s_pEventSystem->PushEvent(GE_VARIABLE_CHANGED, "View_FullScreen", "1");
	}
	else
	{
		const STRING& strVariable = CGameProcedure::s_pVariableSystem->GetAs_String("View_Resoution");
		fVector2 fResoution = s_pVariableSystem->GetAs_Vector2("View_Resoution");

		s_pEventSystem->PushEvent(GE_VARIABLE_CHANGED,"View_Resoution", strVariable.c_str());
		s_pEventSystem->PushEvent(GE_CHAT_ADJUST_MOVE_CTL, (INT)fResoution.x, (INT)fResoution.y);
	}*/
	
	// 禁止改变窗口大小。
	LONG lStyle = ::GetWindowLong(g_hMainWnd, GWL_STYLE);
	lStyle = lStyle & ~WS_THICKFRAME ;
	lStyle = lStyle & ~WS_MAXIMIZEBOX ;
	::SetWindowLong(g_hMainWnd, GWL_STYLE, lStyle);//
	//Rresh title
	::PostMessage(g_hMainWnd, WM_NCACTIVATE, TRUE, 0);

	//--- for debug
	if(CGameProcedure::s_pVariableSystem->GetAs_Int("GameServer_ConnectDirect") == 1) return;
	//--- for debug

	//打开登录界面
	if(LOGIN_SELECT_SERVER == m_Status)
	{
		s_pEventSystem->PushEvent(GE_GAMELOGIN_OPEN_SELECT_SERVER);
	}

	// 选择旧的服务器
	SelectOldServer();
}

// 在界面上选择服务器
VOID CGamePro_Login::SelectOldServer()
{
	int iPreCurSelArea		  = CGameProcedure::s_pVariableSystem->GetAs_Int("Login_Area");
    int iPreCurSelLoginServer = CGameProcedure::s_pVariableSystem->GetAs_Int("Login_Server");

	if((iPreCurSelArea < m_iAreaCount)&&(iPreCurSelArea >= 0))
	{
		int iLoginServerCount = GetAreaLoginServerCount(iPreCurSelArea);

		if((iPreCurSelLoginServer < iLoginServerCount)&&(iPreCurSelLoginServer >= 0))
		{
			// 选择前一次选择的服务器.
			s_pEventSystem->PushEvent(GE_GAMELOGIN_SELECT_AREA,			iPreCurSelArea);		// 选择区域
			s_pEventSystem->PushEvent(GE_GAMELOGIN_SELECT_LOGINSERVER,	iPreCurSelLoginServer);	// 选择Login Server
			return;
		}
	}
}

VOID CGamePro_Login::Tick(VOID)
{
	CGameProcedure::Tick();

	switch(m_Status)
	{
	case LOGIN_DEBUG_SETTING:
		{
			if(!CGameProcedure::s_pUISystem)
			{
				SetStatus(CGamePro_Login::LOGIN_DISCONNECT);
			}
			else
			{
				//DO NOTING,WAIT UI...
			}
		}
		break;

	case LOGIN_SELECT_SERVER:// 选择服务器状态
		{
			//--- for debug
			if(CGameProcedure::s_pVariableSystem->GetAs_Int("GameServer_ConnectDirect") == 1)
			{
				//直接切换到Change-Server流程
				CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcChangeScene);
				return;
			}
			//--- for debug
			break;
		}
	case LOGIN_DISCONNECT:
		{
			s_pGfxSystem->PushDebugString("Connect to login server %s:%d...", m_szLoginServerAddr, m_nLoginServerPort);

			//开始登录
			SetStatus(LOGIN_CONNECTING);
			CNetManager::GetMe()->ConnectToServer(m_szLoginServerAddr, m_nLoginServerPort);

		}
		break;

	case LOGIN_CONNECTING:

		break;
	
	//连接成功
	case LOGIN_CONNECTED_OK:
		{

			// 设置正在验证密码
			// SetStatus(LOGIN_ACCOUNT_REQUESTING);

		}
		break;

	//连接失败
	case LOGIN_CONNECT_FAILED:

		CNetManager::GetMe()->Close();
		SetStatus(LOGIN_SELECT_SERVER);
		break;

	
	// 正在验证用户名和密码.
	case LOGIN_ACCOUNT_REQUESTING:
		{

			// 判断是否超时, 超时就提示错误信息.
			break;
		}
	case LOGIN_ACCOUNT_BEGIN_REQUESTING:
		{
			break;
		}
	//登录信息验证成功
	case LOGIN_ACCOUNT_OK:
		{
			// 保存选择的服务器
			CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Area",   CGameProcedure::s_pProcLogIn->m_iCurSelAreaId, FALSE);
			CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Server", CGameProcedure::s_pProcLogIn->m_iCurSelLoginServerId, FALSE);

			if(m_bReLogin)
			{
				//直接进入场景
				CGameProcedure::s_pProcEnter->SetStatus(CGamePro_Enter::ENTERSCENE_READY);
				CGameProcedure::s_pProcEnter->SetEnterType(ENTER_TYPE_FIRST);
				CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcEnter);
			}
			else
			{
				// 设置登录状态为首次登录（以区分切换场景的登录状态）
				CGameProcedure::s_pProcLogIn->FirstLogin();

				//转入人物选择循环
				CGameProcedure::SetActiveProc((CGameProcedure*)s_pProcCharSel);
								
			}
		}
		break;
	default:
		break;
	}
}

VOID CGamePro_Login::Render(VOID)
{
	if(s_pGfxSystem) s_pGfxSystem->RenderFrame();
}

VOID CGamePro_Login::Release(VOID)
{
}

VOID CGamePro_Login::ProcessInput(VOID)
{
	//分析事件队列
	InputEventQueue& inputQueue = s_pInputSystem->GetInputQueue();
	for(INT i=0; i<(INT)inputQueue.size(); i++)
	{
		InputEvent& event = inputQueue[i];
		if(event.bProcessed) continue;
		if(event.nEvent != EVENT_KEY_UP) continue;
		switch(event.kIndex)
		{
		case KC_GRAVE:
			CGameProcedure::s_pEventSystem->PushEvent(GE_TOGLE_CONSOLE);
			event.bProcessed = true;
			break;

		case KC_D:
			{
				if(!(s_pInputSystem->IsKeyDown(KC_LMENU))) 
					break;

				if(s_pUISystem)
					s_pUISystem->Debug_SaveFontTexture();
			}
			break;

		default:
			break;
		}
	}
}



//------------------------------------------------------------------------------------------------------------
//
// 从配置文件得到, login server 的ip地址和端口号
//
//

// 设置端口号.
VOID  CGamePro_Login::SetPort(INT iPort)
{
	m_nLoginServerPort = iPort;
}

// 设置ip地址
VOID  CGamePro_Login::SetIpAddr(LPCTSTR szServerAddr)
{
	memcpy(m_szLoginServerAddr, szServerAddr, sizeof(m_szLoginServerAddr));
}


// 读取login server信息
//
// 返回值: 
//			0 -- 读取成功
//			1 -- 失败找不到配置文件
//          2 -- 其他错误    
int  CGamePro_Login::LoadLoginServerInfo(std::string strLoginServerInfoFile)
{
	m_pAreaInfo.resize(0);
	char szAppName[100];
	char szName[128];
	char szIP[32];
	DWORD dwMerchant;
	DWORD dwPort;
	DWORD dwBelong;
	DWORD dwStat;
	for( int i = 1; i < 256; i ++ )
	{
		sprintf( szAppName, "REGION%d", i );
		if( ::GetPrivateProfileString(szAppName, "NAME", "", szName, 120, strLoginServerInfoFile.c_str()) == 0 ) continue;
		dwMerchant = (DWORD)::GetPrivateProfileInt(szAppName, "MERCHANT", 0XFFFFFFFF, strLoginServerInfoFile.c_str());
		if( dwMerchant == 0XFFFFFFFF ) continue;
		AREA_INFO info;
		info.szAreaName = szName;
		info.iProvide = (NET_PROVIDE)dwMerchant;
		m_pAreaInfo.push_back( info );
	}

	m_iAreaCount = (int)m_pAreaInfo.size();
	for( int i = 1; i < 1024; i ++ )
	{
		sprintf( szAppName, "LOGIN%d", i );
		if( ::GetPrivateProfileString(szAppName, "NAME", "", szName, 120, strLoginServerInfoFile.c_str()) == 0 ) continue;
		if( ::GetPrivateProfileString(szAppName, "IP", "", szIP, 30, strLoginServerInfoFile.c_str()) == 0 ) continue;
		dwPort = (DWORD)::GetPrivateProfileInt(szAppName, "PORT", 0XFFFFFFFF, strLoginServerInfoFile.c_str());
		if( dwPort == 0XFFFFFFFF ) continue;
		dwStat = (DWORD)::GetPrivateProfileInt(szAppName, "STATE", 0XFFFFFFFF, strLoginServerInfoFile.c_str());
		if( dwStat == 0XFFFFFFFF ) continue;
		dwBelong = (DWORD)::GetPrivateProfileInt(szAppName, "BELONG", 0XFFFFFFFF, strLoginServerInfoFile.c_str());
		if( dwBelong == 0XFFFFFFFF ) continue;

		if( dwBelong == 0 || dwBelong > (DWORD)m_iAreaCount ) continue;
		AREA_INFO& info = m_pAreaInfo[dwBelong-1];

		LOGINSERVER_INOF li;
		li.szLoginServerName = szName;
		li.szIp = szIP;
		li.iPort = dwPort;
		li.iLoginServerStatus = (LOGIN_SERVER_STATUS)dwStat;
		info.LoginInfo.push_back( li );
	}

	return LOAD_LOGIN_SERVER_SUCCESS;
}

// 初始化区域信息
void CGamePro_Login::InitLoginAreaInfo()
{
	m_pAreaInfo.resize( 0 );
	m_iAreaCount = 0;
}


// 读取区域信息.
/*int CGamePro_Login::ReadAreaInfo(std::string& strLoginServer, int iAreaIndex)
{
	if(iAreaIndex >= m_iAreaCount)
	{
		return LOAD_LOGIN_SERVER_OTHER_ERR;
	}

	char bufInfo[512];
	char bufTempInfo1[128];
	char bufTempInfo2[128];
	char bufTempInfo3[128];


	while(!feof(pFile))
	{
		
		// 得到区域的名字.
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_OTHER_ERR;
		}
		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo3));
		::sscanf(bufInfo, "%s %s %s", bufTempInfo1, bufTempInfo2, bufTempInfo3);
		if(strcmp("区域名字", bufTempInfo1))
		{
			// 如果没有得到正确的区域, 一直读取错误的区域.
			continue;
		}

		// 保存区域的名字.
		strncpy(m_pAreaInfo[iAreaIndex].szAreaName, bufTempInfo3, sizeof(m_pAreaInfo[iAreaIndex].szAreaName));

		// 读取供应商.
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
		}
		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
		::sscanf(bufInfo, "%s %s %d", bufTempInfo1, bufTempInfo2, &(m_pAreaInfo[iAreaIndex].iProvide));


		// 读取login server 的个数.
		::ZeroMemory(bufInfo, sizeof(bufInfo));
		if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
		{
			return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
		}
		::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
		::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
		::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
		::sscanf(bufInfo, "%s %s %d", bufTempInfo1, bufTempInfo2, &(m_pAreaInfo[iAreaIndex].iLoginServerCount));

		int iReadRes = 0;
		for(int i = 0; i < m_pAreaInfo[iAreaIndex].iLoginServerCount; i++)	
		{
			iReadRes = ReadLoginInfo(pFile, iAreaIndex, i);
			if(iReadRes)
			{
				return iReadRes;
			}
		}

		// 读完一个区域信息就返回
		return LOAD_LOGIN_SERVER_SUCCESS;

	}
	
	// 没有读取完整区域的名字.
	return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
}*/


// 读取login server信息.
/*int CGamePro_Login::ReadLoginInfo(FILE* pFile, int iAreaIndex, int iLoginServerIndex)
{
	
	//if(NULL == pFile)
	//{
	//	return LOAD_LOGIN_SERVER_FILE_NOT_FINDED;
	//}

	//char bufInfo[512];
	//char bufTempInfo1[128];
	//char bufTempInfo2[128];
	//char bufTempInfo3[128];


	//while(!feof(pFile))
	//{
	//	//-------------------------------------------------------
	//	// 得到login server 的名字.
	//	::ZeroMemory(bufInfo, sizeof(bufInfo));
	//	if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
	//	{
	//		return LOAD_LOGIN_SERVER_OTHER_ERR;
	//	}

	//	::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
	//	::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
	//	::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
	//	::sscanf(bufInfo, "%s %s %s", bufTempInfo1, bufTempInfo2, bufTempInfo3);
	//	if(strcmp("LoginServer名字", bufTempInfo1))
	//	{
	//		// 如果没有得到login server, 一直读取错误的行.
	//		continue;
	//	}

	//	// 保存login server 的名字.
	//	//strncpy(
	//	//		m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].szLoginServerName, 
	//	//		bufTempInfo3, 
	//	//		sizeof(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].szLoginServerName)
	//	//		);

	//	//--------------------------------------------------------
	//	// 读取状态
	//	::ZeroMemory(bufInfo, sizeof(bufInfo));
	//	if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
	//	{
	//		return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
	//	}
	//	::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
	//	::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
	//	::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
	//	::sscanf(
	//				bufInfo, 
	//				"%s %s %d", 
	//				bufTempInfo1, 
	//				bufTempInfo2, 
	//				&(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].iLoginServerStatus)
	//				);


	//	//--------------------------------------------------------
	//	// 读取ip地址
	//	::ZeroMemory(bufInfo, sizeof(bufInfo));
	//	if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
	//	{
	//		return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
	//	}
	//	::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
	//	::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
	//	::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
	//	::sscanf(
	//				bufInfo, 
	//				"%s %s %s", 
	//				bufTempInfo1, 
	//				bufTempInfo2, 
	//				m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].szIp
	//				);


	//	//---------------------------------------------------------
	//	// 读取端口号
	//	::ZeroMemory(bufInfo, sizeof(bufInfo));
	//	if(NULL == ::fgets(bufInfo, sizeof(bufInfo), pFile))
	//	{
	//		return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;
	//	}
	//	::ZeroMemory(bufTempInfo1, sizeof(bufTempInfo1));
	//	::ZeroMemory(bufTempInfo2, sizeof(bufTempInfo2));
	//	::ZeroMemory(bufTempInfo3, sizeof(bufTempInfo3));
	//	//::sscanf(
	//	//			bufInfo, 
	//	//			"%s %s %d", 
	//	//			bufTempInfo1, 
	//	//			bufTempInfo2, 
	//	//			&(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].iPort)
	//	//			);

	//	// 读完一个login server 返回.
	//	return LOAD_LOGIN_SERVER_SUCCESS;

	//}
	
	// 没有读取完整区域的名字.
	return LOAD_LOGIN_SERVER_AREA_NOT_FINDED;//

}*/


//-----------------------------------------------------------------------------------------------------------------------------
//
// 逻辑数据操作
//
//

// 得到区域的个数
int CGamePro_Login::GetAreaCount()
{
	return m_iAreaCount;
}

// 得到区域名字
const TCHAR* CGamePro_Login::GetAreaName(int iAreaIndex)
{
	if( iAreaIndex < 0 || iAreaIndex >= m_iAreaCount )
		return _T("");

	return m_pAreaInfo[iAreaIndex].szAreaName.c_str();
}

// 得到区域信息
PAREA_INFO CGamePro_Login::GetAreaInfo(int iAreaIndex)
{
	if( iAreaIndex < 0 || iAreaIndex >= m_iAreaCount )
		return NULL;

	return &m_pAreaInfo[iAreaIndex];
}

// 通过网络供应商选择一个login server
int CGamePro_Login::AutoSelLoginServer(int iProvide)
{
	PPROVIDE_INFO pProvideInfo = NULL;
	int iAreaIndex        = -1;
	int iLoginServerIndex = -1;

	switch(iProvide)
	{
	case 0:// 电信供应商
		{
			pProvideInfo = &m_Provide0;
			break;
		}
	case 1:// 网通供应商
		{
			pProvideInfo = &m_Provide1;
			break;
		}
	case 2:// 其他供应商
		{
			pProvideInfo = &m_Provide2;
			break;
		}
	default:
		{
			break;
		}
	}

	int iLoginServerCount = (int)pProvideInfo->IdleLoginServerVector.size();
	// 查找空闲的服务器
	if(iLoginServerCount)
	{
		if(m_iPreFindIdle + 1 <  iLoginServerCount)
		{
			m_iPreFindIdle++;
			
		}
		else
		{
			m_iPreFindIdle = 0;
		}

		iAreaIndex        = pProvideInfo->IdleLoginServerVector[m_iPreFindIdle].iAreaIndex;
		iLoginServerIndex = pProvideInfo->IdleLoginServerVector[m_iPreFindIdle].iLoginServerIndex;
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SELECT_LOGIN_SERVER, iAreaIndex, iLoginServerIndex);//
		return 0;
	}

	iLoginServerCount = (int)pProvideInfo->NorLoginServerVector.size();
	// 查找正常的服务器
	if(iLoginServerCount)
	{
		if(m_iPreFindNor + 1 <  iLoginServerCount)
		{
			m_iPreFindNor++;
		}
		else
		{
			m_iPreFindNor = 0;
		}

		iAreaIndex        = pProvideInfo->NorLoginServerVector[m_iPreFindNor].iAreaIndex;
		iLoginServerIndex = pProvideInfo->NorLoginServerVector[m_iPreFindNor].iLoginServerIndex;
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SELECT_LOGIN_SERVER, iAreaIndex, iLoginServerIndex);//
		return 0;
	}

	iLoginServerCount = (int)pProvideInfo->BusyLoginServerVector.size();
	// 查找繁忙的服务器
	if(iLoginServerCount)
	{
		if(m_iPreFindBusy + 1 <  iLoginServerCount)
		{
			m_iPreFindBusy++;
		}
		else
		{
			m_iPreFindBusy = 0;
		}

		iAreaIndex        = pProvideInfo->BusyLoginServerVector[m_iPreFindBusy].iAreaIndex;
		iLoginServerIndex = pProvideInfo->BusyLoginServerVector[m_iPreFindBusy].iLoginServerIndex;
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SELECT_LOGIN_SERVER, iAreaIndex, iLoginServerIndex);//
		return 0;
	}

	iLoginServerCount = (int)pProvideInfo->FullLoginServerVector.size();
	// 查找爆满的服务器
	if(iLoginServerCount)
	{
		if(m_iPreFindFull + 1 <  iLoginServerCount)
		{
			m_iPreFindFull++;
		}
		else
		{
			m_iPreFindFull = 0;
		}

		iAreaIndex        = pProvideInfo->FullLoginServerVector[m_iPreFindFull].iAreaIndex;
		iLoginServerIndex = pProvideInfo->FullLoginServerVector[m_iPreFindFull].iLoginServerIndex;
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SELECT_LOGIN_SERVER, iAreaIndex, iLoginServerIndex);//
		return 0;
	}


	return 0;
}

// 构造自动选择login server需要的数据表
int CGamePro_Login::BuildAutoSelTable()
{
	//LOGIN_SERVER_POS LoginServerPos;
	//PPROVIDE_INFO			 pProvideInfo     = NULL;
	//LOGIN_SERVER_POS_VECTOR* pServerPosVector = NULL;
	//int iLoginServerCount = 0;
	//for(int i = 0; i < m_iAreaCount; i++)
	//{
		//switch(m_pAreaInfo[i].iProvide)
		//{
		//case 0:// 电信供应商
		//	{
		//		pProvideInfo = &m_Provide0;
		//		break;
		//	}
		//case 1:// 网通供应商
		//	{
		//		pProvideInfo = &m_Provide1;
		//		break;
		//	}
		//case 2:// 其他供应商
		//	{
		//		pProvideInfo = &m_Provide2;
		//		break;
		//	}
		//}

		//iLoginServerCount = m_pAreaInfo[i].iLoginServerCount;
		//for(int j = 0; j < iLoginServerCount; j++)
		//{
		//	LoginServerPos.iAreaIndex        = i;
		//	LoginServerPos.iLoginServerIndex = j;
		//
		//	switch(m_pAreaInfo[i].LoginInfo[j].iLoginServerStatus)
		//	{
		//	case LOGIN_SERVER_FULL:
		//		{
		//			pServerPosVector = &(pProvideInfo->FullLoginServerVector);
		//			break;
		//		}
		//	case LOGIN_SERVER_BUSY:
		//		{
		//			pServerPosVector = &(pProvideInfo->BusyLoginServerVector);
		//			break;
		//		}
		//	case LOGIN_SERVER_NORMAL:
		//		{
		//			pServerPosVector = &(pProvideInfo->NorLoginServerVector);
		//			break;
		//		}
		//	case LOGIN_SERVER_IDLE:
		//		{
		//			pServerPosVector = &(pProvideInfo->IdleLoginServerVector);
		//			break;
		//		}
		//	default:
		//		{

		//		}

		//	}// switch(m_pAreaInfo[i].LoginInfo[j].iLoginServerStatus)

		//	pServerPosVector->push_back(LoginServerPos);

		//}// for(int j = 0; j < iLoginServerCount; j++)

	//}// for(int i = 0; i < m_iAreaCount; i++)

	return 0;
}

// 得到区域中login server 的个数
int CGamePro_Login::GetAreaLoginServerCount(int iAreaIndex)
{
	if( iAreaIndex < 0 || iAreaIndex >= m_iAreaCount )
		return 0;

	return (int)(m_pAreaInfo[iAreaIndex].LoginInfo.size());
}

// 得到区域中login server 的个数
PLOGINSERVER_INOF CGamePro_Login::GetAreaLoginServerInfo(int iAreaIndex, int iLoginServerIndex)
{
	if( iAreaIndex < 0 || iAreaIndex >= m_iAreaCount )
		return NULL;

	if( iLoginServerIndex >= (int)m_pAreaInfo[iAreaIndex].LoginInfo.size() )
		return NULL;

	return &(m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex]);
}


// 连接到login server
int CGamePro_Login::ConnectToLoginServer(int iAreaIndex, int iLoginServerIndex)
{

	if( iAreaIndex < 0 || iAreaIndex >= m_iAreaCount )
		return 1;

	if( iLoginServerIndex >= (int)m_pAreaInfo[iAreaIndex].LoginInfo.size() )
		return 1;

	// 设置ip地址和端口号.
	SetIpAddr( m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].szIp.c_str() );
	SetPort( m_pAreaInfo[iAreaIndex].LoginInfo[iLoginServerIndex].iPort );

	// 设置当前的状态为非连接状态
	SetStatus(LOGIN_DISCONNECT);

	// 通知界面显示系统提示信息, 正在连接服务器.
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "正在连接到服务器.....");

	return 0;

}


// 记录选择的服务器login server
int CGamePro_Login::SelectLoginServer(int iAreaIndex, int iLoginServerIndex)
{

	// 记录选择的服务器
	m_iCurSelAreaId			= iAreaIndex;
	m_iCurSelLoginServerId  = iLoginServerIndex;

	OpenCountInputDlg();

	return 0;
}


// 连接到login server
int CGamePro_Login::ConnectToLoginServer()
{
	// 设置当前的状态为非连接状态
	SetStatus(LOGIN_DISCONNECT);

	// 通知界面显示系统提示信息, 正在连接服务器.
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "正在连接到服务器.....");
	return 0;
}


//-------------------------------------------------------------------------------------------------------------------------------
//
// 界面操作
//

// 打开帐号输入界面
int CGamePro_Login::OpenCountInputDlg()
{
	// 打开帐号输入界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_COUNT_INPUT);

	// 关闭系统界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// 关闭服务器选择界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_SERVER);

	// 保存选择的服务器
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Area",   CGameProcedure::s_pProcLogIn->m_iCurSelAreaId, FALSE);
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Server", CGameProcedure::s_pProcLogIn->m_iCurSelLoginServerId, FALSE);

	// 设置帐号输入界面
	//SetStatus(LOGIN_ACCOUNT_BEGIN_REQUESTING);
	return 0;
}

// 打开服务器选择界面
int CGamePro_Login::OpenSelectServerDlg()
{
	// 打开服务器选择界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_SELECT_SERVER);

	// 关闭系统界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// 关闭帐号输入界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_COUNT_INPUT);

	// 设置服务器选择界面
	SetStatus(LOGIN_SELECT_SERVER);

	// 断开网络连接
	CNetManager::GetMe()->Close();

	SelectOldServer();
	return 0;
}


// 打开人物选择界面
int CGamePro_Login::OpenSelectCharacter()
{

	return 0;
}

// 切换到人物选择界面
int CGamePro_Login::ChangeToCharacterSel()
{

	
	// 保存选择的服务器
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Area",   CGameProcedure::s_pProcLogIn->m_iCurSelAreaId, FALSE);
	CGameProcedure::s_pVariableSystem->SetAs_Int("Login_Server", CGameProcedure::s_pProcLogIn->m_iCurSelLoginServerId, FALSE);

	// 关闭系统界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// 关闭帐号输入界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_COUNT_INPUT);

	// 显示人物选择界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_SELECT_CHARACTOR);

	//切换到人物选择界面
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharSel);

	// 设置用户名
	CGameProcedure::s_pProcCharSel->SetUserName(m_strUserName.c_str());

	// 进入到人物选择界面， 清空ui模型。
	CGameProcedure::s_pProcCharSel->m_bClearUIModel = TRUE;

	// 发送得到角色信息消息.
	CLAskCharList msg;
	msg.SetAccount((CHAR*)m_strUserName.c_str());
	CNetManager::GetMe()->SendPacket(&msg);


	return 0;
}



//-------------------------------------------------------------------------------------------------------------------------------
//
// 网络命令消息操作
//

// 发送同步消息
int CGamePro_Login::SendClConnectMsg()
{
	CLConnect msg;

	// 发送网络连接消息
	CNetManager::GetMe()->SendPacket(&msg);
	return 0;
}

// 检查用户名和密码.
int CGamePro_Login::CheckAccount(CHAR* szUserName, CHAR* szPassword)
{
	if(NULL == szUserName)
	{
		// 通知界面显示系统提示信息, 用户名错误
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "输入用户名错误");
		return 0;
	}

	if(NULL == szPassword)
	{
		// 通知界面显示系统提示信息, 用户名错误
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "输入密码错误");
		return 0;
	}

	m_strUserName = szUserName;
	m_strPassword = szPassword;
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 验证用户名
	//
	int iPos = (int)m_strUserName.find('\'');
	if(-1 != iPos)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "用户名不能含有 \" \' \" ");
		return 0;
	}

    iPos = (int)m_strUserName.find('"');
	if(-1 != iPos)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "用户名不能含有 \"  \"  \" ");
		return 0;
	}

	iPos = (int)m_strUserName.find('\\');
	if(-1 != iPos)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "用户名不能含有 \" \\ \" ");
		return 0;
	}//


	//// 到非法字符串表中查找。
	//int iFindPos = -1;
	//char bufInfo[512];
	//for(int i = 0; i < (int)m_UnUseStrVector.size(); i++)
	//{
	//	iFindPos = (int)m_strUserName.find(m_UnUseStrVector[i]);
	//	if(-1 != iFindPos)
	//	{
	//		memset(bufInfo, 0, sizeof(bufInfo));
	//		sprintf(bufInfo, "用户名包含非法符号%s", m_UnUseStrVector[i]);
	//		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, bufInfo);
	//		return 0;
	//	}

	//}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 验证密码
	//
	iPos = (int)m_strPassword.find('\'');
	if(-1 != iPos)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "密码不能含有 \" \' \" ");
		return 0;
	}

    iPos = (int)m_strPassword.find('"');
	if(-1 != iPos)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "密码不能含有 \"  \"  \" ");
		return 0;
	}

	iPos = (int)m_strPassword.find('\\');
	if(-1 != iPos)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "密码不能含有 \" \\ \" ");
		return 0;
	}//

	//// 到非法字符串表中查找
	//for(int j = 0; j < (int)m_UnUseStrVector.size(); j++)
	//{
	//	iFindPos = (int)m_strPassword.find(m_UnUseStrVector[j]);
	//	if(-1 != iFindPos)
	//	{
	//		memset(bufInfo, 0, sizeof(bufInfo));
	//		sprintf(bufInfo, "密码包含非法符号%s", m_UnUseStrVector[j]);
	//		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, bufInfo);
	//		return 0;
	//	}
	//}

	// 与服务器进行socket 连接.
	ConnectToLoginServer(m_iCurSelAreaId, m_iCurSelLoginServerId);
	return 0;
}

// 发送check msg 消息
int CGamePro_Login::SendCheckAccountMsg()
{
	// 发送验证密码消息
	CLAskLogin msg;
	msg.SetAccount((CHAR*)m_strUserName.c_str());
	msg.SetPassWord((CHAR*)m_strPassword.c_str());
	msg.SetVersion(1005); // MagicKey
	CNetManager::GetMe()->SendPacket(&msg);
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "正在验证密码....");

	s_pVariableSystem->SetVariable( "User_NAME", m_strUserName.c_str() );
	return 0;
}


// 
// 
//
//------------------------------------------------------------------------------------------------------------
