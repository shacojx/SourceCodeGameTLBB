#include "stdafx.h"
#include "GCPackage_SwapItem.h"


BOOL GCPackage_SwapItem::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_nResult,sizeof(BYTE));
	iStream.Read((CHAR*)&m_nPackageIndex1,sizeof(BYTE));
	iStream.Read((CHAR*)&m_nPackageIndex2,sizeof(BYTE));
	return TRUE;
	__LEAVE_FUNCTION
	return	FALSE;
}


BOOL GCPackage_SwapItem::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_nResult,sizeof(BYTE));
	oStream.Write((CHAR*)&m_nPackageIndex1,sizeof(BYTE));
	oStream.Write((CHAR*)&m_nPackageIndex2,sizeof(BYTE));
	
	return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT GCPackage_SwapItem::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCPackage_SwapItemHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}


