#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"

#include "GCDetailBuff.h"


namespace Packets
{
	uint GCDetailBuffHandler::Execute( GCDetailBuff* pPacket, Player* pPlayer )
	{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			//加入UI数据池
			CDataPool* pDataPool = CDataPool::GetMe();
			if ( pPacket->GetEnable() )
			{
				_BUFF_IMPACT_INFO infoBuffImpact;
				infoBuffImpact.m_nReceiverID			= pPacket->GetReceiverID();
				infoBuffImpact.m_nSenderID				= pPacket->GetSenderID();
				infoBuffImpact.m_nBuffID				= pPacket->GetBuffID();
				infoBuffImpact.m_nSkillID				= pPacket->GetSkillID();
				infoBuffImpact.m_nSenderLogicCount		= pPacket->GetSenderLogicCount();
				infoBuffImpact.m_nSN					= pPacket->GetSN();
				infoBuffImpact.m_nTimer					= pPacket->GetContinuance();
				pDataPool->BuffImpact_Add( &infoBuffImpact );
			}
			else
			{
				pDataPool->BuffImpact_Remove( pPacket->GetSN() );
			}
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
	}
}
