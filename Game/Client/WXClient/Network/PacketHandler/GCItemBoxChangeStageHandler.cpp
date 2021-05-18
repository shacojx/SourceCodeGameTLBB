#include "StdAfx.h"
#include "GCItemBoxChangeStage.h"

uint GCItemBoxChangeStageHandler::Execute(GCItemBoxChangeStage* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION
		return PACKET_EXE_ERROR;
}