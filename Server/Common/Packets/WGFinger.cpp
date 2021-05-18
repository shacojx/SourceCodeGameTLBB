#include "stdafx.h"
#include "stdafx.h"


#include "WGFinger.h"



BOOL WGFinger::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_Finger.Read( iStream );
	iStream.Read((CHAR*)&m_GUID,sizeof(m_GUID));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL WGFinger::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	m_Finger.Write( oStream );
	oStream.Write((CHAR*)&m_GUID,sizeof(m_GUID));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT WGFinger::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGFingerHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}


