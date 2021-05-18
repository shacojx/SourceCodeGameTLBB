// CGPlayerCallOfResultHandler.cpp

#include "StdAfx.h"

#include "CGPlayerCallOfResult.h"

using namespace Packets;

uint CGPlayerCallOfResultHandler::Execute( CGPlayerCallOfResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
