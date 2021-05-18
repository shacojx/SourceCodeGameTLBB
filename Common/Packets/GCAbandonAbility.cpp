#include "stdafx.h"
// GCAbandonAbility.cpp

#include "GCAbandonAbility.h"

BOOL GCAbandonAbility::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_AbilityID), sizeof(AbilityID_t));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCAbandonAbility::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_AbilityID), sizeof(AbilityID_t));
		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCAbandonAbility::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCAbandonAbilityHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}