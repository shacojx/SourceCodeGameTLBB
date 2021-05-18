#include "stdafx.h"
// CGSetMoodToHead.cpp
// 
/////////////////////////////////////////////////////

#include "CGSetMoodToHead.h"

BOOL CGSetMoodToHead::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_bView), sizeof(BYTE) );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGSetMoodToHead::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_bView), sizeof(BYTE) );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGSetMoodToHead::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGSetMoodToHeadHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

