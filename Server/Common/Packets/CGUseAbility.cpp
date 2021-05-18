#include "stdafx.h"
#include "CGUseAbility.h"

BOOL	CGUseAbility::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION
	
		iStream.Read((CHAR*)&m_Ability,sizeof(AbilityID_t));
		iStream.Read((CHAR*)&m_Prescription,sizeof(PrescriptionID_t));
		iStream.Read((CHAR*)&m_Platform,sizeof(ObjID_t));
		return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}

BOOL	CGUseAbility::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
		
		oStream.Write((CHAR*)&m_Ability,sizeof(AbilityID_t));
		oStream.Write((CHAR*)&m_Prescription,sizeof(PrescriptionID_t));
		oStream.Write((CHAR*)&m_Platform,sizeof(ObjID_t));
		return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	CGUseAbility::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
		
		return CGUseAbilityHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}
