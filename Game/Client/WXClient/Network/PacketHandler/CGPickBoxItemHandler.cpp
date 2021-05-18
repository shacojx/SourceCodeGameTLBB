#include "StdAfx.h"
#include "CGPickBoxItem.h"


using namespace Packets;

uint CGPickBoxItemHandler::Execute(CGPickBoxItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}