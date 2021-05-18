#include "StdAfx.h"
#include "CGReqResetAttr.h"


uint	CGReqResetAttrHandler::Execute(CGReqResetAttr* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}