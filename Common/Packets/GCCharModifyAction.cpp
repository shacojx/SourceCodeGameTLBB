#include "stdafx.h"
// GCCharModifyAction.cpp
// 
/////////////////////////////////////////////////////

#include "GCCharModifyAction.h"

BOOL GCCharModifyAction::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_nLogicCount), sizeof(m_nLogicCount));
	iStream.Read( (CHAR*)(&m_nModifyTime), sizeof(m_nModifyTime));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharModifyAction::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_nLogicCount), sizeof(m_nLogicCount));
	oStream.Write( (CHAR*)(&m_nModifyTime), sizeof(m_nModifyTime));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharModifyAction::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharModifyActionHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

