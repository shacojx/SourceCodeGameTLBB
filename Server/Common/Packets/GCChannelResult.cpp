#include "stdafx.h"


#include "GCChannelResult.h"



BOOL GCChannelResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_Return), sizeof(BYTE) ) ;
		iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );
		iStream.Read( (CHAR*)&m_ChannelID, sizeof(ChannelID_t) );
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCChannelResult::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_Return), sizeof(BYTE) ) ;
		oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );
		oStream.Write( (CHAR*)&m_ChannelID, sizeof(ChannelID_t) );
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCChannelResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCChannelResultHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


