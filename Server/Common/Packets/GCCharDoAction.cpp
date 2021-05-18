#include "stdafx.h"
// GCCharDoAction.cpp
// 
/////////////////////////////////////////////////////

#include "GCCharDoAction.h"

BOOL GCCharDoAction::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_nActionID), sizeof(m_nActionID));
	iStream.Read( (CHAR*)(&m_TargetID), sizeof(m_TargetID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharDoAction::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_nActionID), sizeof(m_nActionID));
	oStream.Write( (CHAR*)(&m_TargetID), sizeof(m_TargetID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharDoAction::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharDoActionHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

