#include "stdafx.h"
// GCAbilitySucc.cpp

#include "GCAbilitySucc.h"

BOOL	GCAbilitySucc::Read(SocketInputStream& iStream )	
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_Ability,sizeof(AbilityID_t));
	iStream.Read((CHAR*)&m_Prescription,sizeof(PrescriptionID_t));
	iStream.Read((CHAR*)&m_Obj,sizeof(UINT));

	return TRUE;

__LEAVE_FUNCTION

	return	FALSE;
}

BOOL	GCAbilitySucc::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_Ability,sizeof(AbilityID_t));
	oStream.Write((CHAR*)&m_Prescription,sizeof(PrescriptionID_t));
	oStream.Write((CHAR*)&m_Obj,sizeof(UINT));

	return TRUE;

__LEAVE_FUNCTION

	return	FALSE;
}

UINT	GCAbilitySucc::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
		
		return GCAbilitySuccHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}
