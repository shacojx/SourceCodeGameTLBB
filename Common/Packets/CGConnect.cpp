#include "stdafx.h"


#include "CGConnect.h"

BOOL CGConnect::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Key), sizeof(UINT) ) ;
	iStream.Read( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;
	iStream.Read( (CHAR*)(&m_ServerID), sizeof(ID_t) ) ;
	iStream.Read( (CHAR*)(&m_szAccount), sizeof(CHAR)*MAX_ACCOUNT ) ;
	m_szAccount[MAX_ACCOUNT] = 0 ;

	iStream.Read( (CHAR*)(&m_nGender), sizeof(INT) ) ;
	iStream.Read( (CHAR*)(&m_nCheckVer), sizeof(INT) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGConnect::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Key), sizeof(UINT) ) ;
	oStream.Write( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;
	oStream.Write( (CHAR*)(&m_ServerID), sizeof(ID_t) ) ;
	oStream.Write( (CHAR*)(&m_szAccount), sizeof(CHAR)*MAX_ACCOUNT ) ;
	oStream.Write( (CHAR*)(&m_nGender), sizeof(INT) ) ;
	oStream.Write( (CHAR*)(&m_nCheckVer), sizeof(INT) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGConnect::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGConnectHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


