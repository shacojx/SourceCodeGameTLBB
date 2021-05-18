//
// 2006-5-21
//
//

#include "stdafx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"
#include "../Procedure/GamePro_CharSel.h"
#include "../Procedure/GamePro_Login.h"
#include "../Procedure/GameProcedure.h"
#include "../Procedure/GamePro_CharCreate.h"

#include "../NetWork/NetManager.h"
#include "LuaPlus.h"
#include "../Global.h"
#include "../Event/GMEventSystem.h"
#include "TDException.h"
#include "../Action/GMActionSystem.h"
#include "../DataPool/GMUIDataPool.h"
#include "../DataPool/GMDataPool.h"
#include "../Object/ObjectManager.h"
#include "../Object/Object.h"
#include "../Object/Character/Obj_Character.h"
#include "../Input/GMInputSystem.h"
#include "../world/worldmanager.h"
#include "../object/character/obj_playermyself.h"
#include "ScriptCommandStruct.h"

#include "..\..\Common\database\tlbb_dbc.h"
#include "..\..\Common\BuffImpactMgr.h"
#include "TDDBC_Struct.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "..\Sound\GMSoundSystem.h"


namespace SCRIPT_SANDBOX
{

	// 定义静态成员变量.
	CGameProduce_Login CGameProduce_Login::s_GameProduceLogin;

	// 静态lua导出变量定义.
	LuaPlus::LuaObject* CGameProduce_Login::s_pMetaTable = NULL;

	CGameProduce_Login::CGameProduce_Login()
	{

	}

	CGameProduce_Login::~CGameProduce_Login()
	{

	}

	//得到区域服务器的个数.
	INT CGameProduce_Login::GetAreaCount(LuaPlus::LuaState* state)
	{

		int iAreaCount = 0;
		if(CGameProcedure::s_pProcLogIn)
		{
			iAreaCount = CGameProcedure::s_pProcLogIn->GetAreaCount();
		}
		
		state->PushInteger(iAreaCount);
		return 1;
		
	}
	
	//得到区域服务器的名字.
	INT CGameProduce_Login::GetAreaName(LuaPlus::LuaState* state)
	{

		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int nAreaIndex = args[2].GetInteger();

		std::string strInfo = "";
		if(CGameProcedure::s_pProcLogIn)
		{
			strInfo = CGameProcedure::s_pProcLogIn->GetAreaName(nAreaIndex);
		}
	
		state->PushString(strInfo.c_str());
		
		return 1;
	}

	//得到区域服务器的信息
	INT CGameProduce_Login::GetAreaServerInfo(LuaPlus::LuaState* state)
	{

		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int nAreaIndex = args[2].GetInteger();

		std::string strInfoName = "";
		int iAreaProvide		= 0;
		if(CGameProcedure::s_pProcLogIn)
		{
			PAREA_INFO pInfo = CGameProcedure::s_pProcLogIn->GetAreaInfo(nAreaIndex);
			if(pInfo)
			{
				strInfoName  = pInfo->szAreaName;
				iAreaProvide = pInfo->iProvide;
			}
		}
	
		state->PushString(strInfoName.c_str());
		state->PushInteger(iAreaProvide);
		return 2;
	}


	//得到区域服务器中, login server 的个数.
	INT CGameProduce_Login::GetAreaLoginServerCount(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int nAreaIndex = args[2].GetInteger();

		int iLoginServerCount = 0;
		if(CGameProcedure::s_pProcLogIn)
		{
			iLoginServerCount = CGameProcedure::s_pProcLogIn->GetAreaLoginServerCount(nAreaIndex);
		}
	
		state->PushInteger(iLoginServerCount);
		
		return 1;
	}


	// 得到login server 的信息.
	INT CGameProduce_Login::GetAreaLoginServerInfo(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}


		int nAreaIndex		  = args[2].GetInteger();
		int nLoginServerIndex = args[3].GetInteger();

		PLOGINSERVER_INOF pLoginServerInfo = NULL;
		if(CGameProcedure::s_pProcLogIn)
		{
			pLoginServerInfo = CGameProcedure::s_pProcLogIn->GetAreaLoginServerInfo(nAreaIndex, nLoginServerIndex);
		
		}
	
