#include "StdAfx.h"

#include "GGSceneNotify.h"

using namespace Packets;

uint GGSceneNotifyHandler::Execute( GGSceneNotify* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
