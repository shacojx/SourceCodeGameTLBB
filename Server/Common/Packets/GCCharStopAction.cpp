#include "stdafx.h"
// GCCharStopAction.cpp
// 
/////////////////////////////////////////////////////

#include "GCCharStopAction.h"

BOOL GCCharStopAction::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_nLogicCount), sizeof(m_nLogicCount));
	iStream.Read( (CHAR*)(&m_uStopTime), sizeof(m_uStopTime));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharStopAction::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_nLogicCount), sizeof(m_nLogicCount));
	oStream.Write( (CHAR*)(&m_uStopTime), sizeof(m_uStopTime));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharStopAction::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharStopActionHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

