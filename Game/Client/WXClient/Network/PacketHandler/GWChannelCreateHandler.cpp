#include "StdAfx.h"


#include "GWChannelCreate.h"




uint GWChannelCreateHandler::Execute( GWChannelCreate* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
