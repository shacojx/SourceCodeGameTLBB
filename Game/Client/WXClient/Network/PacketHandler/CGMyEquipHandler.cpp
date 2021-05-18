#include "StdAfx.h"
#include "CGMyEquip.h"

uint CGMyEquipHandler :: Execute( CGMyEquip* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;
__LEAVE_FUNCTION
	return PACKET_EXE_ERROR ;
}

