#include "stdafx.h"
#include "BWValidateUser.h"
UINT BWValidateUserHandler::Execute( BWValidateUser* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
