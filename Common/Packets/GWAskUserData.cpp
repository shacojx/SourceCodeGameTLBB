#include "stdafx.h"


#include "GWAskUserData.h"



BOOL GWAskUserData::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	iStream.Read( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;
	iStream.Read( (CHAR*)(&m_uKey), sizeof(UINT) ) ;
	iStream.Read( (CHAR*)(&m_szAccount), sizeof(CHAR)*MAX_ACCOUNT ) ;
	m_szAccount[MAX_ACCOUNT] = 0 ;
	iStream.Read( (CHAR*)(&m_nGender), sizeof(INT) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWAskUserData::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	oStream.Write( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;
	oStream.Write( (CHAR*)(&m_uKey), sizeof(UINT) ) ;
	oStream.Write( (CHAR*)(&m_szAccount), sizeof(CHAR)*MAX_ACCOUNT ) ;
	oStream.Write( (CHAR*)(&m_nGender), sizeof(INT) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWAskUserData::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWAskUserDataHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


