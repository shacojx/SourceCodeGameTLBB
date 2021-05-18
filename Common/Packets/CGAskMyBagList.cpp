#include "stdafx.h"
#include "CGAskMyBagList.h"

using namespace Packets;

BOOL CGAskMyBagList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read((CHAR*)(&m_Mode), sizeof(ASK_BAG_MODE));
		iStream.Read((CHAR*)(&m_AskCount),sizeof(BYTE));
		if(m_AskCount>MAX_BAG_SIZE) m_AskCount = MAX_BAG_SIZE;
		
		for(INT	i = 0;i<m_AskCount;i++)
		{
			iStream.Read((CHAR*)(&m_ItemIndex[i]),sizeof(BYTE));
			
		}
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGAskMyBagList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)(&m_Mode), sizeof(ASK_BAG_MODE));
	oStream.Write((CHAR*)(&m_AskCount),sizeof(BYTE));

	Assert( m_AskCount<=MAX_BAG_SIZE ) ;
	for(INT	i = 0;i<((m_AskCount>MAX_BAG_SIZE)?(MAX_BAG_SIZE):(m_AskCount));i++)
	{
		oStream.Write((CHAR*)(&m_ItemIndex[i]),sizeof(BYTE));
		
	}
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT	CGAskMyBagList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGAskMyBagListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}