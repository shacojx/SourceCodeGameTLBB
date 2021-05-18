#include "stdafx.h"


#include "GWCommand.h"
#include "Log.h"
#include "OnlineUser.h"
#include "ServerPlayer.h"
#include "WGChat.h"




uint GWCommandHandler::Execute( GWCommand* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer ;

	CHAR szName[] = "信息查询" ;
	CHAR szContex[256] ;
	memset( szContex, 0, 256 ) ;
	switch( pPacket->GetServerCommand()->m_Command )
	{
	case COMMAND_TYPE_ALLWHO://获取当前游戏世界总人信息及一些角色信息
		{
			//全局信息
			sprintf( szContex, "Total:%d", g_pOnlineUser->GetTotalCount() ) ;
			//随机取部分玩家信息
			INT fromindex = rand()%MAX_WORLD_USER ;
			INT cansend=5 ;
			for( int i=0; i<MAX_WORLD_USER; i++ )
			{
				fromindex++ ;
				if( fromindex>=MAX_WORLD_USER ) fromindex = 0;

				USER* pUser = g_pOnlineUser->GetUserByIndex(fromindex) ;
				if( pUser==NULL || pUser->UserStatus()==US_NONE )
					continue;
				
				CHAR szUser[64] ;
				sprintf( szUser, "\r\n(%s:%X:%d)",
					pUser->GetName(), 
					pUser->GetGUID(),
					pUser->GetSceneID() ) ;
				strcat( szContex, szUser ) ;
				cansend -- ;
				if( cansend<=0 )
					break ;
			}


			WGChat Msg ;
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetChatType( CHAT_TYPE_TEAM ) ;
			Msg.SetSourName( szName ) ;
			Msg.SetSourNameSize( (BYTE)strlen(szName) ) ;
			Msg.SetContex( szContex ) ;
			Msg.SetContexSize( (BYTE)strlen(szContex) ) ;
			Msg.SetCampID( INVALID_CAMP );

			pServerPlayer->SendPacket( &Msg ) ;
		}
		break ;
	case COMMAND_TYPE_CATCHPLAYER://获取某个玩家的基本信息
		{
			if( pPacket->GetServerCommand()->m_Param0==0 )
			{//按照名字查询
				USER* pUser = g_pOnlineUser->FindUser(pPacket->GetServerCommand()->m_szName) ;
				if( pUser==NULL )
				{
					sprintf( szContex, "Cannot find name=%s player", pPacket->GetServerCommand()->m_szName ) ;
				}
				else
				{
					sprintf( szContex, "%X: SceneID=%d Level=%d Menpai=%d",
						pUser->GetGUID(), 
						pUser->GetSceneID(), 
						pUser->GetLevel(),
						pUser->GetMenpai() ) ;
				}
			}
			else if( pPacket->GetServerCommand()->m_Param0==1 )
			{//按照GUID查询
				USER* pUser = g_pOnlineUser->FindUser((GUID_t)pPacket->GetServerCommand()->m_Param1) ;
				if( pUser==NULL )
				{
					sprintf( szContex, "Cannot find GUID=%X player", pPacket->GetServerCommand()->m_Param1 ) ;
				}
				else
				{
					sprintf( szContex, "%s: SceneID=%d Level=%d Menpai=%d",
						pUser->GetName(), 
						pUser->GetSceneID(), 
						pUser->GetLevel(),
						pUser->GetMenpai() ) ;
				}
			}
			else
			{
				Assert(FALSE) ;
				break ;
			}

			WGChat Msg ;
			Msg.SetPlayerID( pPacket->GetPlayerID() ) ;
			Msg.SetChatType( CHAT_TYPE_TEAM ) ;
			Msg.SetSourName( szName ) ;
			Msg.SetSourNameSize( (BYTE)strlen(szName) ) ;
			Msg.SetContex( szContex ) ;
			Msg.SetContexSize( (BYTE)strlen(szContex) ) ;
			Msg.SetCampID( INVALID_CAMP );

			pServerPlayer->SendPacket( &Msg ) ;
		}
		break ;
	default :
		{
		}
		break ;
	};

	Log::SaveLog( WORLD_LOGFILE, "GWCommandHandler...PlayerID=%d Command=%d", 
		pPacket->GetPlayerID(), 
		pPacket->GetServerCommand()->m_Command ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
