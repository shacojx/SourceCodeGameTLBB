// GCPlayerReliveHandler.cpp

#include "StdAfx.h"
#include "TDDEbuger.h"
#include "TDEventDefine.h"
#include "..\WxRender\RenderSystem.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Global.h"
#include "..\..\DataPool\GMUIDataPool.h"

#include "GCPlayerRelive.h"

using namespace Packets;

uint GCPlayerReliveHandler::Execute( GCPlayerRelive* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CEventSystem::GetMe()->PushEvent( GE_RELIVE_HIDE );
		CUIDataPool::GetMe()->EndOutGhostTimer( );

		//CGameProcedure::s_pGfxSystem->Scene_SetPostFilter_Death(FALSE);
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
