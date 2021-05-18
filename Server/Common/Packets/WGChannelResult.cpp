#include "stdafx.h"


#include "WGChannelResult.h"



BOOL WGChannelResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
		iStream.Read( (CHAR*)(&m_Return), sizeof(BYTE) ) ;
		iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );
		iStream.Read( (CHAR*)&m_ChannelID, sizeof(ChannelID_t) );
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGChannelResult::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
		oStream.Write( (CHAR*)(&m_Return), sizeof(BYTE) ) ;
		oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );
		oStream.Write( (CHAR*)&m_ChannelID, sizeof(ChannelID_t) );
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGChannelResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return WGChannelResultHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


