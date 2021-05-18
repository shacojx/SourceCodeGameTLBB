#include "stdafx.h"
// GCDetailSubEffectList.cpp
// 
/////////////////////////////////////////////////////

#include "GCDetailSubEffectList.h"

BOOL GCDetailSubEffectList::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCDetailSubEffectList::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCDetailSubEffectList::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCDetailSubEffectListHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

