#include "stdafx.h"
// CGLockTarget.cpp
// 
/////////////////////////////////////////////////////

#include "CGLockTarget.h"

BOOL CGLockTarget::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_nTargetID), sizeof(m_nTargetID));
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGLockTarget::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nTargetID), sizeof(m_nTargetID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGLockTarget::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGLockTargetHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


