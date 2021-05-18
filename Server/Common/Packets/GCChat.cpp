#include "stdafx.h"


#include "GCChat.h"



BOOL GCChat::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

 	iStream.Read( (CHAR*)(&m_ChatType), sizeof(BYTE) ) ;

	iStream.Read( (CHAR*)(&m_ContexSize), sizeof(BYTE) ) ;
	if( m_ContexSize>0 && m_ContexSize<=MAX_CHAT_SIZE )
		iStream.Read( (CHAR*)(&m_Contex), sizeof(CHAR)*m_ContexSize ) ;
	
	iStream.Read( (CHAR*)(&m_SourNameSize), sizeof(BYTE) ) ;
	if( m_SourNameSize>0 && m_SourNameSize<=MAX_CHARACTER_NAME )
		iStream.Read( (CHAR*)(&m_SourName), sizeof(CHAR)*m_SourNameSize ) ;

	if( m_ChatType==CHAT_TYPE_NORMAL )
		iStream.Read( (CHAR*)(&m_SourID), sizeof(ObjID_t) ) ;

	iStream.Read( (CHAR*)(&m_CampID), sizeof(CampID_t) ) ;
	iStream.Read( (CHAR*)(&m_uWorldChatID), sizeof(UINT) ) ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCChat::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ChatType), sizeof(BYTE) ) ;

	oStream.Write( (CHAR*)(&m_ContexSize), sizeof(BYTE) ) ;
	if( m_ContexSize>0 && m_ContexSize<=MAX_CHAT_SIZE )
		oStream.Write( (CHAR*)(&m_Contex), sizeof(CHAR)*m_ContexSize ) ;
	
	oStream.Write( (CHAR*)(&m_SourNameSize), sizeof(BYTE) ) ;
	if( m_SourNameSize>0 && m_SourNameSize<=MAX_CHARACTER_NAME )
		oStream.Write( (CHAR*)(&m_SourName), sizeof(CHAR)*m_SourNameSize ) ;

	if( m_ChatType==CHAT_TYPE_NORMAL )
		oStream.Write( (CHAR*)(&m_SourID), sizeof(ObjID_t) ) ;

	oStream.Write( (CHAR*)(&m_CampID), sizeof(CampID_t) ) ;
	oStream.Write( (CHAR*)(&m_uWorldChatID), sizeof(UINT) ) ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCChat::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCChatHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


