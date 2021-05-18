#include "StdAfx.h"


#include "WGChannelResult.h"




uint WGChannelResultHandler::Execute( WGChannelResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
