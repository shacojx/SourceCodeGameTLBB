#include "stdafx.h"
#include "GCBankItemInfo.h"



BOOL GCBankItemInfo::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	
	iStream.Read((CHAR*)(&m_BankIndex), sizeof(WORD));
	iStream.Read((CHAR*)(&m_nsNull), sizeof(BOOL));
	
	m_ITEM.Read(iStream);

	return TRUE ;
	
	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCBankItemInfo::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_BankIndex), sizeof(WORD));
	oStream.Write((CHAR*)(&m_nsNull), sizeof(BOOL));

	m_ITEM.Write(oStream);
	
	return TRUE;

	__LEAVE_FUNCTION
	return FALSE ;
}

UINT	GCBankItemInfo::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCBankItemInfoHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}