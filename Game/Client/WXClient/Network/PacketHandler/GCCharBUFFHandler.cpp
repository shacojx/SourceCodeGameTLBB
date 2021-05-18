#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\Object\GMObjectLoadQueue.h"
#include "GCCharBuff.h"
#include "..\..\DataPool\TeamOrGroup.h"
#include "..\..\DataPool\GMUIDataPool.h"

namespace Packets
{
	uint GCCharBuffHandler::Execute( GCCharBuff* pPacket, Player* pPlayer )
	{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->GetReceiverID() ));
			if ( pObj == NULL )
				return PACKET_EXE_CONTINUE;

			////如果需要更新资源，尝试放入缓冲队列
			//if(pObjectManager->GetLoadQueue()->TryAddLoadTask(pObj->GetID(), pPacket))
			//	return PACKET_EXE_NOTREMOVE;

			SCommand_Object cmdTemp;
			cmdTemp.m_wID			= OC_UPDATE_IMPACT;
			cmdTemp.m_adwParam[0]	= pPacket->GetBuffID();
			cmdTemp.m_abParam[1]	= (pPacket->GetEnable())?(TRUE):(FALSE);
			cmdTemp.m_anParam[2]	= pPacket->GetSenderID();
			pObj->PushCommand(&cmdTemp );

			CTeamOrGroup* pTeam = CUIDataPool::GetMe()->GetTeamOrGroup();
			if ( pTeam != NULL )
			{
				if ( pPacket->GetEnable() )
				{
					pTeam->AddImpact( pPacket->GetReceiverID(), pPacket->GetBuffID() );
				}
				else
				{
					pTeam->RemoveImpact( pPacket->GetReceiverID(), pPacket->GetBuffID() );
				}
			}

			pObj->PushDebugString("GCCharBuff");
			pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
	}
}
