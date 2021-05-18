#include "StdAfx.h"
#include "GCRemoveGemResult.h"


uint GCRemoveGemResultHandler::Execute(GCRemoveGemResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
		
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}