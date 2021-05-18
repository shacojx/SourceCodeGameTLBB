#include "StdAfx.h"
#include "GCOtherAttack.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\WxRender\RenderSystem.h"

uint GCOtherAttackHandler::Execute( GCOtherAttack* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
//	AxTrace(0, 2, "GCOtherAttackHandler");
//	CGameProcedure::s_pGfxSystem->PushDebugString("GCOtherAttackHandler");
	//COpe_PhysicalAttack::DoFromServer(pPacket);

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
