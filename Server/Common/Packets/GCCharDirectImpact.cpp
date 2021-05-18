#include "stdafx.h"

// GCCharDirectImpact.cpp
// 
/////////////////////////////////////////////////////

#include "GCCharDirectImpact.h"


namespace Packets
{
	BOOL GCCharDirectImpact::Read( SocketInputStream& iStream ) 
	{
	__ENTER_FUNCTION
		iStream.Read((CHAR *)&m_nReceiverID, sizeof(m_nReceiverID));// 目标对象的ID
		iStream.Read((CHAR *)&m_nSenderID, sizeof(m_nSenderID));// 技能发起者的ID
		iStream.Read((CHAR *)&m_nSenderLogicCount, sizeof(m_nSenderLogicCount));// 攻击者的逻辑计数
		iStream.Read((CHAR *)&m_nImpactID, sizeof(m_nImpactID));// 效果ID //参考GameStruct_Impact.h的DIRECT_IMPACT_SEID
		iStream.Read((CHAR *)&m_nSkillID, sizeof(m_nSkillID));// 技能ID
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

	BOOL GCCharDirectImpact::Write( SocketOutputStream& oStream )const
	{
	__ENTER_FUNCTION

		oStream.Write((CHAR const*)&m_nReceiverID, sizeof(m_nReceiverID));// 目标对象的ID
		oStream.Write((CHAR const*)&m_nSenderID, sizeof(m_nSenderID));// 技能发起者的ID
		oStream.Write((CHAR const*)&m_nSenderLogicCount, sizeof(m_nSenderLogicCount));// 攻击者的逻辑计数
		oStream.Write((CHAR const*)&m_nImpactID, sizeof(m_nImpactID));// 效果ID //参考GameStruct_Impact.h的DIRECT_IMPACT_SEID
		oStream.Write((CHAR const*)&m_nSkillID, sizeof(m_nSkillID));// 技能ID
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

	UINT GCCharDirectImpact::Execute( Player* pPlayer )
	{
	__ENTER_FUNCTION

		return GCCharDirectImpactHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
	}
}
