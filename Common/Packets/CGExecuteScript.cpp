#include "stdafx.h"


#include "CGExecuteScript.h"

BOOL CGExecuteScript::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_Script.Read( iStream ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGExecuteScript::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	m_Script.Write( oStream ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGExecuteScript::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGExecuteScriptHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