		if(pLoginServerInfo)
		{
			// 得到服务器名字
			state->PushString(pLoginServerInfo->szLoginServerName.c_str());

			// 得到服务器状态
			state->PushInteger(pLoginServerInfo->iLoginServerStatus);
		}
		else
		{
			state->PushString("");
			state->PushInteger(0);
		}
	
		return 2;

	}

	// 连接到login server
	INT CGameProduce_Login::ConnectToLoginServer(LuaPlus::LuaState* state)
	{

		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}


		int nAreaIndex		  = args[2].GetInteger();
		int nLoginServerIndex = args[3].GetInteger();

		if(CGameProcedure::s_pProcLogIn)
		{
			
			CGameProcedure::s_pProcLogIn->ConnectToLoginServer(nAreaIndex, nLoginServerIndex);
		
		}
		return 0;
	}


	// 退到服务器选择界面
	INT CGameProduce_Login::ExitToSelectServer(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::s_pProcLogIn)
		{
			
			CGameProcedure::s_pProcLogIn->OpenSelectServerDlg();
		
		}
		return 0;
	}


	// 验证用户名和密码
	INT CGameProduce_Login::CheckAccount(LuaPlus::LuaState* state)
	{

		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsString()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}


		const char* pUserName = args[2].GetString();
		const char* pPassword = args[3].GetString();

		if(CGameProcedure::s_pProcLogIn)
		{
			
			CGameProcedure::s_pProcLogIn->CheckAccount((CHAR*)pUserName, (CHAR*)pPassword);
		
		}
		return 0;
	}


	// 选择一个服务器
	INT CGameProduce_Login::SelectLoginServer(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}


		int nAreaIndex		  = args[2].GetInteger();
		int nLoginServerIndex = args[3].GetInteger();

		if(CGameProcedure::s_pProcLogIn)
		{
			
			CGameProcedure::s_pProcLogIn->SelectLoginServer(nAreaIndex, nLoginServerIndex);
		
		}

		return 0;
	}

	// 从人物选择界面切换到帐号输入界面
	INT CGameProduce_Login::ChangeToAccountInputDlgFromSelectRole(LuaPlus::LuaState* state)
	{
		
		if(CGameProcedure::s_pProcCharSel)
		{
			
			CGameProcedure::s_pProcCharSel->ChangeToAccountInput();
		
		}
		return 0;
	}

	// 从人物选择界面切换到人物创建界面
	INT CGameProduce_Login::ChangeToCreateRoleDlgFromSelectRole(LuaPlus::LuaState* state)
	{
		
		if(CGameProcedure::s_pProcCharSel)
		{
			if(CGameProcedure::s_pProcCharSel->GetCurRoleCount() >= 3)
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "角色已经满了! ");	
				return 0;
			}
			CGameProcedure::s_pProcCharSel->ChangeToCreateRole();
		}
		return 0;
	}

	// 从人物创建界面切换到人物选择界面
	INT CGameProduce_Login::ChangeToSelectRoleDlgFromCreateRole(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::s_pProcCharCreate)
		{
			
			CGameProcedure::s_pProcCharCreate->ChangeToSelectRole();
		
		}//
		return 0;
	}

	// 得到角色的个数
	INT CGameProduce_Login::GetRoleCount(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		int iRoleCount = 0;
		if(CGameProcedure::s_pProcCharSel)
		{
			
			iRoleCount = CGameProcedure::s_pProcCharSel->GetCurRoleCount();

		}//	
		
		state->PushInteger(iRoleCount);
		return 1;
	}

	// 得到角色的信息
	INT CGameProduce_Login::GetRoleInfo(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iRoleIndex = args[2].GetInteger();
		const DB_CHAR_BASE_INFO* pInfo = NULL;
		if(CGameProcedure::s_pProcCharSel)
		{
			
			pInfo = CGameProcedure::s_pProcCharSel->GetRoleInfo(iRoleIndex);
			if(pInfo)
			{
				// 名字
				state->PushString(pInfo->m_Name);
				// 门派
				state->PushInteger(pInfo->m_Menpai);
				// 等级
				state->PushInteger(pInfo->m_Level);
				// 阵营
				state->PushInteger(pInfo->m_Camp);
				return 4;
			}

		}//	
		
		// 名字
		state->PushString("");
		// 门派
		state->PushInteger(0);
		// 等级
		state->PushInteger(0);
		// 阵营
		state->PushInteger(0);
		return 4;
		
	}


	// 得到角色的ui模型名字
	INT CGameProduce_Login::GetRoleUIModleName(LuaPlus::LuaState* state)
	{

		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iRoleIndex = args[2].GetInteger();
		
		const TCHAR* pUIModleName = NULL;
		if(CGameProcedure::s_pProcCharSel)
		{
			pUIModleName = CGameProcedure::s_pProcCharSel->GetRoleUIName(iRoleIndex);
		}//	
		
		if(pUIModleName)
		{
			// 名字
			state->PushString(pUIModleName);
		}
		else
		{
			// 名字
			state->PushString("");
		}

		return 1;
	}

	// 创建角色
	INT CGameProduce_Login::CreateRole(LuaPlus::LuaState* state)
	{

		LuaPlus::LuaStack args(state);
		if (!(args[2].IsString()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		
		if(CGameProcedure::s_pProcCharCreate)
		{
			const char* pChar = args[2].GetString();
			int iSex = args[3].GetInteger();
			if(pChar)
			{
				int iStrLen = (int)::strlen(pChar);

				if(iStrLen > 12)
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "角色名过长");
					return 0;
				}

				CGameProcedure::s_pProcCharCreate->m_iSex = iSex;
				strcpy(CGameProcedure::s_pProcCharCreate->m_szCreateRoleName, pChar);
				CGameProcedure::s_pProcCharCreate->CreateRole();
			}
			else
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "角色名不能为空");
			}
		}
		return 0;
	}

	// 删除角色
	INT CGameProduce_Login::DelRole(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if(CGameProcedure::s_pProcCharSel)
		{
			int iRoleIndex = args[2].GetInteger();
			CGameProcedure::s_pProcCharSel->DelRole(iRoleIndex);

			
		}//	

		return 0;
		
	}

	// 删除选择的角色
	INT CGameProduce_Login::DelSelRole(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::s_pProcCharSel)
		{

			CGameProcedure::s_pProcCharSel->DelSelRole();
			
		}//	

		return 0;
		
	}

	// 发送进入游戏消息
	INT CGameProduce_Login::SendEnterGameMsg(LuaPlus::LuaState* state)
	{
		
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if(CGameProcedure::s_pProcCharSel)
		{
			int iRoleIndex = args[2].GetInteger();
			CGameProcedure::s_pProcCharSel->SendEnterGameMsg(iRoleIndex);

			
		}//	

		return 0;
	}

	// 询问是否删除角色
	INT CGameProduce_Login::DelRole_YesNo(LuaPlus::LuaState* state)
	{

		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iRoleIndex = args[2].GetInteger();
		CGameProcedure::s_pProcCharSel->m_iCurSelRoleDel = iRoleIndex;
		std::vector< STRING > strParamInfo;

		if(CGameProcedure::s_pProcCharSel->IsValidIndex(iRoleIndex))
		{
			// 
			std::string strInfo = ""; 
			// 通过索引得到角色的信息
			const DB_CHAR_BASE_INFO* pInfo = CGameProcedure::s_pProcCharSel->GetRoleInfo(iRoleIndex);
			if(pInfo)
			{
				//if(pInfo->m_Level < 9)
				{
					TCHAR szBuf[128];
					_stprintf(szBuf, _T("你确定要将 %d级 的角色#c00ff00 %s #cffffff删除吗?"), pInfo->m_Level, pInfo->m_Name);
					strParamInfo.push_back(szBuf);
					strParamInfo.push_back("1");
					((CEventSystem*)CGameProcedure::s_pEventSystem)->PushEvent( GE_GAMELOGIN_SYSTEM_INFO_YESNO, strParamInfo);
				}
			}
			else
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "请选择一个要删除的角色.");
			}
		}
		else
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "请选择一个要删除的角色.");
		}
		
		return 0;
	}

	// 询问是否退出游戏
	INT CGameProduce_Login::ExitGame_YesNo(LuaPlus::LuaState* state)
	{
		std::vector< STRING > strParamInfo;

		strParamInfo.push_back("确定要退出游戏吗?");
		strParamInfo.push_back("0");
		 
		((CEventSystem*)CGameProcedure::s_pEventSystem)->PushEvent( GE_GAMELOGIN_SYSTEM_INFO_YESNO, strParamInfo);
		return 0;
	}

	// 询问是否退到帐号输入界面
	INT CGameProduce_Login::ExitToAccountInput_YesNo(LuaPlus::LuaState* state)
	{
		std::vector< STRING > strParamInfo;
		strParamInfo.push_back("确定退到帐号输入界面吗?");
		strParamInfo.push_back("2");
		 
		((CEventSystem*)CGameProcedure::s_pEventSystem)->PushEvent( GE_GAMELOGIN_SYSTEM_INFO_YESNO, strParamInfo);
		return 0;
	}

	// 自动选择一个与当前选择的网络供应商匹配的login server
	INT CGameProduce_Login::AutoSelLoginServer(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iIndex = args[2].GetInteger();
		CGameProcedure::s_pProcLogIn->AutoSelLoginServer(iIndex);
   		return 0;
	}

	
	// 得到女主角头像的个数
	INT CGameProduce_Login::GetWomanFaceCount(LuaPlus::LuaState* state)
	{

		LuaPlus::LuaStack args(state);
		
		int iFaceCount = 0;

		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			iFaceCount = CGameProcedure::s_pProcCharCreate->m_FaceMng.GetWomanFaceCount();
		}
			
		// 女主角脸形的个数
		state->PushInteger(iFaceCount);
		return 1;
	}

	// 得到男主角头像的个数
	INT CGameProduce_Login::GetManFaceCount(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		
		int iFaceCount = 0;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			iFaceCount = CGameProcedure::s_pProcCharCreate->m_FaceMng.GetManFaceCount();
		}

		// 男主角脸形的个数
		state->PushInteger(iFaceCount);
		return 1;
	}

	// 得到女主角头像的名称
	INT CGameProduce_Login::GetWomanFaceName(LuaPlus::LuaState* state)
	{

		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// 头像索引
		int iIndex = args[2].GetInteger();
		LPCSTR pImageName = NULL;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			LPCSTR pImageName = CGameProcedure::s_pProcCharCreate->m_FaceMng.GetFaceImageInfo(0, iIndex);
		}

		if(pImageName)
		{
			// 名字
			state->PushString(pImageName);
		}
		else
		{
			// 名字
			state->PushString("");
		}

		return 1;
	}

	// 得到男主角头像的名称
	INT CGameProduce_Login::GetManFaceName(LuaPlus::LuaState* state)
	{

		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// 头像索引
		int iIndex = args[2].GetInteger();
		LPCSTR pImageName = NULL;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			LPCSTR pImageName = CGameProcedure::s_pProcCharCreate->m_FaceMng.GetFaceImageInfo(1, iIndex);
		}

		if(pImageName)
		{
			// 名字
			state->PushString(pImageName);
		}
		else
		{
			// 名字
			state->PushString("");
		}

		return 1;
	}

	// 得到主角头像的名称
	INT CGameProduce_Login::GetFaceName(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// 头像索引
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		LPCSTR pImageName = NULL;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			pImageName = CGameProcedure::s_pProcCharCreate->m_FaceMng.GetFaceImageInfo(iRace, iIndex);
		}

		if(pImageName)
		{
			// 名字
			state->PushString(pImageName);
		}
		else
		{
			// 名字
			state->PushString("");
		}

		return 1;
	}

	// 设置头像id
	INT CGameProduce_Login::SetFaceId(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// 头像索引
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFace))
		{
			CGameProcedure::s_pProcCharCreate->SetFaceByRaceAndIndex(iRace, iIndex);
		}

		return 0;
	}


	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 关于脸形部分.
	//
	
	// 得到脸形model的个数
	INT CGameProduce_Login::GetFaceModelCount(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// 性别
		int iRace  = args[2].GetInteger();

		int iModelCount = 0;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFaceModel))
		{
			iModelCount = CGameProcedure::s_pProcCharCreate->m_FaceModelmng.GetFaceModelCount(iRace);
		}

		state->PushInteger(iModelCount);
		return 1;
	}

	// 得到脸形模型的名字
	INT CGameProduce_Login::GetFaceModelName(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// 头像索引
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		LPCSTR pImageName = NULL;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFaceModel))
		{
			//pImageName = CGameProcedure::s_pProcCharCreate->m_FaceModelmng.GetFaceModel(iRace, iIndex);
			// 2006-4-6
			pImageName = CGameProcedure::s_pProcCharCreate->m_FaceModelmng.GetFaceModelShowName(iRace, iIndex);
		}

		if(pImageName)
		{
			// 名字
			state->PushString(pImageName);
		}
		else
		{
			// 名字
			state->PushString("");
		}
		return 1;
	}

	// 设置脸形模型id
	INT CGameProduce_Login::SetFaceModelId(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// 头像索引
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFaceModel))
		{
			CGameProcedure::s_pProcCharCreate->SetFaceModelByRaceAndIndex(iRace, iIndex);
		}

		return 0;
	}



	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 关于发形部分.
	//
	
	// 得到脸形model的个数
	INT CGameProduce_Login::GetHairModelCount(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// 性别
		int iRace  = args[2].GetInteger();

		int iModelCount = 0;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadHairModel))
		{
			iModelCount = CGameProcedure::s_pProcCharCreate->m_HairModelMng.GetHairModelCount(iRace);
		}

		state->PushInteger(iModelCount);
		return 1;
	}

	// 得到脸形模型的名字
	INT CGameProduce_Login::GetHairModelName(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// 头像索引
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		LPCSTR pImageName = NULL;
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadHairModel))
		{
			//pImageName = CGameProcedure::s_pProcCharCreate->m_HairModelMng.GetHairModel(iRace, iIndex);
			//2006-4-6
			pImageName = CGameProcedure::s_pProcCharCreate->m_HairModelMng.GetHairModelShowName(iRace, iIndex);
		}

		if(pImageName)
		{
			// 名字
			state->PushString(pImageName);
		}
		else
		{
			// 名字
			state->PushString("");
		}

		return 1;
	}

	// 设置脸形模型id
	INT CGameProduce_Login::SetHairModelId(LuaPlus::LuaState* state)
	{

		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		// 头像索引
		int iRace  = args[2].GetInteger();
		int iIndex = args[3].GetInteger();

		
		if((CGameProcedure::s_pProcCharCreate)&&(CGameProcedure::s_pProcCharCreate->m_iIsLoadFaceModel))
		{
			CGameProcedure::s_pProcCharCreate->SetHairModelByRaceAndIndex(iRace, iIndex);
		}

		return 0;
	}

	// 得到玩家阵营
	INT CGameProduce_Login::GetPlayerCamp(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		state->PushInteger( CGameProcedure::s_pProcCharCreate->m_iCamp );

		return 1;
	}

	INT CGameProduce_Login::SetPlayerCamp(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iCamp  = args[2].GetInteger();
		if( iCamp != CAMP1_PLAYER && iCamp != CAMP2_PLAYER )
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		CGameProcedure::s_pProcCharCreate->m_iCamp = iCamp;

		return 0;
	}

	INT CGameProduce_Login::SetPlayerMenpai(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		int iMenPai  = args[2].GetInteger();
		if( iMenPai != MATTRIBUTE_SHAOLIN && 
			iMenPai != MATTRIBUTE_WUDANG &&
			iMenPai != MATTRIBUTE_EMEI )
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		CGameProcedure::s_pProcCharCreate->m_iMenpai = iMenPai;

		return 0;
	}

	// 关闭网络连接。2006－4－12
	INT CGameProduce_Login::CloseNetConnect(LuaPlus::LuaState* state)
	{

		CNetManager::GetMe()->Close();

		if(CGameProcedure::s_pProcCharSel)
		{
			CGameProcedure::s_pProcCharSel->ChangeToAccountInput();
		}
		return 0;
	}
}