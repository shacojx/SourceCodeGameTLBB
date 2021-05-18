#include "stdafx.h"

#include "GCDetailBUFF.h"

namespace Packets
{
	BOOL GCDetailBuff::Read( SocketInputStream& iStream ) 
	{
	__ENTER_FUNCTION
		iStream.Read((CHAR*)&m_nSN, sizeof(m_nSN));
		iStream.Read((CHAR*)&m_bEnable, sizeof(m_bEnable));
		if ( m_bEnable )
		{
			iStream.Read((CHAR*)&m_nReceiverID, sizeof(m_nReceiverID));
			iStream.Read((CHAR*)&m_nSenderID, sizeof(m_nSenderID));
			iStream.Read((CHAR*)&m_nBuffID, sizeof(m_nBuffID));
			iStream.Read((CHAR*)&m_nSkillID, sizeof(m_nSkillID));
			iStream.Read((CHAR*)&m_nSenderLogicCount, sizeof(m_nSenderLogicCount));
			iStream.Read((CHAR*)&m_nContinuance, sizeof(m_nContinuance));
		}
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

	BOOL GCDetailBuff::Write( SocketOutputStream& oStream )const
	{
	__ENTER_FUNCTION
		oStream.Write((CHAR*)&m_nSN, sizeof(m_nSN));
		oStream.Write((CHAR*)&m_bEnable, sizeof(m_bEnable));
		if ( m_bEnable )
		{
			oStream.Write((CHAR const*)&m_nReceiverID, sizeof(m_nReceiverID));
			oStream.Write((CHAR const*)&m_nSenderID, sizeof(m_nSenderID));
			oStream.Write((CHAR const*)&m_nBuffID, sizeof(m_nBuffID));
			oStream.Write((CHAR const*)&m_nSkillID, sizeof(m_nSkillID));
			oStream.Write((CHAR const*)&m_nSenderLogicCount, sizeof(m_nSenderLogicCount));
			oStream.Write((CHAR const*)&m_nContinuance, sizeof(m_nContinuance));
		}
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

	UINT GCDetailBuff::Execute( Player* pPlayer )
	{
	__ENTER_FUNCTION

		return GCDetailBuffHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
	}

}
