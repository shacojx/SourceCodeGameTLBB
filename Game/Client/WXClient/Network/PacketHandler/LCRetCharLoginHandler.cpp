#include "StdAfx.h"
#include "LCRetCharLogin.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\procedure\GamePro_CharSel.h"
#include "..\..\procedure\GamePro_CharCreate.h"
#include "..\..\procedure\Gamepro_ChangeScene.h"
#include "TDVariable.h"
UINT LCRetCharLoginHandler::Execute(LCRetCharLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


		if(pPacket)
		{
			ASKCHARLOGIN_RESULT res;
			res = pPacket->GetResult();
			switch(res)
			{
			case ASKCHARLOGIN_SUCCESS:		//成功
				{
					UINT	iPort	  = pPacket->GetServerPort();
					CHAR*	pServerIP = pPacket->GetServerIP();
					UINT	iUserKey  = pPacket->GetUserKey();

					// 记录服务器ip地址在全局变量
					CGameProcedure::s_pVariableSystem->SetVariable("GameServer_Address", pServerIP);

					// 记录服务器端口号在全局变量
					CGameProcedure::s_pVariableSystem->SetAs_Int("GameServer_Port", iPort);
					
					// 记录进入游戏角色的guid
					CGameProcedure::s_pVariableSystem->SetAs_Int("User_GUID", CGameProcedure::s_pProcCharSel->m_EnterGameGUID);

					// 进入场景的user key
					CGameProcedure::s_pVariableSystem->SetAs_Int("GameServer_Key", iUserKey);

					// 切换到服务器连接流程
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLEAR_ACCOUNT );
					CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcChangeScene);
					CGameProcedure::s_pProcChangeScene->SetStatus(CGamePro_ChangeScene::CHANGESCENE_DISCONNECT);

					break;
				}
			case ASKCHARLIST_WORLD_FULL:		//世界满了
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "服务器满了!");	
					break;
				}
			case ASKCHARLOGIN_SERVER_BUSY:	//服务器忙，重试
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "服务器忙，重试! ");	
					break;
				}
			case ASKCHARLOGIN_LOADDB_ERROR:	//角色载入出错
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "角色载入出错! ");	
					break;
				}
			case ASKCHARLOGIN_OP_TIMES:		//操作过于频繁
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "操作过于频繁!");	
					break;
				}
			case ASKCHARLOGIN_NOT_OWNER:	//不是角色的所有者
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "不是角色的所有者! ");	
					break;
				}
			case ASKCHARLOGIN_SERVER_STOP:	//服务器维护
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "服务器维护! ");	
					break;
				}
			case ASKCHARLOGIN_CHANGE_SCENE:	//切换场景
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "角色正切换场景! ");	
					break;
				}
			default:
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "未知错误! ");	
					break;
				}
			}
		}
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
