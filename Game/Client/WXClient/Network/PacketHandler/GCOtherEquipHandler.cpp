#include "StdAfx.h"
#include "GCOtherEquip.h"

uint GCOtherEquipHandler :: Execute( GCOtherEquip* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION
	//AxTrace(0, 2, "GCOtherEquipHandler::Execute");

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

