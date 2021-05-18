#include "stdafx.h"



#include "GCPackUpPacket.h"

BOOL GCPackUpPacket::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	//iStream.Read( (CHAR*)(&m_Type), sizeof(UINT) ) ;
	//iStream.Read( (CHAR*)(&m_Test), sizeof(_PGCTEST) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCPackUpPacket::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	//oStream.Write( (CHAR*)(&m_Type), sizeof(UINT) ) ;
	//oStream.Write( (CHAR*)(&m_Test), sizeof(_PGCTEST) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCPackUpPacket::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCPackUpPacketHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


