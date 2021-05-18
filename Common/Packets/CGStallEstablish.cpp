#include "stdafx.h"
// CGStallEstablish.cpp
// 
/////////////////////////////////////////////////////

#include "CGStallEstablish.h"

BOOL CGStallEstablish::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGStallEstablish::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGStallEstablish::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGStallEstablishHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

