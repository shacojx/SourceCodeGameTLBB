#include "StdAfx.h"

#include "..\Network\NetManager.h"
#include "GamePro_CharSel.h"
#include "Gamepro_login.h"
#include "GamePro_Enter.h"
#include "GamePro_CharCreate.h"
#include ".\Gamepro_ChangeScene.h"
#include "CGEnterScene.h"
#include "..\Global.h"
//#include "..\Network\NetManager.h"


#include "..\Input\GMInputSystem.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Character\Obj_PlayerOther.h"
#include "..\GameCommand.h"
#include "..\Object\ObjectCommandDef.h"
#include "..\object\ObjectDef.h"
#include "..\object\character\Obj_PlayerOther.h"
#include "..\FakeObject\GMFakeObjSystem.h"
#include "..\object\Character\Obj_Character.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "..\WxRender\RenderSystem.h"
#include "TDGfxSystem.h"
#include "..\Event\GMEventSystem.h"
#include "CLAskDeleteChar.h"
#include "CLAskCharLogin.h"
#include "GameStruct.h"
//--------------------------------------------------------------------------------------------------------------------------
//
// 在界面上显示的模型.
//

// 模型计数.
UINT CModelShowInLoginUI::m_TeamNumCount = 0;

CModelShowInLoginUI::CModelShowInLoginUI()
{
	
	m_pAvatar = NULL;								// 用于UI显示的逻辑对象.
	memset(m_szBufModel, 0, sizeof(m_szBufModel));	// 模型名字.

	// 创建新的模型
	//CreateNewUIModel();

}

CModelShowInLoginUI::~CModelShowInLoginUI()
{
	DestroyUIModel();
}

// 删除ui模型
BOOL CModelShowInLoginUI::DestroyUIModel()
{
	if(m_pAvatar)
	{
		CFakeObjSystem::GetMe()->DestroyObject(m_szBufModel);
		m_pAvatar = NULL;
	}

	memset(m_szBufModel, 0, sizeof(m_szBufModel));	// 模型名字.
	return FALSE;
}

// 创建新的模型
BOOL	CModelShowInLoginUI::CreateNewUIModel()
{
	if(m_pAvatar)
	{
		DestroyUIModel();
	}
	
	static int iUiModelCount = 0;
	iUiModelCount++;
	m_TeamNumCount = ::timeGetTime();
	m_TeamNumCount += iUiModelCount;
	memset(m_szBufModel, 0, sizeof(m_szBufModel));
	_stprintf(m_szBufModel, "uiModel%d", m_TeamNumCount);
	
	//创建Avatar
	m_pAvatar  = (CObject_PlayerOther*)(CFakeObjSystem::GetMe()->NewFakeObject(_T("CObject_PlayerOther"), m_szBufModel, "Camera_Main"));
	m_pAvatar->SetFaceDir(0);
	m_pAvatar->GetCharacterData()->Set_RaceID(0);
	return FALSE;

}


// 设置模型信息
VOID CModelShowInLoginUI::SetUIModelInfo(HUMAN_EQUIP point, INT nID)
{
	if(m_pAvatar)
	{
		m_pAvatar->GetCharacterData()->Set_Equip(point, nID);
	}
}

// 得到ui模型名字
const TCHAR* CModelShowInLoginUI::GetUIModelName()
{
	
	return m_szBufModel;
	
}


//------------------------------------------------------------------------------------------------------------------------------




CGamePro_CharSel::CGamePro_CharSel()
{
	m_Status = CHARSEL_CHARSHOW;
	m_bIsLoadScene		= false;
	m_iCharacterCount	= 0;

	m_bClearUIModel = true;
}

CGamePro_CharSel::~CGamePro_CharSel()
{
	ClearUIModel();
}

VOID CGamePro_CharSel::Init(VOID)
{

	//return;
	//s_pGfxSystem->Camera_SetCurrent(tGfxSystem::CHAR_VIEW);
	//
	//if(!m_bIsLoadScene)
	//{
	//	s_pGfxSystem->Scene_Load(0, "CharacterSelTest.Scene");
	//	m_bIsLoadScene = true;
	//}

	//s_pGfxSystem->Camera_SetEyePos(fVector3(-100,    325, -1000));
	//s_pGfxSystem->Camera_SetLookAtPos(fVector3(-100, 320, 0));//

	if(m_bClearUIModel)
	{
		ClearUIModel();
	}

}

