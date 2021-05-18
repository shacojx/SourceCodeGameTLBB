
#include "stdAfx.h"
#include "GWCityCloseSuccess.h"


uint GWCityCloseSuccessHandler::Execute( GWCityCloseSuccess* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
