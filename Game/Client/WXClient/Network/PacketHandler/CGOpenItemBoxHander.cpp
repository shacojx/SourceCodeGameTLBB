#include "StdAfx.h"
#include "CGOpenItemBox.h"


using namespace Packets;


uint CGOpenItemBoxHandler::Execute(CGOpenItemBox* pPacket, Player* pPlayer )
{

	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;

}