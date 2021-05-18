#include "StdAfx.h"
#include "LCRetCreateChar.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\procedure\GamePro_CharCreate.h"

UINT	LCRetCreateCharHandler::Execute(LCRetCreateChar* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

			
		if(pPacket)
		{
			ASKCREATECHAR_RESULT	res = 	pPacket->GetResult();

			switch(res)
			{
			case ASKCREATECHAR_SUCCESS:			//成功
				{
					//CGameProcedure::s_pProcCharCreate->ChangeToRoleSel();

					CGameProcedure::s_pProcCharCreate->m_Status = CGamePro_CharCreate::CHAR_CREATE_CREATE_OK;
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CREATE_CLEAR_NAME );
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CREATE_ROLE_OK );
					break;
				}
			case ASKCREATECHAR_SERVER_BUSY:		//服务器忙，重试
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "服务器忙，重试! ");	
					break;
				}
			case ASKCREATECHAR_OP_TIMES:		//操作过于频繁
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "再次创建一个角色需要15秒钟！");
					break;
				}
			case ASKCREATECHAR_FULL:			//角色已经满了
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "角色已经满了! ");	
					break;
				}
			case ASKCREATECHAR_OP_ERROR:		//错误操作流程
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "错误操作流程! ");	
					break;
				}
			case ASKCREATECHAR_INTERNAL_ERROR:  //内部错误
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "内部错误! ");	
					break;
				}
			case ASKCREATECHAR_SAME_NAME:
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "角色名字已经存在");
					break;
				}
			case ASKCREATECHAR_INVALID_NAME:
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "角色名错误");
					break;
				}
			default:							// 未知错误
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