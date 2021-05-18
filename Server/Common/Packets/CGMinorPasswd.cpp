#include "stdafx.h"


#include "CGMinorPasswd.h"



BOOL CGMinorPasswd::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_MinorPassword.Read( iStream );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL CGMinorPasswd::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	m_MinorPassword.Write( oStream );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT CGMinorPasswd::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGMinorPasswdHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}


