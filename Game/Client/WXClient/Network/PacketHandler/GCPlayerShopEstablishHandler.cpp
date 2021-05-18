
#include "StdAfx.h"
#include "GCPlayerShopEstablish.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"

UINT GCPlayerShopEstablishHandler::Execute( GCPlayerShopEstablish* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CHAR resultMsg[256] = {0};
			_snprintf(resultMsg, 256, "开张大吉，你的店铺成功创建了。", pPacket->GetShopName());
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, resultMsg);
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
