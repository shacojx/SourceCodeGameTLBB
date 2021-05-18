#include "stdafx.h"


#include "WGRetUserData.h"



BOOL WGRetUserData::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	iStream.Read( (CHAR*)(&m_Return), sizeof(BYTE) ) ;

	if( m_Return == UDR_USERDATA )
	{
		iStream.Read( (CHAR*)(&m_UserData), sizeof(FULLUSERDATA) ) ;
		iStream.Read( (CHAR*)(&m_Age), sizeof(BYTE) ) ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGRetUserData::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	oStream.Write( (CHAR*)(&m_Return), sizeof(BYTE) ) ;

	if( m_Return == UDR_USERDATA )
	{
		oStream.Write( (CHAR*)(&m_UserData), sizeof(FULLUSERDATA) ) ;
		oStream.Write( (CHAR*)(&m_Age), sizeof(BYTE) ) ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGRetUserData::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGRetUserDataHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


