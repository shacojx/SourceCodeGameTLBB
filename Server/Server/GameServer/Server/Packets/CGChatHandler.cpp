#include "stdafx.h"


#include "CGChat.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "ChatPipe.h"
#include "GCChat.h"
#include "GWChat.h"
#include "GameTable.h"



uint CGChatHandler::Execute( CGChat* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer != NULL ) ;
	if ( pGamePlayer == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGChatHandler: Error (pGamePlayer == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman != NULL ) ;
	if ( pHuman == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGChatHandler: Error (pHuman == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Scene* pScene = pHuman->getScene() ;
	Assert( pScene != NULL ) ;
	if ( pScene == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGChatHandler: Error Obj = %d (pScene == NULL) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	INT i ;
	BOOL bIsCanSay = TRUE ;
	BYTE bChatType = pPacket->GetChatType() ;
	//消耗判断
	for( i=0; i<g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeCount; i++ )
	{
		if( bIsCanSay==FALSE )
			break ;

		switch( g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeType[i] )
		{
		case CHAT_NEED_MP:
			{
				INT CurMP = pHuman->GetMP() ;
				INT NeedMP = g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeValue[i] ;
				if( CurMP<NeedMP )
					bIsCanSay = FALSE ;
			}
			break ;
		//case CHAT_NEED_VIGOR:
		//	{
		//		INT CurVigor = pHuman->GetVigor() ;
		//		INT NeedVigor = g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeValue[i] ;
		//		if( CurVigor<NeedVigor )
		//			bIsCanSay = FALSE ;
		//	}
		//	break ;
		//case CHAT_NEED_ENERGY:
		//	{
		//		INT CurEnergy = pHuman->GetEnergy() ;
		//		INT NeedEnergy = g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeValue[i] ;
		//		if( CurEnergy<NeedEnergy )
		//			bIsCanSay = FALSE ;
		//	}
		//	break ;
		default :
			break ;
		};
	}

	if( bIsCanSay == TRUE )//等级判断
	{
		if( pHuman->GetLevel() < g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_NeedLevel )
		{
			bIsCanSay = FALSE ;
		}
	}

	if( bIsCanSay == FALSE )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGChatHandler: Consume Error! GUID=%X ChatType=%d Contex=%s TarName=%s TeamID=%d ChannelID=%d GuildID=%d PID=%d", 
			pGamePlayer->m_HumanGUID, 
			pPacket->GetChatType(), 
			pPacket->GetContex(), 
			pPacket->GetTargetName(), 
			pPacket->GetTeamID(), 
			pPacket->GetChannelID(),
			pPacket->GetGuildID(),
			pGamePlayer->PlayerID()) ;

		return PACKET_EXE_CONTINUE ;
	}

	for( i=0; i<g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeCount; i++ )
	{
		switch( g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeType[i] )
		{
		case CHAT_NEED_MP:
			{
				INT CurMP = pHuman->GetMP() ;
				INT NeedMP = g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeValue[i] ;
				pHuman->SetMP( CurMP-NeedMP ) ;
			}
			break ;
		//case CHAT_NEED_VIGOR:
		//	{
		//		INT CurVigor = pHuman->GetVigor() ;
		//		INT NeedVigor = g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeValue[i] ;
		//		pHuman->SetVigor( CurVigor-NeedVigor ) ;
		//	}
		//	break ;
		//case CHAT_NEED_ENERGY:
		//	{
		//		INT CurEnergy = pHuman->GetEnergy() ;
		//		INT NeedEnergy = g_ChatConsumeTbl.m_ChatComsumeType[bChatType].m_ConsumeValue[i] ;
		//		pHuman->SetEnergy( CurEnergy-NeedEnergy ) ;
		//	}
		//	break ;
		default :
			break ;
		};
	}

	CHAR* pContex = pPacket->GetContex();
	BYTE nContexSize = pPacket->GetContexSize();
	if( nContexSize >= MAX_CHAT_SIZE ) nContexSize = MAX_CHAT_SIZE-1;
	pContex[MAX_CHAT_SIZE-1] = 0;
	ReplaceIllegalString( pPacket->GetContex(), nContexSize );

	switch( bChatType )
	{
	case CHAT_TYPE_NORMAL:
		{
			ObjID_t SourID = pHuman->GetID() ;
			ObjID_t DestID = INVALID_ID ;
			GCChat* pChatPacket = (GCChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CHAT)) ;

			pChatPacket->SetChatType( pPacket->GetChatType() ) ;
			pChatPacket->SetContexSize( nContexSize ) ;
			pChatPacket->SetContex( pPacket->GetContex() ) ;
			pChatPacket->SetSourNameSize( (BYTE)(strlen(pHuman->GetName())) ) ;
			pChatPacket->SetSourName( pHuman->GetName() ) ;
			pChatPacket->SetSourID(pHuman->GetID()) ;
			pChatPacket->SetSourCamp(pHuman->GetCampData()->m_nCampID);

			pScene->GetChatPipe()->SendPacket( pChatPacket, SourID, DestID ) ;
		}
		break ;
	case CHAT_TYPE_TEAM:
		{
			GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;

			pChatPacket->SetSourGUID( pGamePlayer->m_HumanGUID ) ;
			pChatPacket->SetChatType( pPacket->GetChatType() ) ;
			pChatPacket->SetContexSize( nContexSize ) ;
			pChatPacket->SetContex( pPacket->GetContex() ) ;
			pChatPacket->SetTeamID( pPacket->GetTeamID() ) ;

			g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;
		}
		break ;
	case CHAT_TYPE_TELL:
		{
			GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;

			pChatPacket->SetSourGUID( pGamePlayer->m_HumanGUID ) ;
			pChatPacket->SetChatType( pPacket->GetChatType() ) ;
			pChatPacket->SetContexSize( nContexSize ) ;
			pChatPacket->SetContex( pPacket->GetContex() ) ;
			pChatPacket->SetTargetName( pPacket->GetTargetName() ) ;
			pChatPacket->SetTargetSize( pPacket->GetTargetSize() ) ;

			g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;
		}
		break ;
	case CHAT_TYPE_SCENE:
	case CHAT_TYPE_SYSTEM:
		{
			char sysNotifyMsg[MAX_CHAT_SIZE]={"系统:你消耗了一个道具!"};
			int nItemIndex=g_Config.m_ConfigInfo.m_nWorldChatItemIndex;
			int nBagIndex=INVALID_ID;
			BOOL bContiune=1;

			Item* pItem =NULL;
			if(INVALID_ID==nBagIndex)
			{
				nBagIndex = HumanItemLogic::GetItemPosByType(pHuman, nItemIndex);
				if(nBagIndex == -1)
				{
					memset(sysNotifyMsg,0,sizeof(sysNotifyMsg));
					strcpy(sysNotifyMsg,"系统:对不起！,你没有道具!");
					bContiune=0;

				}
			}
           
			if(bContiune)
			{
				pItem = HumanItemLogic::GetItem(pHuman, nBagIndex);
				if(NULL==pItem)
				{
					memset(sysNotifyMsg,0,sizeof(sysNotifyMsg));
					strcpy(sysNotifyMsg,"系统:对不起！,你没有道具!");
					bContiune=0;
				}

			}

			if(bContiune)
			{
				if(pItem->IsCosSelf())
				{
					if(pItem->GetLayedNum()<1)
					{
						memset(sysNotifyMsg,0,sizeof(sysNotifyMsg));
						strcpy(sysNotifyMsg,"系统:对不起！,你没有道具!");
						bContiune=0;

					}
					else
					{
						ItemContainer*	pContainer = HumanItemLogic::GetContainer(pHuman,nBagIndex);
						if(NULL==pContainer)
						{
							AssertEx(FALSE,"[Obj_Human::Skill_DepleteItem]: Can't find the specified container!!");
							return FALSE;
						}
						HumanItemLogic::DecItemLayCount(pHuman,nBagIndex,1);

					}
				
				}

			}
		
			//发送世界频道聊天

			if(bContiune)
			{
				GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;
				pChatPacket->SetSourGUID( pGamePlayer->m_HumanGUID ) ;
				pChatPacket->SetChatType( pPacket->GetChatType() ) ;
				pChatPacket->SetContexSize( pPacket->GetContexSize() ) ;
				pChatPacket->SetContex( pPacket->GetContex() ) ;

				g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;

			}
			
			/////提示信息；
        	ObjID_t SourID = pHuman->GetID() ;
			ObjID_t DestID = INVALID_ID ;
			GCChat* pChatPacketinfo = (GCChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_CHAT)) ;

			pChatPacketinfo->SetChatType( CHAT_TYPE_SYSTEM ) ;
			pChatPacketinfo->SetContexSize((BYTE)strlen(sysNotifyMsg)) ;
			pChatPacketinfo->SetContex( sysNotifyMsg ) ;
			pChatPacketinfo->SetSourNameSize( (BYTE)(strlen(pHuman->GetName())) ) ;
			pChatPacketinfo->SetSourName( pHuman->GetName() ) ;
			pChatPacketinfo->SetSourID(pHuman->GetID()) ;
			pChatPacketinfo->SetSourCamp(pHuman->GetCampData()->m_nCampID);

			pScene->GetChatPipe()->SendPacket( pChatPacketinfo, SourID, DestID ) ;


		
			
		}
		break ;
	case CHAT_TYPE_CHANNEL:
		{
			GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;

			pChatPacket->SetSourGUID( pGamePlayer->m_HumanGUID ) ;
			pChatPacket->SetChatType( pPacket->GetChatType() ) ;
			pChatPacket->SetContexSize( nContexSize ) ;
			pChatPacket->SetContex( pPacket->GetContex() ) ;
			pChatPacket->SetChannelID( pPacket->GetChannelID() ) ;

			g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;
		}
		break ;
	case CHAT_TYPE_GUILD:
		{
			GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;

			pChatPacket->SetSourGUID( pGamePlayer->m_HumanGUID ) ;
			pChatPacket->SetChatType( pPacket->GetChatType() ) ;
			pChatPacket->SetContexSize( nContexSize ) ;
			pChatPacket->SetContex( pPacket->GetContex() ) ;
			pChatPacket->SetGuildID( pPacket->GetGuildID() ) ;

			g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;
		}
		break ;
	case CHAT_TYPE_MENPAI:
		{
			GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;

			pChatPacket->SetSourGUID( pGamePlayer->m_HumanGUID ) ;
			pChatPacket->SetChatType( pPacket->GetChatType() ) ;
			pChatPacket->SetContexSize( nContexSize ) ;
			pChatPacket->SetContex( pPacket->GetContex() ) ;
			pChatPacket->SetMenpaiID( pPacket->GetMenpaiID() ) ;

			g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;
		}
		break ;
	default :
		{
			Assert( FALSE ) ;
		}
		break ;
	};

	g_pLog->FastSaveLog( LOG_FILE_12, "CGChatHandler: GUID=%X ChatType=%d Contex=%s TarName=%s TeamID=%d ChannelID=%d GuildID=%d PID=%d", 
		pGamePlayer->m_HumanGUID, 
		pPacket->GetChatType(), 
		pPacket->GetContex(), 
		pPacket->GetTargetName(), 
		pPacket->GetTeamID(), 
		pPacket->GetChannelID(),
		pPacket->GetGuildID(),
		pGamePlayer->PlayerID()) ;


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
