#include "StdAfx.h"
#include "CGQueryItemBoxStage.h"

uint CGQueryItemBoxStageHandler::Execute(CGQueryItemBoxStage* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
	
		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION
		return PACKET_EXE_ERROR;
}