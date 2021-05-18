#include "stdafx.h"
// CGStallApply.cpp
// 
/////////////////////////////////////////////////////

#include "CGStallApply.h"

BOOL CGStallApply::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGStallApply::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGStallApply::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGStallApplyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

