#include "StdAfx.h"
#include "CGLeaveScene.h"

uint CGLeaveSceneHandler::Execute( CGLeaveScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
