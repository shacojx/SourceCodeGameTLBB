#include "StdAfx.h"


#include "CGChannelKick.h"




uint CGChannelKickHandler::Execute( CGChannelKick* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
