#include "stdafx.h"
// CGAskDetailAbilityInfo.cpp
// 
/////////////////////////////////////////////////////

#include "CGAskDetailAbilityInfo.h"

BOOL CGAskDetailAbilityInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskDetailAbilityInfo::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskDetailAbilityInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskDetailAbilityInfoHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

