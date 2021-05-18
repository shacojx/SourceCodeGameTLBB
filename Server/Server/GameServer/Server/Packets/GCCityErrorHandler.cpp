#include "stdafx.h"



#include "GCCityError.h"


uint GCCityErrorHandler::Execute( GCCityError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
