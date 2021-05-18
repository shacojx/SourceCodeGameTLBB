#include "StdAfx.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"

#include "GCTeamFollowErr.h"

const char* pErrString[] = {
	"Too_Far_To_Leader",
	"Can_Not_Move_Self",
	"Stall_Can_Not_Follow_Team",
	"In_Follow"
};

uint GCTeamFollowErrHandler::Execute( GCTeamFollowErr* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CHAR *pErrContent = new CHAR[MAX_PATH];

		switch( pPacket->GetError() )
		{
		case TF_ERROR_TOO_FAR:
		case TF_ERROR_IN_TEAM_FOLLOW:
		case TF_ERROR_STALL_OPEN:
		case TF_ERROR_NOT_IN_FOLLOW_MODE:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC(pErrString[pPacket->GetError()]);
				_snprintf(pErrContent, MAX_PATH, strTemp.c_str());
				//_snprintf(pErrContent, MAX_PATH, pErrString[pPacket->GetError()]);
			}
			break;
		default:
			return PACKET_EXE_CONTINUE;
		}

		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, pErrContent );
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
