#include "stdafx.h"
#include "CGAskItemInfo.h"

 

BOOL CGAskItemInfo::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_BagIndex), sizeof(WORD));
	 return TRUE;

	__LEAVE_FUNCTION
	
	return FALSE ;
}

BOOL CGAskItemInfo::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	
		oStream.Write( (CHAR*)(&m_BagIndex), sizeof(WORD));
		return TRUE;

	__LEAVE_FUNCTION
		return FALSE ;
}

UINT	CGAskItemInfo::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGAskItemInfoHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}