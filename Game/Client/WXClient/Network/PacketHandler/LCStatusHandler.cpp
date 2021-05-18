#include "StdAfx.h"
#include "LCStatus.h"
#include "Type.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\procedure\Gameprocedure.h"
#include "..\..\procedure\GamePro_Login.h"

UINT LCStatusHandler::Execute(LCStatus* pPacket, Player* pPlayer )
{
	
		__ENTER_FUNCTION

		if(NULL == pPacket)
		{
			return PACKET_EXE_CONTINUE;
		}

		CLIENT_TURN_STATUS res = pPacket->GetClientStatus();
		switch(res)
		{
			case CTS_NORMAL:
				{
					CGameProcedure::s_pProcLogIn->ChangeToCharacterSel();
					break;
				}
			case CTS_TURN:
				{
					int		iTurn = pPacket->GetTurnNumber();
					TCHAR	bufInfo[1024];
					
					memset(bufInfo, 0, sizeof(bufInfo));
					_stprintf(bufInfo, "服务器已满,你被排在 %d 名", iTurn);
					CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON, bufInfo);
					break;
				}
		}
	
		return PACKET_EXE_CONTINUE ;

		__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}