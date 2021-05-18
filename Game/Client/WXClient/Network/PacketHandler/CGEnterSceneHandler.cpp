#include "StdAfx.h"
#include "CGEnterScene.h"


uint CGEnterSceneHandler::Execute( CGEnterScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
