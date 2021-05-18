
#include "StdAfx.h"
#include "CGAskMissionList.h"



using namespace Packets;


uint CGAskMissionListHandler::Execute( CGAskMissionList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
