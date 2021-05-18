// GCPlayerDieHandler.cpp

#include "StdAfx.h"
#include "TDEventDefine.h"
#include "..\WxRender\RenderSystem.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMUIDataPool.h"

#include "GCPlayerDie.h"

using namespace Packets;

uint GCPlayerDieHandler::Execute( GCPlayerDie* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		INT nTime = (INT)(pPacket->GetTime());

		CHAR szText[32];
		_snprintf(szText,32,"%d",(nTime/1000));

		CEventSystem::GetMe()->PushEvent( GE_RELIVE_SHOW, ((pPacket->IsCanRelive())?("1"):("0")), szText, -1 );
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
