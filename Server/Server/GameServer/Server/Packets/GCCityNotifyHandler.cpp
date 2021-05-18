#include "stdafx.h"



#include "GCCityNotify.h"


uint GCCityNotifyHandler::Execute( GCCityNotify* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
