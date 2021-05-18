#include "stdafx.h"
// GCCharAction.cpp
// 
/////////////////////////////////////////////////////

#include "GCCharAction.h"

BOOL GCCharAction::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_nLogicCount), sizeof(m_nLogicCount));
	iStream.Read( (CHAR*)(&m_nActionID), sizeof(m_nActionID));
	iStream.Read( (CHAR*)(&m_TargetID), sizeof(m_TargetID));
	iStream.Read( (CHAR*)(&m_posTarget), sizeof(m_posTarget));
	iStream.Read( (CHAR*)(&m_nTotalTime), sizeof(m_nTotalTime));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharAction::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_nLogicCount), sizeof(m_nLogicCount));
	oStream.Write( (CHAR*)(&m_nActionID), sizeof(m_nActionID));
	oStream.Write( (CHAR*)(&m_TargetID), sizeof(m_TargetID));
	oStream.Write( (CHAR*)(&m_posTarget), sizeof(m_posTarget));
	oStream.Write( (CHAR*)(&m_nTotalTime), sizeof(m_nTotalTime));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharAction::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharActionHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

