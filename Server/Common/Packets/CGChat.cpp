#include "stdafx.h"


#include "CGChat.h"



BOOL CGChat::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	m_ChatType = 0 ;
	m_ContexSize = 0 ;
	memset( m_Contex, 0, MAX_CHAT_SIZE ) ;
	m_TargetSize = 0 ;
	memset( m_TargetName, 0, MAX_CHARACTER_NAME ) ;
	m_TeamID = INVALID_ID ;
	m_ChannelID = INVALID_ID ;
	m_GuildID = INVALID_ID ;
	m_MenpaiID = MATTRIBUTE_WUMENPAI ;


	iStream.Read( (CHAR*)(&m_ChatType), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)(&m_ContexSize), sizeof(BYTE) ) ;
	Assert( m_ContexSize<=MAX_CHAT_SIZE ) ;
	iStream.Read( (CHAR*)(&m_Contex), sizeof(CHAR)*m_ContexSize ) ;
	m_Contex[MAX_CHAT_SIZE-1]=0;

	switch( m_ChatType )
	{
	case CHAT_TYPE_NORMAL:
	case CHAT_TYPE_SCENE:
	case CHAT_TYPE_SYSTEM:
		break ;
	case CHAT_TYPE_TEAM:
		iStream.Read( (CHAR*)(&m_TeamID), sizeof(TeamID_t) ) ;
		break ;
	case CHAT_TYPE_TELL:
		iStream.Read( (CHAR*)(&m_TargetSize), sizeof(BYTE) ) ;
		Assert( m_TargetSize<=MAX_CHARACTER_NAME ) ;
		iStream.Read( (CHAR*)(&m_TargetName), m_TargetSize*sizeof(CHAR) ) ;
		m_TargetName[MAX_CHARACTER_NAME-1]=0;
		break ;
	case CHAT_TYPE_CHANNEL:
		iStream.Read( (CHAR*)(&m_ChannelID), sizeof(ChannelID_t) ) ;
		break ;
	case CHAT_TYPE_GUILD:
		iStream.Read( (CHAR*)(&m_GuildID), sizeof(GuildID_t) ) ;
		break ;
	case CHAT_TYPE_MENPAI:
		iStream.Read( (CHAR*)(&m_MenpaiID), sizeof(BYTE) ) ;
		break ;
	default :
		Assert(FALSE) ;
		break ;
	}


	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGChat::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ChatType), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_ContexSize), sizeof(BYTE) ) ;
	Assert( m_ContexSize<=MAX_CHAT_SIZE ) ;
	oStream.Write( (CHAR*)(&m_Contex), sizeof(CHAR)*m_ContexSize ) ;
	
	switch( m_ChatType )
	{
	case CHAT_TYPE_NORMAL:
	case CHAT_TYPE_SCENE:
	case CHAT_TYPE_SYSTEM:
		break ;
	case CHAT_TYPE_TEAM:
		oStream.Write( (CHAR*)(&m_TeamID), sizeof(TeamID_t) ) ;
		break ;
	case CHAT_TYPE_TELL:
		oStream.Write( (CHAR*)(&m_TargetSize), sizeof(BYTE) ) ;
		Assert( m_TargetSize<=MAX_CHARACTER_NAME ) ;
		oStream.Write( (CHAR*)(&m_TargetName), m_TargetSize*sizeof(CHAR) ) ;
		break ;
	case CHAT_TYPE_CHANNEL:
		oStream.Write( (CHAR*)(&m_ChannelID), sizeof(ChannelID_t) ) ;
		break ;
	case CHAT_TYPE_GUILD:
		oStream.Write( (CHAR*)(&m_GuildID), sizeof(GuildID_t) ) ;
		break ;
	case CHAT_TYPE_MENPAI:
		oStream.Write( (CHAR*)(&m_MenpaiID), sizeof(BYTE) ) ;
		break ;
	default :
		Assert(FALSE) ;
		break ;
	}


	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGChat::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGChatHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


