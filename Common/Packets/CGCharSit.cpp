#include "stdafx.h"
// CGCharSit.cpp
// 
/////////////////////////////////////////////////////

#include "CGCharSit.h"

BOOL CGCharSit::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_bySit), sizeof(m_bySit));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGCharSit::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_bySit), sizeof(m_bySit));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGCharSit::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGCharSitHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

