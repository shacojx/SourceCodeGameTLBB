#include "stdafx.h"
#include "CGDispelBuffReq.h"

namespace Packets
{
	BOOL	CGDispelBuffReq::Read(SocketInputStream& iStream )	
	{
		__ENTER_FUNCTION
			iStream.Read((CHAR*)&m_nSkillID, sizeof(m_nSkillID));
			iStream.Read((CHAR*)&m_nImpactID, sizeof(m_nImpactID));
			iStream.Read((CHAR*)&m_nSN, sizeof(m_nSN));
			return TRUE;
		__LEAVE_FUNCTION
			return	FALSE;
	}


	BOOL	CGDispelBuffReq::Write(SocketOutputStream& oStream ) const
	{
		__ENTER_FUNCTION
			oStream.Write((CHAR const*)&m_nSkillID, sizeof(m_nSkillID));
			oStream.Write((CHAR const*)&m_nImpactID, sizeof(m_nImpactID));
			oStream.Write((CHAR const*)&m_nSN, sizeof(m_nSN));
			return	TRUE;

		__LEAVE_FUNCTION
			return	FALSE;
	}

	UINT	CGDispelBuffReq::Execute(Player* pPlayer )
	{
		__ENTER_FUNCTION
			
			return CGDispelBuffReqHandler::Execute(this,pPlayer);

		__LEAVE_FUNCTION

			return	FALSE;
	}

}
