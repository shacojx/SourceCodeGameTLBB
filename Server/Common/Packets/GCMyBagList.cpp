#include "stdafx.h"
#include "GCMyBagList.h"


using namespace Packets;

BOOL GCMyBagList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)(&m_Mode), sizeof(ASK_BAG_MODE));
	iStream.Read((CHAR*)(&m_AskCount),sizeof(BYTE));
	if(m_AskCount>MAX_BAG_SIZE)
	{
		Assert(FALSE);
		m_AskCount = MAX_BAG_SIZE;
	}

	for(INT i = 0;i<m_AskCount;i++)
	{
		iStream.Read((CHAR*)(&(m_ItemData[i])), sizeof(_BAG_ITEM));
	}
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCMyBagList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_Mode), sizeof(ASK_BAG_MODE));
	oStream.Write((CHAR*)(&m_AskCount),sizeof(BYTE));
	if(m_AskCount>MAX_BAG_SIZE) 
	{
		Assert(FALSE);
	}

	for(INT i = 0;i<m_AskCount;i++)
	{
		oStream.Write((CHAR*)(&(m_ItemData[i])),sizeof(_BAG_ITEM));
	}
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCMyBagList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCMyBagListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}