#include "stdafx.h"
// CGBBSApply.cpp
// 
/////////////////////////////////////////////////////

#include "CGBBSApply.h"

BOOL CGBBSApply::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_objID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_Serial), sizeof(UINT));
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGBBSApply::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_objID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_Serial), sizeof(UINT));
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGBBSApply::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGBBSApplyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

