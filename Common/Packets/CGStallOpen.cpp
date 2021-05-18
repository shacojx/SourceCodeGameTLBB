#include "stdafx.h"
// CGStallOpen.cpp
// 
/////////////////////////////////////////////////////

#include "CGStallOpen.h"

BOOL CGStallOpen::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_objID), sizeof(ObjID_t));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGStallOpen::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_objID), sizeof(ObjID_t));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGStallOpen::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGStallOpenHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

