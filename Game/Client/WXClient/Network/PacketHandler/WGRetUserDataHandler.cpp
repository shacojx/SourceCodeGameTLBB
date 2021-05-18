#include "StdAfx.h"
#include "WGRetUserData.h"




uint WGRetUserDataHandler::Execute( WGRetUserData* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
