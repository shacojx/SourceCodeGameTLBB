#include "Stdafx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"

#include "GCDetailImpactListUpdate.h"


uint GCDetailImpactListUpdateHandler::Execute( GCDetailImpactListUpdate* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//加入UI数据池
		CDataPool* pDataPool = CDataPool::GetMe();

		pDataPool->BuffImpact_RemoveAll();

		const GCDetailImpactListUpdate::ImpactStruct_T *pImpactList = pPacket->GetImpactList();

		SHORT i;
		for ( i = 0; i < pPacket->NumOfImpacts(); i++ )
		{
			_BUFF_IMPACT_INFO infoBuffImpact;
			infoBuffImpact.m_nReceiverID			= pPacket->GetOwnerID();
			infoBuffImpact.m_nSenderID				= pImpactList[i].m_nSenderID;
			infoBuffImpact.m_nBuffID				= pImpactList[i].m_nBuffID;
			infoBuffImpact.m_nSkillID				= pImpactList[i].m_nSkillID;
			infoBuffImpact.m_nSenderLogicCount		= -1;
			infoBuffImpact.m_nSN					= pImpactList[i].m_nSN;
			infoBuffImpact.m_nTimer					= pImpactList[i].m_nContinuance;

			pDataPool->BuffImpact_Add( &infoBuffImpact );
		}
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
