#include "stdafx.h"


#include "WGNotifyUser.h"



BOOL WGNotifyUser::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;
	iStream.Read( (CHAR*)(&m_Status), sizeof(INT) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGNotifyUser::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;
	oStream.Write( (CHAR*)(&m_Status), sizeof(INT) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGNotifyUser::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGNotifyUserHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


