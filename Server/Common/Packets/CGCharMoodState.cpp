#include "stdafx.h"
// CGCharMoodState.cpp
// 
/////////////////////////////////////////////////////

#include "CGCharMoodState.h"

BOOL CGCharMoodState::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_cMoodState), sizeof(m_cMoodState));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGCharMoodState::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_cMoodState), sizeof(m_cMoodState));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGCharMoodState::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return TRUE;
	//return CGCharMoodStateHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

