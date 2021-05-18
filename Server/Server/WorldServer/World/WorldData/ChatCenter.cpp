#include "stdafx.h"

#include "ChatCenter.h"
#include "PacketFactoryManager.h"
#include "Packet.h"
#include "OnlineUser.h"
#include "CGChat.h"
#include "GWChat.h"
#include "WGChat.h"
#include "ServerManager.h"
#include "ServerPlayer.h"


ChatCenter* g_pChatCenter=NULL ;


/////////////////////////////////////////////////////////////////////////////////
//ChatDB
ChatDB::ChatDB( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

ChatDB::~ChatDB( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

BOOL ChatDB::Init( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION

	return TRUE ;
}

/////////////////////////////////////////////////////////////////////////////////
//ChatChannel
ChatChannel::ChatChannel( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

ChatChannel::~ChatChannel( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

void ChatChannel::CleanUp( )
{
__ENTER_FUNCTION

	m_iNumber = 0 ;
	m_Empty = TRUE ;
	for( int i=0; i<MAX_CHATCHANNEL_MEMBER; i++ )
	{
		m_aMember[i] = INVALID_ID ;
	}

__LEAVE_FUNCTION
}

BOOL ChatChannel::IsActive( )
{
__ENTER_FUNCTION

	if( IsEmpty() ) return FALSE ;
	if( m_iNumber>0 ) return TRUE ;


__LEAVE_FUNCTION

	return FALSE ;
}
	
BOOL ChatChannel::AddMember( GUID_t guid )
{
__ENTER_FUNCTION

	Assert( guid!=INVALID_ID ) ;

	if( m_iNumber>=MAX_CHATCHANNEL_MEMBER )
		return FALSE ;

	m_aMember[m_iNumber] = guid ;
	m_iNumber ++ ;


__LEAVE_FUNCTION

	return TRUE ;
}

BOOL ChatChannel::DelMember( GUID_t guid )
{
__ENTER_FUNCTION

	Assert( guid!=INVALID_ID ) ;

	BOOL bFind = FALSE ;
	int i ;

	for( i=0; i<m_iNumber; i++ )
	{
		if( m_aMember[i] == guid )
		{
			bFind = TRUE ;
			break ;
		}
	}
	if( bFind )
	{
		for( ;i<m_iNumber-1; i++ )
		{
			m_aMember[i] = m_aMember[i+1] ;
		}

		m_iNumber -- ;
		Assert( m_iNumber>=0 ) ;

		return TRUE ;
	}

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ChatChannel::IsMember( GUID_t guid )
{
__ENTER_FUNCTION

	Assert( guid!=INVALID_ID ) ;

	for( int i=0; i<m_iNumber; i++ )
	{
		if( m_aMember[i] == guid )
			return TRUE ;
	}

__LEAVE_FUNCTION

	return FALSE ;
}

GUID_t ChatChannel::Leader( )
{
__ENTER_FUNCTION

	if( IsActive() )
		return m_aMember[0] ;

__LEAVE_FUNCTION

	return INVALID_ID ;
}

GUID_t ChatChannel::Member( int iIndex )
{
__ENTER_FUNCTION

	Assert( iIndex>=0 && iIndex<MAX_CHATCHANNEL_MEMBER ) ;
	if( IsActive() )
		return m_aMember[iIndex] ;

__LEAVE_FUNCTION

	return INVALID_ID ;
}

/////////////////////////////////////////////////////////////////////////////////
//ChatCenter
ChatCenter::ChatCenter( )
{
__ENTER_FUNCTION

	m_pChannels = NULL ;
	m_iCount = 0 ;
	m_iCreateOffset = 0 ;

	for( INT i=0; i<MAX_WORLD_HOLD_CHAT_PACKETS; i++ )
	{
		m_pChatPackets[i] = NULL ;
	}
	m_uCurrentChat = 0 ;
	m_uWorldChatID = 0 ;

__LEAVE_FUNCTION
}

ChatCenter::~ChatCenter( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

BOOL ChatCenter::Init( )
{
__ENTER_FUNCTION

	BOOL bRet = m_MailDB.Init( ) ;
	if( bRet==FALSE )
	{
		Assert(FALSE) ;
		return FALSE ;
	}

	m_pChannels = new ChatChannel[MAX_CHANNELS] ;
	if( m_pChannels == NULL )
	{
		Assert(FALSE) ;
		return FALSE ;
	}

	for( int i=0; i<MAX_CHANNELS; i++ )
	{
		m_pChannels[i].SetChannelID( (ChannelID_t)i ) ;
	}


__LEAVE_FUNCTION

	return TRUE ;
}

void ChatCenter::CleanUp( )
{
__ENTER_FUNCTION

	m_iCount = 0 ;
	m_iCreateOffset = 0 ;
	SAFE_DELETE_ARRAY(m_pChannels) ;

	for( INT i=0; i<MAX_WORLD_HOLD_CHAT_PACKETS; i++ )
	{
		if( m_pChatPackets[i] )
		{
			g_pPacketFactoryManager->RemovePacket( m_pChatPackets[i] ) ;
		}
	}
	m_uCurrentChat = 0 ;
	m_uWorldChatID = 0 ;

__LEAVE_FUNCTION
}

BOOL ChatCenter::HeartBeat( DWORD dwTime )
{
__ENTER_FUNCTION



__LEAVE_FUNCTION

	return TRUE ;
}

ChannelID_t ChatCenter::CreateChatChannel( )
{
__ENTER_FUNCTION

	for( int i=0; i<MAX_CHANNELS; i++ )
	{
		ChatChannel* pChatChannel = GetChatChannel(m_iCreateOffset) ;
		if( pChatChannel && pChatChannel->IsEmpty() )
		{
			ChannelID_t cid = m_iCreateOffset ;
			pChatChannel->SetEmpty( FALSE ) ;
			m_iCount ++ ;
			Assert( m_iCount>=0&&m_iCount<MAX_CHANNELS ) ;
			
			m_iCreateOffset ++ ;
			if( m_iCreateOffset >= MAX_CHANNELS )
				m_iCreateOffset = 0 ;

			return cid ;
		}

		m_iCreateOffset ++ ;
		if( m_iCreateOffset >= MAX_CHANNELS )
			m_iCreateOffset = 0 ;

	}

__LEAVE_FUNCTION

	return INVALID_ID ;
}

BOOL ChatCenter::DestoryChatChannel( ChannelID_t cid )
{
__ENTER_FUNCTION

	ChatChannel* pChatChannel = GetChatChannel( cid ) ;
	if( pChatChannel )
	{
		pChatChannel->CleanUp( ) ;
		m_iCount -- ;
		Assert( m_iCount>=0 ) ;

		return TRUE ;
	}

__LEAVE_FUNCTION

	return TRUE ;
}

VOID ChatCenter::PushChatPacket( Packet* pPacket )
{
__ENTER_FUNCTION

	if( m_pChatPackets[m_uCurrentChat] )
	{
		g_pPacketFactoryManager->RemovePacket( m_pChatPackets[m_uCurrentChat] ) ;
	}
	m_pChatPackets[m_uCurrentChat] = pPacket ;

	m_uCurrentChat ++ ;
	if( m_uCurrentChat >=MAX_WORLD_HOLD_CHAT_PACKETS )
	{
		m_uCurrentChat = 0 ;
	}

__LEAVE_FUNCTION
}

VOID ChatCenter::SendNeedPacket( USER* pUser, UINT uWorldChatID )
{
__ENTER_FUNCTION

	UINT uIndex = m_uCurrentChat ;
	for( INT i=0; i<MAX_WORLD_HOLD_CHAT_PACKETS; i++ )
	{
		if( uIndex>=MAX_WORLD_HOLD_CHAT_PACKETS )
		{
			uIndex = 0 ;
		}

		if( m_pChatPackets[uIndex] == NULL )
		{
			uIndex ++ ;
			continue;
		}

		if( ((WGChat*)m_pChatPackets[uIndex])->GetWorldChatID() <= uWorldChatID )
		{
			uIndex ++ ;
			continue ;
		}

		GWChat* pChat = (GWChat*)(m_pChatPackets[uIndex]) ;
		switch( pChat->GetChatType() )
		{
		case CHAT_TYPE_CHANNEL:
			{
				if( pChat->GetChannelID() == pUser->GetChannelID() )
				{
					GUID_t SourGUID = pChat->GetSourGUID() ;

					USER* pSourUser = g_pOnlineUser->FindUser( SourGUID ) ;
					if( pSourUser==NULL )
					{
						Assert(FALSE) ;
						break ;
					}

					ID_t ServerID = pUser->GetServerID() ;
					ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
					if( pServerPlayer==NULL )
					{
						Assert(FALSE) ;
						break ;
					}
					WGChat Msg ;
					Msg.SetPlayerID( pUser->GetPlayerID() ) ;
					Msg.SetChatType( pChat->GetChatType() ) ;
					Msg.SetContexSize( pChat->GetContexSize() ) ;
					Msg.SetContex( pChat->GetContex() ) ;
					Msg.SetSourNameSize( (BYTE)(strlen(pSourUser->GetName())) ) ;
					Msg.SetSourName( pSourUser->GetName() ) ;
					Msg.SetCampID( pSourUser->GetUserCampData()->m_nCampID );
					Msg.SetWorldChatID( 0 ) ;

					pServerPlayer->SendPacket( &Msg ) ;
				}
			}
			break ;
		case CHAT_TYPE_GUILD:
			{
				if( pChat->GetGuildID() == pUser->GetGuildID() )
				{
					GUID_t SourGUID = pChat->GetSourGUID() ;

					USER* pSourUser = g_pOnlineUser->FindUser( SourGUID ) ;
					if( pSourUser==NULL )
					{
						Assert(FALSE) ;
						break ;
					}

					ID_t ServerID = pUser->GetServerID() ;
					ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
					if( pServerPlayer==NULL )
					{
						Assert(FALSE) ;
						break ;
					}
					WGChat Msg ;
					Msg.SetPlayerID( pUser->GetPlayerID() ) ;
					Msg.SetChatType( pChat->GetChatType() ) ;
					Msg.SetContexSize( pChat->GetContexSize() ) ;
					Msg.SetContex( pChat->GetContex() ) ;
					Msg.SetSourNameSize( (BYTE)(strlen(pSourUser->GetName())) ) ;
					Msg.SetSourName( pSourUser->GetName() ) ;
					Msg.SetCampID( pSourUser->GetUserCampData()->m_nCampID );
					Msg.SetWorldChatID( 0 ) ;

					pServerPlayer->SendPacket( &Msg ) ;
				}
			}
			break ;
		case CHAT_TYPE_MENPAI:
			{
				if( pChat->GetMenpaiID() == pUser->GetMenpai() )
				{
					GUID_t SourGUID = pChat->GetSourGUID() ;

					USER* pSourUser = g_pOnlineUser->FindUser( SourGUID ) ;
					if( pSourUser==NULL )
					{
						Assert(FALSE) ;
						break ;
					}

					ID_t ServerID = pUser->GetServerID() ;
					ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
					if( pServerPlayer==NULL )
					{
						Assert(FALSE) ;
						break ;
					}
					WGChat Msg ;
					Msg.SetPlayerID( pUser->GetPlayerID() ) ;
					Msg.SetChatType( pChat->GetChatType() ) ;
					Msg.SetContexSize( pChat->GetContexSize() ) ;
					Msg.SetContex( pChat->GetContex() ) ;
					Msg.SetSourNameSize( (BYTE)(strlen(pSourUser->GetName())) ) ;
					Msg.SetSourName( pSourUser->GetName() ) ;
					Msg.SetCampID( pSourUser->GetUserCampData()->m_nCampID );
					Msg.SetWorldChatID( 0 ) ;

					pServerPlayer->SendPacket( &Msg ) ;
				}
			}
			break ;
		case CHAT_TYPE_SYSTEM:
			{
				if( TRUE )
				{
					GUID_t SourGUID = pChat->GetSourGUID() ;

					USER* pSourUser = g_pOnlineUser->FindUser( SourGUID ) ;
					if( pSourUser==NULL )
					{
						Assert(FALSE) ;
						break ;
					}

					ID_t ServerID = pUser->GetServerID() ;
					ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
					if( pServerPlayer==NULL )
					{
						Assert(FALSE) ;
						break ;
					}
					WGChat Msg ;
					Msg.SetPlayerID( pUser->GetPlayerID() ) ;
					Msg.SetChatType( pChat->GetChatType() ) ;
					Msg.SetContexSize( pChat->GetContexSize() ) ;
					Msg.SetContex( pChat->GetContex() ) ;
					Msg.SetSourNameSize( (BYTE)(strlen(pSourUser->GetName())) ) ;
					Msg.SetSourName( pSourUser->GetName() ) ;
					Msg.SetCampID( pSourUser->GetUserCampData()->m_nCampID );
					Msg.SetWorldChatID( 0 ) ;

					pServerPlayer->SendPacket( &Msg ) ;
				}
			}
			break ;
		case CHAT_TYPE_TEAM:
			{
				if( pChat->GetTeamID() == pUser->GetTeamID() )
				{
					GUID_t SourGUID = pChat->GetSourGUID() ;

					USER* pSourUser = g_pOnlineUser->FindUser( SourGUID ) ;
					if( pSourUser==NULL )
					{
						Assert(FALSE) ;
						break ;
					}

					ID_t ServerID = pUser->GetServerID() ;
					ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
					if( pServerPlayer==NULL )
					{
						Assert(FALSE) ;
						break ;
					}
					WGChat Msg ;
					Msg.SetPlayerID( pUser->GetPlayerID() ) ;
					Msg.SetChatType( pChat->GetChatType() ) ;
					Msg.SetContexSize( pChat->GetContexSize() ) ;
					Msg.SetContex( pChat->GetContex() ) ;
					Msg.SetSourNameSize( (BYTE)(strlen(pSourUser->GetName())) ) ;
					Msg.SetSourName( pSourUser->GetName() ) ;
					Msg.SetCampID( pSourUser->GetUserCampData()->m_nCampID );
					Msg.SetWorldChatID( 0 ) ;

					pServerPlayer->SendPacket( &Msg ) ;
				}
			}
			break ;
		case CHAT_TYPE_TELL:
			{
				CHAR* pTarName = pChat->GetTargetName() ;
				if( strcmp(pTarName, pUser->GetName())==0 )
				{
					GUID_t SourGUID = pChat->GetSourGUID() ;

					USER* pSourUser = g_pOnlineUser->FindUser( SourGUID ) ;
					if( pSourUser==NULL )
					{
						Assert(FALSE) ;
						break ;
					}

					ID_t ServerID = pUser->GetServerID() ;
					ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
					if( pServerPlayer==NULL )
					{
						Assert(FALSE) ;
						break ;
					}
					WGChat Msg ;
					Msg.SetPlayerID( pUser->GetPlayerID() ) ;
					Msg.SetChatType( pChat->GetChatType() ) ;
					Msg.SetContexSize( pChat->GetContexSize() ) ;
					Msg.SetContex( pChat->GetContex() ) ;
					Msg.SetSourNameSize( (BYTE)(strlen(pSourUser->GetName())) ) ;
					Msg.SetSourName( pSourUser->GetName() ) ;
					Msg.SetCampID( pSourUser->GetUserCampData()->m_nCampID );
					Msg.SetWorldChatID( 0 ) ;

					pServerPlayer->SendPacket( &Msg ) ;
				}
			}
			break ;
		default :
			{
			}
			break ;
		};



		uIndex ++ ;
	}

__LEAVE_FUNCTION
}
