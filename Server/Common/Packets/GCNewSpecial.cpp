#include "stdafx.h"
// GCNewSpecial.cpp

#include "GCNewSpecial.h"

BOOL GCNewSpecial::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(m_posWorld));
	iStream.Read( (CHAR*)(&m_Dir), sizeof(m_Dir));
	iStream.Read( (CHAR*)(&m_nDataID), sizeof(m_nDataID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCNewSpecial::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(m_posWorld));
	oStream.Write( (CHAR*)(&m_Dir), sizeof(m_Dir));
	oStream.Write( (CHAR*)(&m_nDataID), sizeof(m_nDataID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCNewSpecial::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCNewSpecialHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

