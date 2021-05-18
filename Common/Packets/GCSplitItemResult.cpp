#include "stdafx.h"
// GCSplitItemResult.cpp
// 
/////////////////////////////////////////////////////

#include "GCSplitItemResult.h"

BOOL GCSplitItemResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read((CHAR*)(&m_ConIndex),	sizeof(WORD));
		iStream.Read((CHAR*)(&m_nsNull),	sizeof(BOOL));
		iStream.Read((CHAR*)(&m_Container), sizeof(INT));
		iStream.Read((CHAR*)(&m_Result),	sizeof(BYTE));
		
		m_ITEM.Read(iStream);

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCSplitItemResult::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
			oStream.Write((CHAR*)(&m_ConIndex),	sizeof(WORD));
			oStream.Write((CHAR*)(&m_nsNull),	sizeof(BOOL));
			oStream.Write((CHAR*)(&m_Container),sizeof(INT));
			oStream.Write((CHAR*)(&m_Result),	sizeof(BYTE));
			
			m_ITEM.Write(oStream);

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCSplitItemResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCSplitItemResultHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

