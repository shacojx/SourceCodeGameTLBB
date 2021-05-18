#include "stdafx.h"


#include "GCFinger.h"



BOOL GCFinger::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_Finger.Read( iStream );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GCFinger::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	m_Finger.Write( oStream );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GCFinger::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return TRUE;
	//return GCFingerHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}


