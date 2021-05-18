// CGPlayerDieResultHandler.cpp

#include "StdAfx.h"

#include "CGPlayerDieResult.h"

using namespace Packets;

uint CGPlayerDieResultHandler::Execute( CGPlayerDieResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
