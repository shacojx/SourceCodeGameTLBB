#include "StdAfx.h"
#include "CGOtherEquip.h"

uint CGOtherEquipHandler :: Execute( CGOtherEquip* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