VOID CGamePro_CharSel::Tick(VOID)
{
	CGameProcedure::Tick();
	//CGameProcedure::ProcessActiveInput();

	switch(m_Status)
	{
	case CHARSEL_CHARSHOW:
		{
		
			break;
		}

	case CHARSEL_SELDONE:
		{
			break;
		}
	case CHARSEL_CONNNECT_GAME_SERVER_READY:// 与游戏服务器的连接准备好, 可以连接.
		{
			break;
		}
	}

}

VOID CGamePro_CharSel::Render(VOID)
{
	if(s_pGfxSystem) s_pGfxSystem->RenderFrame();
}

VOID CGamePro_CharSel::Release(VOID)
{
}

VOID CGamePro_CharSel::CloseRequest(VOID)
{

}

VOID CGamePro_CharSel::SetStatus(PLAYER_CHARSEL_STATUS status)
{
	m_Status = status;
}

VOID CGamePro_CharSel::ProcessInput(VOID)
{
	////-------------------------------------------------------
	////按键 'A'
	//if(s_pInputSystem->IsKeyPress(KC_A))
	//{
	//	m_Status = CHARSEL_SELDONE;
	//}

	////-------------------------------------------------------
	////按键 '1' 创建角色流程
	//if(s_pInputSystem->IsKeyPress(KC_1))
	//{
	//	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharCreate);
	//}

	////-------------------------------------------------------
	////按键 '2' 进入游戏流程.
	//if(s_pInputSystem->IsKeyPress(KC_2))
	//{
	//	m_bIsLoadScene = false;
	//	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcLogIn);
	//}//

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
			{
			}
			break;
		case KC_1:
			{
				CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharCreate);
			}
			break;
		case KC_2:
			{
				m_Status = CHARSEL_SELDONE;
				m_bIsLoadScene = false;
			}
		
		default:
			break;
		}
	}//

}


// 设置用户名字
void CGamePro_CharSel::SetUserName(const char* pUserName)
{
	if(pUserName)
	{
		m_strUserName = pUserName;
	}
}

// 切换到帐号输入界面
void CGamePro_CharSel::ChangeToAccountInput()
{
	// 关闭系统界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// 打开帐号输入界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_COUNT_INPUT);

	// 关闭人物选择界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);

	//切换到人物选择界面
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcLogIn);
	if(CGameProcedure::s_pProcLogIn)
	{
		CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_ACCOUNT_BEGIN_REQUESTING);
	}

	m_iCharacterCount = 0;
	// 断开网络连接
	CNetManager::GetMe()->Close();
	
}

	// 切换到创建人物界面
void CGamePro_CharSel::ChangeToCreateRole()
{
	// 打开人物创建界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_OPEN_CREATE_CHARACTOR);

	// 关闭系统界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);

	// 关闭人物选择界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);


	//切换到人物选择界面
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcCharCreate);

	if(CGameProcedure::s_pProcCharCreate)
	{
		CGameProcedure::s_pProcCharCreate->CallInit();
	}

	
}


// 添加一个人物
int CGamePro_CharSel::AddCharacter(const DB_CHAR_BASE_INFO& CharacterInfo)
{
	if(m_iCharacterCount < MAX_SHOW_IN_UI)
	{
		memcpy(m_CharacterInfo + m_iCharacterCount, &CharacterInfo, sizeof(DB_CHAR_BASE_INFO));
		m_Character[m_iCharacterCount].CreateNewUIModel();

		int iRaceId		= m_CharacterInfo[m_iCharacterCount].m_Sex;
		int iFaceMeshId = m_CharacterInfo[m_iCharacterCount].m_FaceModel;
		int iFaceHairId = m_CharacterInfo[m_iCharacterCount].m_HairModel;
		UINT iHairColor = m_CharacterInfo[m_iCharacterCount].m_HairColor;
		_CAMP_DATA TempCampData;
		TempCampData.m_nCampID = m_CharacterInfo[m_iCharacterCount].m_Camp;
		
		// 设置性别
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_RaceID(iRaceId);

		// 设置脸的模型
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_FaceMesh(iFaceMeshId);

		// 设置发型的模型.
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_HairMesh(iFaceHairId);

		//头发颜色
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_HairColor(iHairColor);

		//阵营
		m_Character[m_iCharacterCount].m_pAvatar->GetCharacterData()->Set_CampData( &TempCampData );

		const DB_CHAR_EQUIP_LIST& equipInfo = m_CharacterInfo[m_iCharacterCount].m_EquipList;
		// 设置装备.
		for(int iEquipPoint = HEQUIP_WEAPON; iEquipPoint < HEQUIP_NUMBER; iEquipPoint++)
		{
			m_Character[m_iCharacterCount].SetUIModelInfo((HUMAN_EQUIP)iEquipPoint, equipInfo.m_Equip[iEquipPoint]);
		}//

		m_iCharacterCount++;

		return 0;
	}
	else
	{
		return -1;
	}

}



