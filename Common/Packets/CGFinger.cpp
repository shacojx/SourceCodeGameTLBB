#include "stdafx.h"


#include "CGFinger.h"



BOOL CGFinger::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_Finger.Read( iStream );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL CGFinger::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	m_Finger.Write( oStream );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT CGFinger::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return TRUE;
	//return CGFingerHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}


