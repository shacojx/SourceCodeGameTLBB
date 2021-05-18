#include "StdAfx.h"
#include "CGDiscardEquip.h"

uint CGDiscardEquipHandler::Execute(CGDiscardEquip* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}