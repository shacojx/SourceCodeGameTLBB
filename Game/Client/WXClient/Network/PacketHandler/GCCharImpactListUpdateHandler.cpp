#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\GameCommand.h"
#include "..\..\Global.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\DataPool\TeamOrGroup.h"
#include "..\..\DataPool\GMUIDataPool.h"

#include "GCCharImpactListUpdate.h"
#include "..\..\Object\GMObjectLoadQueue.h"


uint GCCharImpactListUpdateHandler::Execute( GCCharImpactListUpdate* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->GetOwnerID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		////如果需要更新资源，尝试放入缓冲队列
		//if(pObjectManager->GetLoadQueue()->TryAddLoadTask(pObj->GetID(), pPacket))
		//	return PACKET_EXE_NOTREMOVE;

		const GCCharImpactListUpdate::ImpactStruct_T *pImpactList = pPacket->GetImpactList();

		if(g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		{
			CObject_Character *pCharacter = (CObject_Character*)pObj;
			pCharacter->RemoveAllImpact();
		}

		SimpleImpactList ImpactList;

		SHORT i;
		for ( i = 0; i < pPacket->NumOfImpacts(); i++ )
		{
			SCommand_Object cmdTemp;
			cmdTemp.m_wID			= OC_UPDATE_IMPACT_EX;
			cmdTemp.m_adwParam[0]	= pImpactList[i].m_nBuffID;
			pObj->PushCommand(&cmdTemp );

			ImpactList.AddImpact(pImpactList[i].m_nBuffID);
		}

		CTeamOrGroup* pTeam = CUIDataPool::GetMe()->GetTeamOrGroup();
		if ( pTeam != NULL )
		{
			pTeam->UpdateImpactsList( pPacket->GetOwnerID(), &ImpactList );
		}

		pObj->PushDebugString("GCCharImpactListUpdate");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
