#include "stdafx.h"
// GCAbilityResult.cpp

#include "GCAbilityResult.h"

BOOL GCAbilityResult::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_Ability,sizeof(AbilityID_t) );
	iStream.Read((CHAR*)&m_Prescription,sizeof(PrescriptionID_t));
	iStream.Read( (CHAR*)(&m_nResult), sizeof(INT) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCAbilityResult::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_Ability,sizeof(AbilityID_t) );
	oStream.Write((CHAR*)&m_Prescription,sizeof(PrescriptionID_t));
	oStream.Write( (CHAR*)(&m_nResult), sizeof(INT) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCAbilityResult::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCAbilityResultHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}
