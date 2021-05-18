#include "stdafx.h"
#include "GCItemInfo.h"



BOOL GCItemInfo::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	
	iStream.Read((CHAR*)(&m_BagIndex), sizeof(WORD));
	iStream.Read((CHAR*)(&m_nsNull), sizeof(BOOL));
	
	m_ITEM.Read(iStream);

	return TRUE ;
	
	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCItemInfo::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_BagIndex), sizeof(WORD));
	oStream.Write((CHAR*)(&m_nsNull), sizeof(BOOL));

	m_ITEM.Write(oStream);
	
	return TRUE;

	__LEAVE_FUNCTION
	return FALSE ;
}

UINT	GCItemInfo::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCItemInfoHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}