#include "stdafx.h"


#include "GWFinger.h"



BOOL GWFinger::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_Finger.Read( iStream );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GWFinger::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	m_Finger.Write( oStream );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWFinger::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWFingerHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}


