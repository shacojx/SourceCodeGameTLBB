#include "stdafx.h"
#include "GCBoxItemList.h"
using namespace Packets;

BOOL GCBoxItemList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&m_ItemBoxID,sizeof(ObjID_t));
		iStream.Read((CHAR*)(&m_ItemNumber), sizeof(BYTE));
		iStream.Read((CHAR*)(&m_ItemBoxType),sizeof(WORD));
		if(m_ItemNumber>MAX_BOXITEM_NUMBER)	m_ItemNumber	= MAX_BOXITEM_NUMBER;

		for(INT	i =0;i<m_ItemNumber;i++)
		{
			m_ItemList[i].Read(iStream);
		}
		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCBoxItemList::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	
		Assert(m_ItemNumber<=MAX_BOXITEM_NUMBER);

		oStream.Write((CHAR*)(&m_ItemBoxID),sizeof(ObjID_t));
		oStream.Write( (CHAR*)(&m_ItemNumber), sizeof(BYTE));
		oStream.Write((CHAR*)(&m_ItemBoxType),sizeof(WORD));				
	
		for(INT	i =0;i<m_ItemNumber;i++)
		{
			m_ItemList[i].Write(oStream);

		}
		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT	GCBoxItemList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCBoxItemListHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}