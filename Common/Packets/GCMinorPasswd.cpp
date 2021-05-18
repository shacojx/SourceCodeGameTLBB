#include "stdafx.h"


#include "GCMinorPasswd.h"



BOOL GCMinorPasswd::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_MinorPassword.Read(iStream);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GCMinorPasswd::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	m_MinorPassword.Write(oStream);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GCMinorPasswd::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCMinorPasswdHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}


