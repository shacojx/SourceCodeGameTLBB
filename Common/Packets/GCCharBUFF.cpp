#include "stdafx.h"

#include "GCCharBUFF.h"

namespace Packets
{
	BOOL GCCharBuff::Read( SocketInputStream& iStream ) 
	{
	__ENTER_FUNCTION
		iStream.Read((CHAR *)(&m_nReceiverID), sizeof(m_nReceiverID));
		iStream.Read((CHAR *)(&m_nBuffID), sizeof(m_nBuffID));
		iStream.Read((CHAR *)(&m_bEnable), sizeof(m_bEnable));
		if ( m_bEnable )
		{
			iStream.Read((CHAR *)(&m_nSenderID), sizeof(m_nSenderID));
			iStream.Read((CHAR *)(&m_nSkillID), sizeof(m_nSkillID));
			iStream.Read((CHAR *)(&m_nSenderLogicCount), sizeof(m_nSenderLogicCount));
			iStream.Read((CHAR *)(&m_nSN), sizeof(m_nSN));
		}
		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
	}

	BOOL GCCharBuff::Write( SocketOutputStream& oStream )const
	{
	__ENTER_FUNCTION
		oStream.Write((CHAR const*)(&m_nReceiverID), sizeof(m_nReceiverID));
		oStream.Write((CHAR const*)(&m_nBuffID), sizeof(m_nBuffID));
		oStream.Write((CHAR const*)(&m_bEnable), sizeof(m_bEnable));
		if ( m_bEnable )
		{
			oStream.Write((CHAR const*)(&m_nSenderID), sizeof(m_nSenderID));
			oStream.Write((CHAR const*)(&m_nSkillID), sizeof(m_nSkillID));
			oStream.Write((CHAR const*)(&m_nSenderLogicCount), sizeof(m_nSenderLogicCount));
			oStream.Write((CHAR const*)(&m_nSN), sizeof(m_nSN));
		}
		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
	}

	UINT GCCharBuff::Execute( Player* pPlayer )
	{
	__ENTER_FUNCTION
		return GCCharBuffHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
	}

}
