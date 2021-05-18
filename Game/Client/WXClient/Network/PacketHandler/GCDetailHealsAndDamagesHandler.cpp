#include "stdafx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\Object\Character\Obj_Character.h"

#include "GCDetailHealsAndDamages.h"

namespace Packets
{
	uint GCDetailHealsAndDamagesHandler::Execute( GCDetailHealsAndDamages* pPacket, Player* pPlayer )
	{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			CObject* pObj = (CObject*)(pObjectManager->FindServerObject( pPacket->GetReceiverID() ));
			if ( pObj != NULL )
			{
				_DAMAGE_INFO infoDamage;
                infoDamage.m_nSkillID				= INVALID_ID;
				infoDamage.m_nTargetID				= pPacket->GetReceiverID();
				infoDamage.m_nSenderID				= pPacket->GetSenderID();
				infoDamage.m_nSenderLogicCount		= pPacket->GetSenderLogicCount();
				infoDamage.m_nImpactID				= INVALID_ID;
				infoDamage.m_nType					= infoDamage.TYPE_HEAL_AND_DAMAGE;
				if(pPacket->IsHpModificationDirty())
				{
					infoDamage.m_bHealthDirty = TRUE;
					infoDamage.m_nHealthIncrement = pPacket->GetHpModification();
				}
				if(pPacket->IsMpModificationDirty())
				{
					infoDamage.m_bManaDirty = TRUE;
					infoDamage.m_nManaIncrement = pPacket->GetMpModification();
				}
				if(pPacket->IsRageModificationDirty())
				{
					infoDamage.m_bRageDirty = TRUE;
					infoDamage.m_nRageIncrement = pPacket->GetRageModification();
				}
				if(pPacket->IsStrikePointModificationDirty())
				{
					infoDamage.m_bStrikePointDirty = TRUE;
					infoDamage.m_nStrikePointIncrement = pPacket->GetStrikePointModification();
				}
				infoDamage.m_bIsCriticalHit = pPacket->IsCriticalHit();

				_LOGIC_EVENT logicEvent;
				logicEvent.Init(pPacket->GetSenderID(), pPacket->GetSenderLogicCount(), &infoDamage);

				SCommand_Object cmdTemp;
				cmdTemp.m_wID			= OC_LOGIC_EVENT;
				cmdTemp.m_apParam[0]	= (VOID*)(&logicEvent);
				pObj->PushCommand(&cmdTemp );

				pObj->PushDebugString("GCCharDirectImpactGCDetailHealsAndDamages");
				pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
			}
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
	}
}
