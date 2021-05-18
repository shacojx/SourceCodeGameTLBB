#include "StdAfx.h"
#include "..\Network\NetManager.h"
#include "GMGameInterface.h"
#include "GMInterface_Script_SystemSetup.h"
#include "..\Procedure\GameProcedure.h"
#include "../Event/GMEventSystem.h"
#include "../Sound/GMSoundSystem.h"
#include "..\Variable\GMVariable.h"
#include "TDVariable.h"
#include "LuaPlus.h"
#include "../Global.h"
#include "CGModifySetting.h"
#include "CGAskPrivateInfo.h"
#include "../Object/ObjectManager.h"
#include "../Object/Character/Obj_PlayerMySelf.h"
#include "../Object/Character/Obj_Character.h"
#include "..\DataPool\GMDP_CharacterData.h"

#include "CGApplyPrivateInfo.h"
#include "TDUtil.h"
#include "..\WxRender\RenderSystem.h"

//#include "../Network/NetManager.h"

SCRIPT_SANDBOX::SystemSetup*	LuaExport< SCRIPT_SANDBOX::SystemSetup >::s_pMe = 0;
LuaPlus::LuaObject*				LuaExport< SCRIPT_SANDBOX::SystemSetup >::s_pMetaTable = 0;

namespace SCRIPT_SANDBOX
{
	VOID WINAPI SystemSetup::_OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData)
	{
		if(pEvent && pEvent->pEventDef && pEvent->pEventDef->idEvent == GE_VARIABLE_CHANGED)
		{
			const STRING& strVariable = pEvent->vArg[0];
			const STRING& strValue = pEvent->vArg[1];

			//----------------------------
			//屏幕分辨率发生改变
			if(strVariable == "View_Resoution") 
			{
				fVector2 fResoution = CVariableSystem::GetMe()->GetAs_Vector2(strVariable.c_str());
				s_pMe->_setResolution((INT)fResoution.x, (INT)fResoution.y);
			}
			//----------------------------
			//是否全屏发生改变
			else if(strVariable == "View_FullScreen")
			{
				s_pMe->_setResolution(strValue == "1");
			}
			//----------------------------
			//...
		}
	}

	VOID SystemSetup::_setResolution(INT nWidth, INT nHeight)
	{
		//如果是在全屏状态下
		if(CVariableSystem::GetMe()->GetAs_Int("View_FullScreen") == 1) return;

		//限定能够设定的分辨率
//		if( (nWidth == 800  && nHeight == 600) ||
//			(nWidth == 1024 && nHeight == 768) ||
//			(nWidth == 1280 && nHeight == 1024) )
		{
			//是否和现有的窗口大小一致
			RECT rectCurrent;
			GetClientRect(g_hMainWnd, &rectCurrent);
			if(nWidth==rectCurrent.right-rectCurrent.left && 
				nHeight == rectCurrent.bottom-rectCurrent.top) return;

			//根据边界调整真实窗口大小
			RECT rectWindow;
			SetRect(&rectWindow, 0, 0, nWidth, nHeight);
			AdjustWindowRect(&rectWindow, GetWindowStyle(g_hMainWnd), FALSE);

			//屏幕有效工作区(排出任务条)
			RECT rectScreen;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &rectScreen, 0);

			//如果无法容纳该窗口，直接最大化
			if(	(rectScreen.right-rectScreen.left) <= (rectWindow.right-rectWindow.left) ||
				(rectScreen.bottom-rectScreen.top) <= (rectWindow.bottom-rectWindow.top))
			{
				::ShowWindow(g_hMainWnd, SW_MAXIMIZE);
				return;
			}

			//居中
			UINT dwX = ((rectScreen.right-rectScreen.left)-(rectWindow.right-rectWindow.left))/2;
			UINT dwY = ((rectScreen.bottom-rectScreen.top)-(rectWindow.bottom-rectWindow.top))/2;

			::ShowWindow(g_hMainWnd, SW_RESTORE);
			::MoveWindow(g_hMainWnd, dwX, dwY, 
				rectWindow.right-rectWindow.left, rectWindow.bottom-rectWindow.top, TRUE);

			CGameProcedure::HandlePossibleSizeChange(WM_EXITSIZEMOVE, 0, 0);
		}
	}

	VOID SystemSetup::_setResolution(BOOL bFullScreen)
	{
		static BOOL s_bFull = FALSE;
		if(s_bFull == bFullScreen) return;

		if(bFullScreen)
		{
			int nScreenWidth  = ::GetSystemMetrics(SM_CXSCREEN);
			int nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);

			//根据边界调整真实窗口大小
			RECT rectWindow;
			SetRect(&rectWindow, 0, 0, nScreenWidth, nScreenHeight);
			AdjustWindowRect(&rectWindow, GetWindowStyle(g_hMainWnd), FALSE);

			::ShowWindow(g_hMainWnd, SW_RESTORE);
			::MoveWindow(g_hMainWnd, rectWindow.left, rectWindow.top, 
				rectWindow.right-rectWindow.left, rectWindow.bottom-rectWindow.top, TRUE);
			
//			::SetWindowPos(g_hMainWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		}
		else
		{
			::ShowWindow(g_hMainWnd, SW_MAXIMIZE);
//			::SetWindowPos(g_hMainWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		}

		s_bFull = bFullScreen;

		CGameProcedure::HandlePossibleSizeChange(WM_EXITSIZEMOVE, 0, 0);
	}
	//纹理相关的设定，地形阴影
	INT SystemSetup::Lua_Texture(LuaPlus::LuaState* state)
	{
		
		//高光
		static INT nQuality = 0;
		if(nQuality == 0)
			nQuality = 1;
		else if(nQuality == 1)
			nQuality = 2;
		else if(nQuality == 2)
			nQuality = 0;
		CGameProcedure::s_pGfxSystem->Scene_SetLightmapQuality(nQuality);

		return 0;
	}

	//显示质量相关的设定
	INT SystemSetup::Lua_Display(LuaPlus::LuaState* state)
	{
		return 0;
	}
	//声音相关的设定
	INT SystemSetup::Lua_Sound(LuaPlus::LuaState* state)
	{
		return 0;
	}
	//打开系统主界面
	INT SystemSetup::Lua_OpenSetup(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_SYSTEMFRAME);

		return 0;
	}
	//打开视频设置界面
	INT SystemSetup::Lua_ViewSetup(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_VIEWSETUP);
		return 0;
	}
	//打开声音设置界面
	INT SystemSetup::Lua_SoundSetup(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_SOUNDSETUP);
		return 0;
	}
	//打开UI设置界面
	INT SystemSetup::Lua_UISetup(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_UISETUP);
		return 0;
	}
	//打开按键设置界面
	INT SystemSetup::Lua_InputSetup(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUTSETUP);
		return 0;
	}
	//打开游戏性设置界面
	INT SystemSetup::Lua_GameSetup(LuaPlus::LuaState* state)
	{
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_GAMESETUP);
		return 0;
	}
	//游戏帮助
	INT SystemSetup::Lua_GameHelp(LuaPlus::LuaState* state)
	{
		return 0;
	}
	//返回游戏
	INT SystemSetup::Lua_BackGame(LuaPlus::LuaState* state)
	{
		return 0;
	}
	//获得视频设置数据
	INT SystemSetup::Lua_View_GetData(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if (!(args[2].IsString()))
		{
			TDThrow("SystemSetup::View_GetData[2] param parameter error");
		}
		std::string str	= args[2].GetString();

		if(str == "dxyy")			//地形阴影
		{
			INT fTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_LightmapQuality");
			state->PushNumber(fTemp);
		}
		else if(str == "fhj")		//反混角
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_Fanhunjiao");
			state->PushNumber(nTemp);
		}
		else if(str == "wtdh")		//物体动画
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_ObjectMove");
			state->PushNumber(nTemp);
		}
		else if(str == "gamma")		//gamma调整
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_Gamma");
			state->PushNumber(nTemp);
		}
		else if(str == "yszl")		//颜色质量
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_ColorQuality");
			state->PushNumber(nTemp);
		}
		else if(str == "wldx")		//纹理大小
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_TextureQuality");
			state->PushNumber(nTemp);
		}
		else if(str == "cy")		//采样
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_TextureFiltering");
			state->PushNumber(nTemp);
		}
		else if(str == "dbxs")		//多边形数
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_PolygonQuantity");
			state->PushNumber(nTemp);
		}

		else if(str == "dxgg")		//地形高光
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_GroundHighLight");
			state->PushNumber(nTemp);
		}
		else if(str == "rwgg")		//人物高光
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_HumanHighLight");
			state->PushNumber(nTemp);
		}
		else if(str == "rwyy")		//人物阴影
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_HumanLightmap");
			state->PushNumber(nTemp);
		}
		else if(str == "qpfg")		//全屏泛光
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_FullScreenLight");
			state->PushNumber(nTemp);
		}
		else if(str == "cztb")		//垂直同步
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_Aplomb");
			state->PushNumber(nTemp);
		}
		else if(str == "sdh")		//水动画
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_WaterMove");
			state->PushNumber(nTemp);
		}
		else if(str == "qpms")		//全屏模式
		{
			INT nTemp = CGameProcedure::s_pVariableSystem->GetAs_Int("View_FullScreen");
			state->PushNumber(nTemp);
		}

		return 1;
	}
	//设置视频数据
	INT SystemSetup::Lua_View_SetData(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if (!(args[2].IsString()))
		{
			TDThrow("SystemSetup::View_SetData[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("SystemSetup::View_SetData[3] param parameter error");
		}
		std::string str	= args[2].GetString();
		INT fTemp = args[3].GetInteger();

		if(str == "dxyy")			//地形阴影
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_LightmapQuality",fTemp,FALSE);
		}
		else if(str == "fhj")		//反混角
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_Fanhunjiao",fTemp,FALSE);
		}
		else if(str == "wtdh")		//物体动画
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_ObjectMove",fTemp,FALSE);
		}
		else if(str == "gamma")		//gamma调整
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_Gamma",fTemp,FALSE);
		}
		else if(str == "yszl")		//颜色质量
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_ColorQuality",fTemp,FALSE);
		}
		else if(str == "wldx")		//纹理大小
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_TextureQuality",fTemp,FALSE);
		}
		else if(str == "cy")		//采样
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_TextureFiltering",fTemp,FALSE);
		}
		else if(str == "dbxs")		//多边形数
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_PolygonQuantity",fTemp,FALSE);
		}

		else if(str == "dxgg")		//地形高光
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_GroundHighLight",fTemp,FALSE);
		}
		else if(str == "rwgg")		//人物高光
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_HumanHighLight",fTemp,FALSE);
		}
		else if(str == "rwyy")		//人物阴影
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_HumanLightmap",fTemp,FALSE);
		}
		else if(str == "qpfg")		//全屏泛光
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_FullScreenLight",fTemp,FALSE);
		}
		else if(str == "cztb")		//垂直同步
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_Aplomb",fTemp,FALSE);
		}
		else if(str == "sdh")		//水动画
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("View_WaterMove",fTemp,FALSE);
		}
		else if(str == "qpms")		//全屏模式
		{
			char szTemp[MAX_PATH];
			_snprintf(szTemp, MAX_PATH, "%d",fTemp);
			CGameProcedure::s_pVariableSystem->SetVariableDelay("View_FullScreen",szTemp);
		}

		return 0;
	}
	//保存视频设置数据
	INT SystemSetup::Lua_View_SaveToFile(LuaPlus::LuaState* state)
	{
		return 0;
	}

	//-----------------------------------------------------------------------------
	//游戏性相关
	INT SystemSetup::Lua_GameGetData(LuaPlus::LuaState* state)
	{
		//向服务器请求数据

		if( ( m_GameData & (1 << (GSF_CANNOT_RECV_MAIL)) ) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		if( ( m_GameData & ( 1 << (GSF_CANNOT_ADD_FRIEND)) ) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		if( (m_GameData & (1<<(GSF_CANNOT_RECV_STRANGER_MAIL))) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		if( (m_GameData & (1<<(GSF_REFUSE_TRADE))) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		if( (m_GameData & (1<<(GSF_REFUSE_TEAM_INVITE))) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}
		
		if( (m_GameData & (1<<(GSF_CLOSE_POPUP))) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}

		if( (m_GameData & (1<<(GSF_REFUSE_LOOK_SPOUSE_INFO))) > 0 )
		{
			state->PushInteger(1);
		}
		else
		{
			state->PushInteger(0);
		}
		state->PushInteger(1);


		return 8;
	}

	INT SystemSetup::Lua_SaveGameSetup(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if (!(args[2].IsInteger()))
		{
			TDThrow("SystemSetup::SaveGameSetup[2] param parameter error");
		}
		if (!(args[3].IsInteger()))
		{
			TDThrow("SystemSetup::SaveGameSetup[3] param parameter error");
		}
		if (!(args[4].IsInteger()))
		{
			TDThrow("SystemSetup::SaveGameSetup[4] param parameter error");
		}
		if (!(args[5].IsInteger()))
		{
			TDThrow("SystemSetup::SaveGameSetup[5] param parameter error");
		}
		if (!(args[6].IsInteger()))
		{
			TDThrow("SystemSetup::SaveGameSetup[6] param parameter error");
		}
		if (!(args[7].IsInteger()))
		{
			TDThrow("SystemSetup::SaveGameSetup[7] param parameter error");
		}
		if (!(args[8].IsInteger()))
		{
			TDThrow("SystemSetup::SaveGameSetup[8] param parameter error");
		}
		if (!(args[9].IsInteger()))
		{
			TDThrow("SystemSetup::View_SetData[9] param parameter error");
		}
		INT nRefuseAllMail			= args[2].GetInteger();
		INT nRefuseJoinMeToFriend	= args[3].GetInteger();
		INT nRefuseStrangerMail		= args[4].GetInteger();
		INT nRefuseExchange			= args[5].GetInteger();
		INT nRefuseTeamRequest		= args[6].GetInteger();
		INT nCloseMyPaopao			= args[7].GetInteger();
		INT nRefuseSeeMySpouse		= args[8].GetInteger();

		INT nCloseTalkFrame			= args[9].GetInteger();

		INT Original = m_GameData;

		if(nRefuseAllMail == 0)
		{
			INT nGsf =  1 << (GSF_CANNOT_RECV_MAIL);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf =  1 << (GSF_CANNOT_RECV_MAIL);		
			m_GameData = nGsf | m_GameData;
		}

		if(nRefuseJoinMeToFriend == 0)
		{
			INT nGsf =  1 << (GSF_CANNOT_ADD_FRIEND);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf =  1 << (GSF_CANNOT_ADD_FRIEND);		
			m_GameData = nGsf | m_GameData;
		}

		if(nRefuseStrangerMail == 0)
		{
			INT nGsf = 1 << (GSF_CANNOT_RECV_STRANGER_MAIL);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf = 1<< (GSF_CANNOT_RECV_STRANGER_MAIL);		
			m_GameData = nGsf | m_GameData;
		}

		if(nRefuseExchange == 0)
		{
			INT nGsf = 1 << (GSF_REFUSE_TRADE);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf = 1<< (GSF_REFUSE_TRADE);		
			m_GameData = nGsf | m_GameData;
		}

		if(nRefuseTeamRequest == 0)
		{
			INT nGsf = 1 << (GSF_REFUSE_TEAM_INVITE);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf = 1<< (GSF_REFUSE_TEAM_INVITE);		
			m_GameData = nGsf | m_GameData;
		}

		if(nCloseMyPaopao == 0)
		{
			INT nGsf = 1 << (GSF_CLOSE_POPUP);
			m_GameData = ~(nGsf) & m_GameData;
			CGameProcedure::s_pUISystem->SetPaoPaoActive(true);
		}
		else
		{
			INT nGsf = 1<< (GSF_CLOSE_POPUP);		
			m_GameData = nGsf | m_GameData;
			CGameProcedure::s_pUISystem->SetPaoPaoActive(false);
		}

		if(nRefuseSeeMySpouse == 0)
		{
			INT nGsf = 1 << (GSF_REFUSE_LOOK_SPOUSE_INFO);
			m_GameData = ~(nGsf) & m_GameData;
		}
		else
		{
			INT nGsf = 1<< (GSF_REFUSE_LOOK_SPOUSE_INFO);		
			m_GameData = nGsf | m_GameData;
		}


		if(Original != m_GameData)
		{
			CGModifySetting msg;
			_OWN_SETTING* pSend_Value = msg.GetValue();

			msg.SetType(SETTING_TYPE_GAME);
			pSend_Value->m_SettingType = -1;
			pSend_Value->m_SettingData = m_GameData;
			//给服务器发送CGMODIFYSETTING
	
			CNetManager::GetMe()->SendPacket( &msg );
		}

		return 0;
	}

	INT  SystemSetup::Set_GameData(INT nData)
	{
		m_GameData = nData;
		return 1;
	}
	//请求玩家信息
	INT SystemSetup::Lua_AskPrivateInfo(LuaPlus::LuaState* state)
	{
		return 0;
	}
	//提交玩家信息
	INT SystemSetup::Lua_ApplyPrivateInfo(LuaPlus::LuaState* state)
	{
		//CGApplyPrivateInfo msg;

		//msg.SetAge				( (BYTE)CDataPool::GetMe()->PI_GetMyAge() );        //pHuman->GetDB()->GetPrivateInfoDB()->m_Age);
		//msg.SetBloodType		( CDataPool::GetMe()->PI_GetMyBloodType());
		//msg.SetCharGUID			( CDataPool::GetMe()->PI_GetMyCharGUID());
		//msg.SetCityLength		( (BYTE) strlen( CDataPool::GetMe()->PI_GetMyCity() ));
		//msg.SetCity				( (CHAR*)CDataPool::GetMe()->PI_GetMyCity());
		//msg.SetConsella			( CDataPool::GetMe()->PI_GetMyConsella());
		//msg.SetEmailInfoLength	( (BYTE) strlen( CDataPool::GetMe()->PI_GetMyEmailInfo() ));
		//msg.SetEmailInfo		( (CHAR*)CDataPool::GetMe()->PI_GetMyEmailInfo());
		//msg.SetInfoType			( CDataPool::GetMe()->PI_GetMyInfoType());
		//msg.SetJobInfoLength	( (BYTE) strlen( CDataPool::GetMe()->PI_GetMyJobInfo() ));
		//msg.SetJobInfo			( (CHAR*)CDataPool::GetMe()->PI_GetMyJobInfo());
		//msg.SetLuckWordLength	( (BYTE) strlen( CDataPool::GetMe()->PI_GetMyLuckWord() ));
		//msg.SetLuckWord			( (CHAR*)CDataPool::GetMe()->PI_GetMyLuckWord());
		//msg.SetSchoolInfoLength	( (BYTE) strlen( CDataPool::GetMe()->PI_GetMySchoolInfo() ));
		//msg.SetSchoolInfo		( (CHAR*)CDataPool::GetMe()->PI_GetMySchoolInfo());
		//msg.SetProvince			( CDataPool::GetMe()->PI_GetMyProvince());
		//msg.SetSex				( CDataPool::GetMe()->PI_GetMySex());
		//msg.SetYearAnimal		( CDataPool::GetMe()->PI_GetMyYearAnimal());

		//CNetManager::GetMe()->SendPacket( &msg );
		return 0;
	}
	//获得玩家信息
	INT SystemSetup::Lua_GetPrivateInfo(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("SystemSetup::Lua_GetPrivateInfo[2] param parameter error");
		}
		if (!(args[3].IsString()))
		{
			TDThrow("SystemSetup::Lua_GetPrivateInfo[3] param parameter error");
		}
		//std::string strWho = args[2].GetString();
		//std::string str = args[3].GetString();

		//if( strWho == "self" )
		//{
		//	if( str == "type" )
		//	{
		//		state->PushInteger(CDataPool::GetMe()->PI_GetMyInfoType());
		//	}
		//	else if( str == "guid" )
		//	{
		//		CHAR szTemp[16] = {0};
		//		_snprintf(szTemp, 16, "%X",CDataPool::GetMe()->PI_GetMyCharGUID());
		//		state->PushString(szTemp);
		//	}
		//	else if( str == "age" )
		//	{
		//		state->PushInteger(CDataPool::GetMe()->PI_GetMyAge());
		//	}
		//	else if( str == "sex" )
		//	{
		//		state->PushInteger(CDataPool::GetMe()->PI_GetMySex());
		//	}
		//	else if( str == "job" )
		//	{
		//		state->PushString(CDataPool::GetMe()->PI_GetMyJobInfo());
		//	}
		//	else if( str == "school" )
		//	{
		//		state->PushString(CDataPool::GetMe()->PI_GetMySchoolInfo());
		//	}
		//	else if( str == "blood" )
		//	{
		//		state->PushInteger(CDataPool::GetMe()->PI_GetMyBloodType());
		//	}
		//	else if( str == "animal" )
		//	{
		//		state->PushInteger(CDataPool::GetMe()->PI_GetMyYearAnimal());
		//	}
		//	else if( str == "Consella" )
		//	{
		//		state->PushInteger(CDataPool::GetMe()->PI_GetMyConsella());
		//	}
		//	else if( str == "Province" )
		//	{
		//		state->PushInteger(CDataPool::GetMe()->PI_GetMyProvince());
		//	}
		//	else if( str == "city" )
		//	{
		//		state->PushString(CDataPool::GetMe()->PI_GetMyCity());
		//	}
		//	else if( str == "email" )
		//	{
		//		state->PushString(CDataPool::GetMe()->PI_GetMyEmailInfo());
		//	}
		//	else if( str == "luck" )
		//	{
		//		state->PushString(CDataPool::GetMe()->PI_GetMyLuckWord());
		//	}
		//	else if( str == "name" )
		//	{
		//		state->PushString(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
		//	}
		//	else
		//	{
		//		state->PushInteger(0);
		//	}
		//}
		//else
		//{
		//	if( str == "type" )
		//	{
		//		state->PushInteger(CDataPool::GetMe()->PI_GetOtInfoType());
		//	}
		//	else if( str == "guid" )
		//	{
		//		CHAR szTemp[16];
		//		_snprintf(szTemp, 16, "%X",CDataPool::GetMe()->PI_GetOtCharGUID());
		//		state->PushString(szTemp);
		//	}
		//	else if( str == "age" )
		//	{
		//		state->PushInteger(CDataPool::GetMe()->PI_GetOtAge());
		//	}
		//	else if( str == "sex" )
		//	{
		//		state->PushInteger(CDataPool::GetMe()->PI_GetOtSex());
		//	}
		//	else if( str == "job" )
		//	{
		//		state->PushString(CDataPool::GetMe()->PI_GetOtJobInfo());
		//	}
		//	else if( str == "school" )
		//	{
		//		state->PushString(CDataPool::GetMe()->PI_GetOtSchoolInfo());
		//	}
		//	else if( str == "blood" )
		//	{
		//		state->PushInteger(CDataPool::GetMe()->PI_GetOtBloodType());
		//	}
		//	else if( str == "animal" )
		//	{
		//		state->PushInteger(CDataPool::GetMe()->PI_GetOtYearAnimal());
		//	}
		//	else if( str == "Consella" )
		//	{
		//		state->PushInteger(CDataPool::GetMe()->PI_GetOtConsella());
		//	}
		//	else if( str == "Province" )
		//	{
		//		state->PushInteger(CDataPool::GetMe()->PI_GetOtProvince());
		//	}
		//	else if( str == "city" )
		//	{
		//		state->PushString(CDataPool::GetMe()->PI_GetOtCity());
		//	}
		//	else if( str == "email" )
		//	{
		//		state->PushString(CDataPool::GetMe()->PI_GetOtEmailInfo());
		//	}
		//	else if( str == "luck" )
		//	{
		//		state->PushString(CDataPool::GetMe()->PI_GetOtLuckWord());
		//	}
		//	else if( str == "name" )
		//	{
		//		state->PushString(CDataPool::GetMe()->PI_GetOtName());
		//	}
		//	else
		//	{
		//		state->PushInteger(0);
		//	}
		//}

		return 0;
	}

	//获得玩家信息
	INT SystemSetup::Lua_SetPrivateInfo(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("SystemSetup::Lua_SetPrivateInfo[2] param parameter error");
		}
		if (!(args[3].IsString()))
		{
			TDThrow("SystemSetup::Lua_SetPrivateInfo[3] param parameter error");
		}
		//std::string strWho = args[2].GetString();
		//std::string str = args[3].GetString();

		//std::string strDesc;
		//if ((args[4].IsString()))
		//{
		//	strDesc = args[4].GetString();
		//	//验证商店名称的合法性
		//	if(!CGameProcedure::s_pUISystem->CheckStringFilter(strDesc))
		//	{
		//		STRING strTemp = "";
		//		strTemp = NOCOLORMSGFUNC("ERRORSpecialString");
		//		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		//		strDesc = "";

		//		state->PushInteger(0);
		//		return 1;
		//	}
		//}
		//if( strWho == "self" )
		//{
		//	if( str == "type" )
		//	{
		//		CDataPool::GetMe()->PI_SetMyInfoType(args[4].GetInteger() );
		//	}
		//	else if( str == "age" )
		//	{
		//		CDataPool::GetMe()->PI_SetMyAge((args[4].GetInteger()==-1) ? 0 : args[4].GetInteger());
		//	}
		//	else if( str == "sex" )
		//	{
		//		CDataPool::GetMe()->PI_SetMySex((args[4].GetInteger()==-1) ? 0 : args[4].GetInteger());
		//	}
		//	else if( str == "job" )
		//	{
		//		CDataPool::GetMe()->PI_SetMyJobInfo(args[4].GetString());
		//	}
		//	else if( str == "school" )
		//	{
		//		CDataPool::GetMe()->PI_SetMySchoolInfo(args[4].GetString());
		//	}
		//	else if( str == "blood" )
		//	{
		//		CDataPool::GetMe()->PI_SetMyBloodType((args[4].GetInteger()==-1) ? 0 : args[4].GetInteger());
		//	}
		//	else if( str == "animal" )
		//	{
		//		CDataPool::GetMe()->PI_SetMyYearAnimal((args[4].GetInteger()==-1) ? 0 : args[4].GetInteger());
		//	}
		//	else if( str == "Consella" )
		//	{
		//		CDataPool::GetMe()->PI_SetMyConsella((args[4].GetInteger()==-1) ? 0 : args[4].GetInteger());
		//	}
		//	else if( str == "Province" )
		//	{
		//		CDataPool::GetMe()->PI_SetMyProvince((args[4].GetInteger()==-1) ? 0 : args[4].GetInteger());
		//	}
		//	else if( str == "city" )
		//	{
		//		CDataPool::GetMe()->PI_SetMyCity(args[4].GetString());
		//	}
		//	else if( str == "email" )
		//	{
		//		CDataPool::GetMe()->PI_SetMyEmailInfo(args[4].GetString());
		//	}
		//	else if( str == "luck" )
		//	{
		//		CDataPool::GetMe()->PI_SetMyLuckWord(args[4].GetString());
		//	}
		//}
		//else
		//{
		//	if( str == "type" )
		//	{
		//		CDataPool::GetMe()->PI_SetOtInfoType(args[4].GetInteger());
		//	}
		//	else if( str == "age" )
		//	{
		//		CDataPool::GetMe()->PI_SetOtAge(args[4].GetInteger());
		//	}
		//	else if( str == "sex" )
		//	{
		//		CDataPool::GetMe()->PI_SetOtSex(args[4].GetInteger());
		//	}
		//	else if( str == "job" )
		//	{
		//		CDataPool::GetMe()->PI_SetOtJobInfo(args[4].GetString());
		//	}
		//	else if( str == "school" )
		//	{
		//		CDataPool::GetMe()->PI_SetOtSchoolInfo(args[4].GetString());
		//	}
		//	else if( str == "blood" )
		//	{
		//		CDataPool::GetMe()->PI_SetOtBloodType(args[4].GetInteger());
		//	}
		//	else if( str == "animal" )
		//	{
		//		CDataPool::GetMe()->PI_SetOtYearAnimal(args[4].GetInteger());
		//	}
		//	else if( str == "Consella" )
		//	{
		//		CDataPool::GetMe()->PI_SetOtConsella(args[4].GetInteger());
		//	}
		//	else if( str == "Province" )
		//	{
		//		CDataPool::GetMe()->PI_SetOtProvince(args[4].GetInteger());
		//	}
		//	else if( str == "city" )
		//	{
		//		CDataPool::GetMe()->PI_SetOtCity(args[4].GetString());
		//	}
		//	else if( str == "email" )
		//	{
		//		CDataPool::GetMe()->PI_SetOtEmailInfo(args[4].GetString());
		//	}
		//	else if( str == "luck" )
		//	{
		//		CDataPool::GetMe()->PI_SetOtLuckWord(args[4].GetString());
		//	}
		//}
		//state->PushInteger(1);

		return 0;
	}

	//打开页面
	INT SystemSetup::Lua_OpenPrivatePage(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("SystemSetup::Lua_OpenPrivatePage[2] param parameter error");
		}
		std::string str = args[2].GetString();

		//if( str == "self" )
		//{
		//	CGAskPrivateInfo msg;
		//	ObjID_t SelfId = CObjectManager::GetMe()->GetMySelf()->GetServerID();

		//	msg.SetPlayerID((ObjID_t)SelfId);

		//	CNetManager::GetMe()->SendPacket( &msg );
		//}
		//else
		//{
		//	//使用已经刷新到数据池中的数据来显示界面
		//	CEventSystem::GetMe()->PushEvent(GE_OPEN_PRIVATE_INFO,"other");
		//}

		return 0;
	}
	//从玩家信息界面切换到玩家装备界面
	INT SystemSetup::Lua_OpenEquipFrame(LuaPlus::LuaState* state)
	{
		//打开装备界面
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("SystemSetup::Lua_OpenEquipFrame[2] param parameter error");
		}
		std::string str = args[2].GetString();

		if( str == "self" )
		{
			//打开自己的
			CEventSystem::GetMe()->PushEvent(GE_OPEN_EQUIP);
		}
		else
		{
			// 打开别人的
			CEventSystem::GetMe()->PushEvent(GE_OTHERPLAYER_UPDATE_EQUIP);
		}
		return 0;
	}
	//从玩家信息界面切换到玩家宠物界面
	INT SystemSetup::Lua_OpenPetFrame(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("SystemSetup::Lua_OpenPetFrame[2] param parameter error");
		}
		std::string str = args[2].GetString();

		if( str == "self" )
		{
			CEventSystem::GetMe()->PushEvent(GE_TOGLE_PET_PAGE);
		}
		else
		{
			//暂时没有这个功能
		}
		return 0;
	}
	//获得玩家资料界面关心的Obj
	INT SystemSetup::Lua_GetCaredObjId(LuaPlus::LuaState* state)
	{
		//state->PushInteger(CDataPool::GetMe()->PI_GetObjId());
		state->PushInteger(0);

		return 1;
	}
}
