
#include "stdAfx.h"
#include "GWCityClose.h"


uint GWCityCloseHandler::Execute( GWCityClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
