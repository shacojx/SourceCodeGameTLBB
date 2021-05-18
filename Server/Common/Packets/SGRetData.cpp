#include "stdafx.h"



#include "SGRetData.h"



BOOL SGRetData::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Data), sizeof(PLAYERDATA) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL SGRetData::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Data), sizeof(PLAYERDATA) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT SGRetData::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return SGRetDataHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


