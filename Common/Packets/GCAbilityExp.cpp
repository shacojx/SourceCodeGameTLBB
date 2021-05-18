#include "stdafx.h"
// GCAbilityExp.cpp

#include "GCAbilityExp.h"

BOOL GCAbilityExp::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_AbilityID), sizeof(AbilityID_t));
	iStream.Read((CHAR*)(&m_Exp),sizeof(UINT));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCAbilityExp::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_AbilityID), sizeof(AbilityID_t));
	oStream.Write((CHAR*)(&m_Exp),sizeof(UINT));
		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCAbilityExp::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCAbilityExpHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}