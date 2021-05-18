#include "StdAfx.h"
#include "GCReturnTeamFollow.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\TeamOrGroup.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"

#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "TDUISystem.h"

uint GCReturnTeamFollowHandler::Execute( GCReturnTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pDataPool);
		CTeamOrGroup* team = pDataPool->GetTeamOrGroup();
		TeamMemberInfo* pTMInfo = team->GetMember( pPacket->GetGUID() );

		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
		STRING strTemp = "";

		if( pTMInfo == NULL )
		{
			Assert(FALSE && "没有发现如上队员。");
			return PACKET_EXE_CONTINUE;
		}

		CObject_PlayerMySelf *pMe = CObjectManager::GetMe()->GetMySelf();
		CHAR szText[_MAX_PATH];

		switch( pPacket->GetReturn() )
		{
		case TF_RESULT_REFUSE_FOLLOW:
			{
				_snprintf(szText, _MAX_PATH, "%s拒绝跟随。", pTMInfo->m_szNick);
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );
			}
			break;
		case TF_RESULT_ENTER_FOLLOW:
			{
				GUID_t myGUID = pMe->GetServerGUID();

				if( pPacket->GetGUID() == myGUID )
				{
					SCommand_Object cmdTemp;

					cmdTemp.m_wID				= OC_UPDATE_TEAM_FOLLOW_FLAG;
					cmdTemp.m_abParam[0]		= TRUE;

					pMe->PushCommand( &cmdTemp );

					//msg
					if(bLog)
					{
						TeamMemberInfo* pTMInfo = team->GetMember(team->GetLeaderGUID());
						strTemp = COLORMSGFUNC("TEAM_FOLLOW_OTHER", pTMInfo->m_szNick);
						ADDTALKMSG(strTemp);
						strTemp = NOCOLORMSGFUNC("TEAM_FOLLOW_OTHER", pTMInfo->m_szNick);
						ADDNEWDEBUGMSG(strTemp);
					}
				}
				else
				{
					_snprintf(szText, _MAX_PATH, "%s加入跟随队列。", pTMInfo->m_szNick);
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );
				}
			}
			break;
		case TF_RESULT_STOP_FOLLOW:
			{
				GUID_t myGUID = pMe->GetServerGUID();

				if( pPacket->GetGUID() == myGUID )
				{
					SCommand_Object cmdTemp;

					cmdTemp.m_wID				= OC_UPDATE_TEAM_FOLLOW_FLAG;
					cmdTemp.m_abParam[0]		= FALSE;

					pMe->PushCommand( &cmdTemp );

					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "退出组队跟随状态。" );
				}
				else
				{
					_snprintf(szText, _MAX_PATH, "%s退出跟随队列。", pTMInfo->m_szNick);
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );
				}
			}
			break;
		case TF_RESULT_FOLLOW_FLAG:
			{
				GUID_t myGUID = pMe->GetServerGUID();

				if( pPacket->GetGUID() == myGUID )
				{
					SCommand_Object cmdTemp;

					cmdTemp.m_wID				= OC_UPDATE_TEAM_FOLLOW_FLAG;
					cmdTemp.m_abParam[0]		= TRUE;

					pMe->PushCommand( &cmdTemp );
				}
			}
			break;
		default:
			{
				Assert(FALSE && "不明返回类型。");
				return PACKET_EXE_CONTINUE;
			}
		}

	//	CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "GCReturnTeamFollow Received." );
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}





















