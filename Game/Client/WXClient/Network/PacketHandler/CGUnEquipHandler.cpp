#include "StdAfx.h"
#include "CGUnEquip.h"

uint CGUnEquipHandler::Execute( CGUnEquip* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}