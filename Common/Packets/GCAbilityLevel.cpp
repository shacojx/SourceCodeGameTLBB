#include "stdafx.h"
// GCAbilityLevel.cpp

#include "GCAbilityLevel.h"

BOOL GCAbilityLevel::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_AbilityID), sizeof(AbilityID_t));
	iStream.Read((CHAR*)(&m_Level),sizeof(UINT));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCAbilityLevel::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_AbilityID), sizeof(AbilityID_t));
	oStream.Write((CHAR*)(&m_Level),sizeof(UINT));
		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCAbilityLevel::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCAbilityLevelHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}