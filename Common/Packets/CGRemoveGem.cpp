#include "stdafx.h"
#include "CGRemoveGem.h"


BOOL	CGRemoveGem::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_GemIndex,sizeof(BYTE));
	iStream.Read((CHAR*)&m_EquipBagIndex,sizeof(BYTE));

	return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	CGRemoveGem::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_GemIndex,sizeof(BYTE));
	oStream.Write((CHAR*)&m_EquipBagIndex,sizeof(BYTE));

	return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	CGRemoveGem::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGRemoveGemHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}


