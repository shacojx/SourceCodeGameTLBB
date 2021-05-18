#include "stdafx.h"


#include "GCArrive.h"



BOOL GCArrive::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID) ) ;
	iStream.Read( (CHAR*)(&m_nHandleID), sizeof(m_nHandleID) ) ;
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(m_posWorld) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCArrive::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID) ) ;
	oStream.Write( (CHAR*)(&m_nHandleID), sizeof(m_nHandleID) ) ;
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(m_posWorld) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCArrive::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCArriveHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


