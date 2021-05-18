#include "stdafx.h"


#include "GWChat.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "ServerManager.h"

#include "WGChat.h"
#include "Team.h"

#include "ChatCenter.h"
#include "PacketFactoryManager.h"


uint GWChatHandler::Execute( GWChat* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pSourServerPlayer = (ServerPlayer*)pPlayer ;

	GUID_t SourGUID = pPacket->GetSourGUID() ;

	USER* pSourUser = g_pOnlineUser->FindUser( SourGUID ) ;
	if( pSourUser==NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWChatHandler...User Sour GUID=%X not find!", 
			SourGUID ) ;
		return PACKET_EXE_CONTINUE ;
	}

	WGChat Msg ;
	Msg.SetChatType( pPacket->GetChatType() ) ;
	Msg.SetContexSize( pPacket->GetContexSize() ) ;
	Msg.SetContex( pPacket->GetContex() ) ;
	Msg.SetSourNameSize( (BYTE)(strlen(pSourUser->GetName())) ) ;
	Msg.SetSourName( pSourUser->GetName() ) ;
	Msg.SetCampID( pSourUser->GetUserCampData()->m_nCampID );
	Msg.SetWorldChatID( g_pChatCenter->NextWorldChatID() ) ;

	switch( pPacket->GetChatType() )
	{
	case CHAT_TYPE_NORMAL:
		{
			Assert(FALSE) ;
		}
		break ;
	case CHAT_TYPE_GUILD:
		{
			Msg.SetGuildID( pPacket->GetGuildID() ) ;
			g_pServerManager->BroadCastServer( &Msg ) ;
		}
		break ;
	case CHAT_TYPE_MENPAI:
		{
			Msg.SetMenpaiID( pPacket->GetMenpaiID() ) ;
			g_pServerManager->BroadCastServer( &Msg ) ;
		}
		break ;
	case CHAT_TYPE_SYSTEM:
	case CHAT_TYPE_SCENE:
		{
			g_pServerManager->BroadCastServer( &Msg ) ;
		}
		break ;
	case CHAT_TYPE_TEAM:
		{
			Team* pTeam = g_pTeamList->GetTeam( pSourUser->GetTeamID() ) ;
			if( pTeam==NULL )
				break ;

			for( int i=0; i<pTeam->MemberCount(); i++ )
			{
				TEAMMEMBER* pMember = pTeam->Member( i ) ;
				if( pMember==NULL )
				{
					Assert(FALSE) ;
					continue ;
				}

				USER* pUser = g_pOnlineUser->FindUser( pMember->m_Member ) ;
				if( pUser==NULL )
				{//如果队员离线,则用户数据是空
					continue ;
				}
				
				ID_t ServerID = pUser->GetServerID() ;
				ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
				if( pServerPlayer==NULL )
				{
					Assert(FALSE) ;
					continue ;
				}

				Msg.SetPlayerID( pUser->GetPlayerID() ) ;

				pServerPlayer->SendPacket( &Msg ) ;
			}
		}
		break ;
	case CHAT_TYPE_TELL:
		{
			USER* pUser = g_pOnlineUser->FindUser( pPacket->GetTargetName() ) ;

			if( pUser )
			{
				if( pSourUser->IsEnemy( pUser->GetUserCampData() ) )
					pUser = NULL;	//如果是敌对阵营，处理方法跟查无此人相同!!
									//按道理说是不能密的哦～～
			}

			if( pUser==NULL )
			{//如果队员离线,则用户数据是空
				CHAR* szName="system" ;
				CHAR szContex[32] ;
				sprintf( szContex, "@@%s", pPacket->GetTargetName() ) ;

				WGChat MsgR ;
				MsgR.SetPlayerID( pSourUser->GetPlayerID() ) ;
				MsgR.SetChatType( CHAT_TYPE_SELF ) ;
				MsgR.SetContexSize( sizeof(szContex) ) ;
				MsgR.SetContex( szContex ) ;
				MsgR.SetSourNameSize( (BYTE)(strlen(szName)) ) ;
				MsgR.SetSourName( szName ) ;
				MsgR.SetWorldChatID( g_pChatCenter->NextWorldChatID() ) ;
				MsgR.SetCampID( pSourUser->GetUserCampData()->m_nCampID );
				
				pSourServerPlayer->SendPacket( &MsgR ) ;

				break ;
			}
				
			ID_t ServerID = pUser->GetServerID() ;
			ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
			if( pServerPlayer==NULL )
			{
				Assert(FALSE) ;
				break ;
			}

			Msg.SetPlayerID( pUser->GetPlayerID() ) ;

			pServerPlayer->SendPacket( &Msg ) ;
		}
		break ;
	case CHAT_TYPE_CHANNEL:
		{
			ChatChannel* pChannel = g_pChatCenter->GetChatChannel( pPacket->GetChannelID() ) ;
			if( pChannel==NULL )
				break ;

			for( int i=0; i<pChannel->MemberCount(); i++ )
			{
				GUID_t memberguid = pChannel->Member(i) ;
				if( memberguid==INVALID_ID )
				{
					Assert(FALSE) ;
					continue ;
				}

				USER* pUser = g_pOnlineUser->FindUser( memberguid ) ;
				if( pUser==NULL )
				{//如果队员离线,则用户数据是空
					continue ;
				}
				
				ID_t ServerID = pUser->GetServerID() ;
				ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
				if( pServerPlayer==NULL )
				{
					Assert(FALSE) ;
					continue ;
				}

				Msg.SetPlayerID( pUser->GetPlayerID() ) ;

				pServerPlayer->SendPacket( &Msg ) ;
			}
		}
		break ;
	default :
		{
			Assert(FALSE) ;
		}
		break ;
	};

	g_pChatCenter->PushChatPacket( pPacket ) ;

	Log::SaveLog( WORLD_LOGFILE, "GWChatHandler...GUID=%X ChatType=%d Contex=%s", 
		SourGUID, pPacket->GetChatType(), pPacket->GetContex() ) ;

	return PACKET_EXE_NOTREMOVE ;//不能删除pPacket, 已经放入预存管道

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
