#include "stdafx.h"
#include "CGDiscardItem.h"



BOOL		CGDiscardItem::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_BagIndex,sizeof(BYTE));
		iStream.Read((CHAR*)&m_Opt,sizeof(BYTE));

		return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}


BOOL	CGDiscardItem::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_BagIndex,sizeof(BYTE));
		oStream.Write((CHAR*)&m_Opt,sizeof(BYTE));


		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

UINT	CGDiscardItem::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGDiscardItemHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
		return FALSE;
}