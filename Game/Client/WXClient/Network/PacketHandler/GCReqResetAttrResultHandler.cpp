#include "StdAfx.h"
#include "GCReqResetAttrResult.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\WxRender\RenderSystem.h"


uint GCReqResetAttrResultHandler::Execute(GCReqResetAttrResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

	//AxTrace(0, 2, "GCReqResetAttrResultHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		RATTR_RESULT ret = pPacket->GetResult();

		switch(ret)
		{
		case RATTR_SUCCESS:
			break;

		case RATTR_NO_POINT:
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("No Point!");
			}
			break;

		case RATTR_NO_LEVEL:
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("No Level!");
			}
			break;
		}
	}
	
	return	PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION
	return	PACKET_EXE_ERROR;
}