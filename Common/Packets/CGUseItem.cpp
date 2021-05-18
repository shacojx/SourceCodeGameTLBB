#include "stdafx.h"
#include "CGUseItem.h"


BOOL CGUseItem::Read(SocketInputStream& iStream )	
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_BagIndex,sizeof(m_BagIndex));
	iStream.Read((CHAR*)&m_Target,sizeof(m_Target));
	iStream.Read((CHAR*)&m_posTarget,sizeof(m_posTarget));
	iStream.Read((CHAR*)&m_fDir,sizeof(m_fDir));
	iStream.Read((CHAR*)&m_TargetPetGUID,sizeof(m_TargetPetGUID));
	iStream.Read((CHAR*)&m_TargetItemIndex,sizeof(m_TargetItemIndex));

	return TRUE;

__LEAVE_FUNCTION

	return	FALSE;
}


BOOL CGUseItem::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_BagIndex,sizeof(m_BagIndex));
	oStream.Write((CHAR*)&m_Target,sizeof(m_Target));
	oStream.Write((CHAR*)&m_posTarget,sizeof(m_posTarget));
	oStream.Write((CHAR*)&m_fDir,sizeof(m_fDir));
	oStream.Write((CHAR*)&m_TargetPetGUID,sizeof(m_TargetPetGUID));
	oStream.Write((CHAR*)&m_TargetItemIndex,sizeof(m_TargetItemIndex));

	return	TRUE;

__LEAVE_FUNCTION

	return	FALSE;
}

UINT CGUseItem::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return CGUseItemHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return	FALSE;
}


