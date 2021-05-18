#include "stdafx.h"
#include "CGPackage_SwapItem.h"


BOOL CGPackage_SwapItem::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_nPackageIndex1,sizeof(BYTE));
	iStream.Read((CHAR*)&m_nPackageIndex2,sizeof(BYTE));
	return TRUE;
	__LEAVE_FUNCTION
	return	FALSE;
}


BOOL CGPackage_SwapItem::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_nPackageIndex1,sizeof(BYTE));
	oStream.Write((CHAR*)&m_nPackageIndex2,sizeof(BYTE));
	
	return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	CGPackage_SwapItem::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPackage_SwapItemHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}


