#include "StdAfx.h"
#include "GCErrorAttack.h"
#include "..\..\Procedure\GameProcedure.h"


uint GCErrorAttackHandler::Execute( GCErrorAttack* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
//	AxTrace(0, 2, "GCErrorAttackHandler");
	//COpe_PhysicalAttack::DoFromServer(pPacket);
	
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
