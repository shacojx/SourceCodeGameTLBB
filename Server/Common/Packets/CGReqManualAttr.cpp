#include "stdafx.h"
#include "CGReqManualAttr.h"


BOOL	CGReqManualAttr::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_nStr,sizeof(m_nStr));
		iStream.Read((CHAR*)&m_nSpr,sizeof(m_nSpr));
		iStream.Read((CHAR*)&m_nCon,sizeof(m_nCon));
		iStream.Read((CHAR*)&m_nInt,sizeof(m_nInt));
		iStream.Read((CHAR*)&m_nDex,sizeof(m_nDex));
		return TRUE;

	__LEAVE_FUNCTION
		return FALSE;
}

BOOL	CGReqManualAttr::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION
	 
		oStream.Write((CHAR const*)&m_nStr,sizeof(m_nStr));
		oStream.Write((CHAR const*)&m_nSpr,sizeof(m_nSpr));
		oStream.Write((CHAR const*)&m_nCon,sizeof(m_nCon));
		oStream.Write((CHAR const*)&m_nInt,sizeof(m_nInt));
		oStream.Write((CHAR const*)&m_nDex,sizeof(m_nDex));
		return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT CGReqManualAttr::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
		
		return CGReqManualAttrHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION
		return FALSE;
}


