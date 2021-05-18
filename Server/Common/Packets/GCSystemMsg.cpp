#include "stdafx.h"


#include "GCSystemMsg.h"



BOOL GCSystemMsg::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION


	iStream.Read( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;
	iStream.Read( (CHAR*)(&m_MessageType), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)(&m_ContexSize), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)(&m_Contex), sizeof(CHAR)*m_ContexSize ) ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCSystemMsg::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION


	oStream.Write( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;
	oStream.Write( (CHAR*)(&m_MessageType), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_ContexSize), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_Contex), sizeof(CHAR)*m_ContexSize ) ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCSystemMsg::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

	return GCSystemMsgHandler::Execute(this,pPlayer) ;

	__LEAVE_FUNCTION

	return FALSE ;
}


