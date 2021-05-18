#include "stdafx.h"
#include "CGUseGem.h"


BOOL	CGUseGem::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_GemBagIndex,sizeof(BYTE));
	iStream.Read((CHAR*)&m_EquipBagIndex,sizeof(BYTE));
	iStream.Read((CHAR*)&m_Platform,sizeof(ObjID_t));
	return TRUE;
	__LEAVE_FUNCTION
	return	FALSE;
}


BOOL	CGUseGem::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_GemBagIndex,sizeof(BYTE));
	oStream.Write((CHAR*)&m_EquipBagIndex,sizeof(BYTE));
	oStream.Write((CHAR*)&m_Platform,sizeof(ObjID_t));
	
	return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	CGUseGem::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGUseGemHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}


