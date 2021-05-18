#include "StdAfx.h"
#include "CGUseEquip.h"

using namespace Packets;

uint CGUseEquipHandler::Execute(CGUseEquip* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}