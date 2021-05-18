
#include "stdAfx.h"
#include "WGCityCloseSuccess.h"


uint WGCityCloseSuccessHandler::Execute( WGCityCloseSuccess* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
