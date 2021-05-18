#include "stdafx.h"
#include "CGDiscardEquip.h"



BOOL		CGDiscardEquip::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_EquipPoint,sizeof(BYTE));

		return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	CGDiscardEquip::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_EquipPoint,sizeof(BYTE));

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

UINT	CGDiscardEquip::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGDiscardEquipHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
		return FALSE;
}