// 得到当前角色的个数
int  CGamePro_CharSel::GetCurRoleCount()
{

	return m_iCharacterCount;
}

// 通过索引得到角色的信息
const DB_CHAR_BASE_INFO* CGamePro_CharSel::GetRoleInfo(int iIndex)
{

	if((iIndex < MAX_SHOW_IN_UI)&&(iIndex >= 0))
	{

		return (m_CharacterInfo + iIndex);
	}
	
	return NULL;
}


// 通过索引得到角色的UI 模型名字
const TCHAR* CGamePro_CharSel::GetRoleUIName(int iIndex)
{
	if((iIndex < MAX_SHOW_IN_UI)&&(iIndex >= 0))
	{

		return m_Character[iIndex].GetUIModelName();
	}
	
	return NULL;
}


// 删除一个角色
int CGamePro_CharSel::DelRole(int iRoleIndex)
{
	if((iRoleIndex < m_iCharacterCount)&&(iRoleIndex >= 0))
	{
		if(iRoleIndex < MAX_SHOW_IN_UI)
		{
			CLAskDeleteChar msg;

			msg.SetCharGuid(m_CharacterInfo[iRoleIndex].m_GUID);

			// 删除角色
			CNetManager::GetMe()->SendPacket(&msg);
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "正在删除角色.....");	
			return 0;
			
		}
	}

	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "请选择一个角色");	
	return 1;
}


// 进入游戏
int CGamePro_CharSel::SendEnterGameMsg(int iRoleIndex)
{
	if((iRoleIndex < m_iCharacterCount)&&(iRoleIndex >= 0))
	{
		if(iRoleIndex < MAX_SHOW_IN_UI)
		{
			CLAskCharLogin msg;

			m_EnterGameGUID = m_CharacterInfo[iRoleIndex].m_GUID;
			msg.SetCharGuid(m_EnterGameGUID);

			// 删除角色
			CNetManager::GetMe()->SendPacket(&msg);
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "准备进入游戏.....");	
			return 0;
		}
	}

	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "请选择一个角色");	
	return 1;
}


// 切换到服务器连接流程
void CGamePro_CharSel::ChangeToServerConnect()
{
	// 切换到连接game server流程
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcChangeScene);

	// 设置game server流程为断开状态
	CGameProcedure::s_pProcChangeScene->SetStatus(CGamePro_ChangeScene::CHANGESCENE_DISCONNECT);


}

// 位置是否合法
bool CGamePro_CharSel::IsValidIndex(int iRoleIndex)
{

	if((iRoleIndex < m_iCharacterCount)&&(iRoleIndex >= 0))
	{
		if(iRoleIndex < MAX_SHOW_IN_UI)
		{
			return true;
		}
	}

	return false;
}

// 删除选择的角色
int CGamePro_CharSel::DelSelRole()
{

	if((m_iCurSelRoleDel < m_iCharacterCount)&&(m_iCurSelRoleDel >= 0))
	{
		if(m_iCurSelRoleDel < MAX_SHOW_IN_UI)
		{
			CLAskDeleteChar msg;
			msg.SetCharGuid(m_CharacterInfo[m_iCurSelRoleDel].m_GUID);

			// 删除角色
			CNetManager::GetMe()->SendPacket(&msg);
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "正在删除角色.....");	
			return 0;
		}
	}

	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "请选择一个角色");	
	return 1;
}

void CGamePro_CharSel::SetCurSelDel(int iIndex)
{
	m_iCurSelRoleDel = -1;
}

void CGamePro_CharSel::ClearUIModel()
{
	for(int i = 0; i < MAX_SHOW_IN_UI; i++)
	{
		m_Character[i].DestroyUIModel();
	}
}



