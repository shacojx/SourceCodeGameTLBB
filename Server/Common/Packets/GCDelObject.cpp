#include "stdafx.h"

#include "GCDelObject.h"

BOOL GCDelObject::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_idScene), sizeof(m_idScene));
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCDelObject::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_idScene), sizeof(m_idScene));
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCDelObject::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCDelObjectHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

