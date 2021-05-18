#include "StdAfx.h"
#include "LCRetLogin.h"
#include "Type.h"
#include "CLAskCharList.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\procedure\Gameprocedure.h"
#include "..\..\procedure\GamePro_Login.h"

UINT	LCRetLoginHandler::Execute(LCRetLogin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	
	if((pPacket)&&(pPacket->GetResult() == LOGINR_SUCCESS))
	{
		
		//CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);
		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, "等待进入游戏...");
		
	}
	else
	{
		int iLoginRes = pPacket->GetResult();
		switch(iLoginRes)
		{
			
		case LOGINR_AUTH_FAIL:			//验证失败
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "验证用户名密码错误!");
				break;
			}

		//case LOGINR_VERSION_FAIL:		//版本错误
		//	{
		//		CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "版本错误!");
		//		break;
		//	}//
		case LOGINR_NOT_EN_POINT:		//点数不够
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "点数不够!");
				break;
			}
		case LOGINR_STOP_SERVICE:		//暂时停止服务
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "服务器停止服务!");
				break;
			}
		default:
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "未知错误!");
				break;
			}

		}

		CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_ACCOUNT_BEGIN_REQUESTING);
		
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}