#include "StdAfx.h"


#include "CGChannelCreate.h"




uint CGChannelCreateHandler::Execute( CGChannelCreate* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
