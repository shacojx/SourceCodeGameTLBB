#include "StdAfx.h"

#include "GCTeamError.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\Datapool\GMDP_CharacterData.h"
#include "TDException.h"

const char* pString[] = {
	"Target_Join_Team",
	"Target_Reject_Invite",
	"You_Team_Is_Full",
	"Leader_Reject_Inviter_Join",
	"Leader_Can_Dismiss_Team",
	"Leader_Can_Kick_Member",
	"Target_Join_Other_Team",
	"Target_No_Team",
	"Leader_Reject_Apply",
	"Target_Team_Is_Full",
	"Leader_Change",
	"You_Are_Not_Leader_Can_Not_Appoint",
	"Can_Not_Appoint_Not_Member",
	"Can_Not_Appoint_Member_Leave",
	"Can_Not_Appoint_You_Not_Leader",
	"Target_Leader_Not_Respone",
	"Invitor_Not_In_Team",
	"You_Leave_Old_Then_Join_New",
	"Team_Is_Full",
	"Target_Set_Refuse_Invite",
	"Target_Is_Not_Online",
};

uint GCTeamErrorHandler::Execute( GCTeamError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CHAR *pName = new CHAR[MAX_CHARACTER_NAME];
		strncpy(pName, CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name(), MAX_CHARACTER_NAME-1);
		CHAR *pContex= new CHAR[MAX_PATH];

		switch( pPacket->GetErrorCode() )
		{
		case TEAM_ERROR_INVITEDESTHASTEAM :
		case TEAM_ERROR_INVITEREFUSE :
		case TEAM_ERROR_INVITETEAMFULL :
		case TEAM_ERROR_INVITELEADERREFUSE :
		case TEAM_ERROR_DISMISSNOTLEADER :
		case TEAM_ERROR_KICKNOTLEADER :
		case TEAM_ERROR_APPLYSOURHASTEAM :
		case TEAM_ERROR_APPLYDESTHASNOTTEAM :
		case TEAM_ERROR_APPLYLEADERREFUSE :
		case TEAM_ERROR_APPLYTEAMFULL :
		case TEAM_ERROR_APPLYLEADERGUIDERROR :
		case TEAM_ERROR_APPOINTSOURNOTEAM :
		case TEAM_ERROR_APPOINTDESTNOTEAM :
		case TEAM_ERROR_APPOINTNOTSAMETEAM :
		case TEAM_ERROR_APPOINTSOURNOLEADER :
		case TEAM_ERROR_APPLYLEADERCANTANSWER:
		case TEAM_ERROR_INVITERNOTINTEAM:
		case TEAM_ERROR_APPLYWHENINTEAM:
		case TEAM_ERROR_TEAMFULL:
		case TEAM_ERROR_REFUSEINVITESETTING:
		case TEAM_ERROR_TARGETNOTONLINE:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC(pString[ pPacket->GetErrorCode() ]);
				strncpy(pContex, strTemp.c_str(), MAX_PATH-1);
				//strncpy(pContex, pString[ pPacket->GetErrorCode() ], MAX_PATH-1);
			}
			break;
		default:
			TDAssert(FALSE);
			return PACKET_EXE_CONTINUE;
		}

		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, pContex );

//		CGameProcedure::s_pEventSystem->PushEvent( GE_CHAT_MESSAGE, pName, pContex, 0 );
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
