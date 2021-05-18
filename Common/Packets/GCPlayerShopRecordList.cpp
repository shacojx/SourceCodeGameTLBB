#include "stdafx.h"
//  GCPlayerShopRecordList.cpp
// 
/////////////////////////////////////////////////////

#include "GCPlayerShopRecordList.h"

BOOL GCPlayerShopRecordList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_EntryNum), sizeof(BYTE));
	if(m_EntryNum>0)
	{
		iStream.Read( (CHAR*)(&m_EntryList), sizeof(MessageEntry_t)*m_EntryNum);
	}
	iStream.Read( (CHAR*)(&m_nPage), sizeof(INT));
    return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCPlayerShopRecordList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_EntryNum), sizeof(BYTE));
	if(m_EntryNum>0)
	{
		oStream.Write( (CHAR*)(&m_EntryList), sizeof(MessageEntry_t)*m_EntryNum);
	}
	oStream.Write( (CHAR*)(&m_nPage), sizeof(INT));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCPlayerShopRecordList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCPlayerShopRecordListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

