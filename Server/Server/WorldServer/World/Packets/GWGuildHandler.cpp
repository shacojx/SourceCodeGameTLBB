#include "stdafx.h"
/*
帮派事件处理接口
*/
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "OnlineUser.h"
#include "GuildManager.h"
#include "Log.h"
#include "GWGuild.h"
#include "WGGuildReturn.h"
#include "WGGuildError.h"
#include "WGGuild.h"
#include "City.h"
#include "MailCenter.h"
#include "TimeManager.h"


uint	GWGuildHandler::Execute(GWGuild* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
	GUID_t guid = pPacket->GetGUID();

	USER* pUser = g_pOnlineUser->FindUser( guid );
	if( pUser == NULL )
	{
		Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X not find!", 
			guid );
		return PACKET_EXE_CONTINUE;
	}

	GuildID_t guildID = pUser->GetGuildID();
	_GUILD_CGW_PACKET* pGuildPacket = pPacket->GetGuildPacket();
	GUILD_ERROR_TYPE GuildErr = GUILD_ERROR;

	switch( pGuildPacket->m_uPacketType )
	{
	case GUILD_PACKET_GW_ASKLIST:
		{ // 返回帮会信息列表
			GUILD_CGW_ASKLIST* pAskList = (GUILD_CGW_ASKLIST*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_ASKLIST));

			WGGuild Msg;
			Msg.SetPlayerID( pUser->GetPlayerID() );
			Msg.SetGUID(guid);
			Msg.GetGuildPacket()->m_uPacketType = GUILD_PACKET_WG_LIST;
			GUILD_WGC_LIST* pGuildList = (GUILD_WGC_LIST*)(Msg.GetGuildPacket()->GetPacket(GUILD_PACKET_WG_LIST));
			Assert( pGuildList );

			if( g_pGuildManager->GetGuildList( pGuildList, pAskList->m_uStart, pAskList->m_Camp ) )
			{
				pServerPlayer->SendPacket( &Msg );
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X get guild list!", 
					guid );
				return PACKET_EXE_CONTINUE;
			}

			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X Get Guild info failed!", 
				guid );
		}
		break;
	case GUILD_PACKET_GW_CREATE:
		{
			if( guildID != INVALID_ID )
			{ // 已有帮会，这里应该已经由 Server 正确处理，所以不返回错误结果
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to create an extra guild!", 
					guid );
				GuildErr = GUILD_ERROR_GUILD_ALREADY_EXIST;
				break;
			}

			GUILD_CGW_CREATE* pGuildCreate = (GUILD_CGW_CREATE*)pGuildPacket->GetPacket(GUILD_PACKET_GW_CREATE);
			Assert( pGuildCreate );

			GuildErr = g_pGuildManager->CreateGuild(pUser, pGuildCreate->m_szGuildName, pGuildCreate->m_Camp);
			if( GuildErr == GUILD_ERROR_NOTHING )
			{ // 创建成功

				//全世界所有帮会中的申请列表遍历一边，呵呵，够狠
				for( INT i=0; i<MAX_GUILD_SIZE; i++ )
				{
					Guild* pDeleteGuild = g_pGuildManager->GetGuild( i );
					if(pDeleteGuild->GetGuildID() != INVALID_ID && pDeleteGuild->GetGuildID() != pUser->GetGuildID() )
					{
						for(INT j = 0; j<USER_ARRAY_SIZE;j++)
						{
							GUILDUSER* pDeleteUser = pDeleteGuild->GetGuildUserByIndex(j);
							if(pDeleteUser->m_UserGUID != INVALID_ID)
							{
								if(pDeleteUser->m_UserGUID == guid
									&&pDeleteUser->m_Position == GUILD_POSITION_TRAINEE)
								{//删掉此人
									pDeleteGuild->OnUserLeaveByIndex(j);
									break;
								}
							}
						}
					}
				}

				//恭喜你申请的%s帮会创建成功，可使用快捷键字母O或通过人物属性界面的“帮会”查看和管理你的帮会。
				CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
				sprintf(szMailContent, "恭喜你申请的%s帮会创建成功，可使用快捷键字母O或通过人物属性界面的“帮会”查看和管理你的帮会。", pGuildCreate->m_szGuildName);
				g_pMailCenter->SendNormalMail(pUser->GetName(), szMailContent);

				WGGuildReturn Msg;
				Msg.SetPlayerID( pUser->GetPlayerID() );
				Msg.SetGUID(guid);
				Guild* pGuild = g_pGuildManager->GetGuild(pUser->GetGuildID());
				pGuild->SetGuildDesc(pGuildCreate->m_szGuildDesc);
				_GUILD_RETURN GuildReturn;
				GuildReturn.m_GuildID = pUser->GetGuildID();
				GuildReturn.m_ReturnType = GUILD_RETURN_CREATE;
				GuildReturn.m_GuildNameSize = (UCHAR)strlen(pGuild->GetGuildName());
				strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), GuildReturn.m_GuildNameSize);

				Msg.SetGuildReturn( &GuildReturn );
				pServerPlayer->SendPacket( &Msg );
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X created a guild success!", 
					guid );
				return PACKET_EXE_CONTINUE;
			}

			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X created a guild failed!", 
				guid );
		}
		break;
	case GUILD_PACKET_GW_JOIN:
		{
			if( guildID != INVALID_ID )
			{ // 已有帮会，这里应该已经由 Server 正确处理，所以不返回错误结果
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to join an extra guild!", 
					guid );
				GuildErr = GUILD_ERROR_IN_GUILD;
				break;
			}

			GUILD_CGW_JOIN* pGuildJoin = (GUILD_CGW_JOIN*)pGuildPacket->GetPacket(GUILD_PACKET_GW_JOIN);

			Guild* pGuild = g_pGuildManager->GetGuild( pGuildJoin->m_GuildGUID );
			if( pGuild == NULL )
			{
				Assert(FALSE);
				GuildErr = GUILD_ERROR_NOT_EXIST;
			}
			else
			{
				switch( pGuild->GetGuildStatus() )
				{
				case GUILD_STATUS_NORMAL:
					{
						GuildErr = pGuild->OnUserEnter(pUser, pGuildJoin->m_Camp);
						if( GuildErr == GUILD_ERROR_NOTHING )
						{ // 成功
							pUser->SetGuildID( pGuild->GetGuildID() );

							WGGuildReturn Msg;
							Msg.SetPlayerID( pUser->GetPlayerID() );
							Msg.SetGUID(guid);
							_GUILD_RETURN GuildReturn;
							GuildReturn.m_ReturnType = GUILD_RETURN_JOIN;
							GuildReturn.m_GuildID = pUser->GetGuildID();
							GuildReturn.m_GuildNameSize = (UCHAR)strlen(pGuild->GetGuildName());
							strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), GuildReturn.m_GuildNameSize);
							Msg.SetGuildReturn( &GuildReturn );

							pServerPlayer->SendPacket( &Msg );
							Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X joined a guild!", 
								guid );
							return PACKET_EXE_CONTINUE;
						}
					}
					break;
				default:
					Assert(FALSE);
				}
			}
			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X join a guild failed!", 
				guid );
		}
		break;
	case GUILD_PACKET_GW_ASKINFO:
		{
			GUILD_CGW_ASKINFO* pAskInfo = (GUILD_CGW_ASKINFO*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_ASKINFO));
			Guild* pGuild = g_pGuildManager->GetGuild(pAskInfo->m_GuildGUID);
			// Assert(pGuild);
			if(!pGuild)
			{
				/*Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Invalid guild!", 
					pUser->GetName() );*/
				return PACKET_EXE_CONTINUE;
			}
			const GUILDUSER*	pGuildUser = pGuild->GetGuildUser(guid);
			Assert(pGuildUser);
			if(!pGuildUser)
			{
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Invalid User!", 
					pUser->GetName() );
				return PACKET_EXE_CONTINUE;
			}

			//根据请求类型生成不同的包
			if(pAskInfo->m_Type == GUILD_CGW_ASKINFO::GUILD_SELF_INFO)
			{//本人帮派信息,人物初始化时请求
				WGGuild Msg;
				Msg.SetPlayerID( pUser->GetPlayerID() );
				Msg.SetGUID(guid);

				if(!pGuild->CanDoIt(Guild::OPT_ASK_SELF_INFO, pGuildUser->m_Position, (GUILD_AUTHORITY)pGuildUser->m_uAccess))
				{
					Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Can Not Get Self Guild Info!", 
						pUser->GetName() );
					return PACKET_EXE_CONTINUE;
				}

				Msg.GetGuildPacket()->m_uPacketType = GUILD_PACKET_WG_SELF_GUILD_INFO;
				GUILD_WGC_SELF_GUILD_INFO* pSelfGuildInfo = (GUILD_WGC_SELF_GUILD_INFO*)(Msg.GetGuildPacket()->GetPacket(GUILD_PACKET_WG_SELF_GUILD_INFO));

				//当前只需要帮派名
				memcpy(pSelfGuildInfo->m_GuildName, pGuild->GetGuildName(),MAX_GUILD_NAME_SIZE );
				pSelfGuildInfo->m_GuildName[MAX_GUILD_NAME_SIZE - 1] = 0;
				pSelfGuildInfo->m_GuildNameSize = (BYTE)strlen(pSelfGuildInfo->m_GuildName);
				pSelfGuildInfo->m_bAccess = pGuildUser->m_uAccess;

				pServerPlayer->SendPacket( &Msg );
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s GUILD_PACKET_WG_SELF_GUILD_INFO!", 
					pUser->GetName() );
				return PACKET_EXE_CONTINUE;
			}
			else if(pAskInfo->m_Type == GUILD_CGW_ASKINFO::GUILD_MEMBER_INFO)
			{//成员列表
				WGGuild Msg;
				Msg.SetPlayerID( pUser->GetPlayerID() );
				Msg.SetGUID(guid);

				if(!pGuild->CanDoIt(Guild::OPT_ASK_MEMBER_LIST, pGuildUser->m_Position, (GUILD_AUTHORITY)pGuildUser->m_uAccess))
				{
					Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Can Not Get Member Info!", 
						pUser->GetName() );
					return PACKET_EXE_CONTINUE;
				}
				Msg.GetGuildPacket()->m_uPacketType = GUILD_PACKET_WG_MEMBER_LIST;
				GUILD_WGC_MEMBER_LIST* pMemberList = (GUILD_WGC_MEMBER_LIST*)(Msg.GetGuildPacket()->GetPacket(GUILD_PACKET_WG_MEMBER_LIST));

				memcpy(pMemberList->m_GuildDesc, pGuild->GetGuildDesc(),MAX_GUILD_DESC_SIZE );
				memcpy(pMemberList->m_GuildName, pGuild->GetGuildName(),MAX_GUILD_NAME_SIZE );
	
				pMemberList->m_uMemberCount =	0;
				pMemberList->m_uValidMemberCount = 0;
				pMemberList->m_uMemberMax	=	pGuild->GetGuildUserCount();
				pMemberList->m_bAccess		=	pGuildUser->m_uAccess;
				pMemberList->m_bPosition	=	pGuildUser->m_Position;

				INT CurNum = 0;
				for(INT i = 0; i<USER_ARRAY_SIZE; i++)
				{
					if(pGuild->GetGuildUserByIndex(i)->m_UserGUID != INVALID_ID)
					{
						const GUILDUSER* pUser = pGuild->GetGuildUserByIndex(i);
						pMemberList->m_GuildMemberData[CurNum].m_Guid			= pUser->m_UserGUID;
						memcpy(pMemberList->m_GuildMemberData[CurNum].m_szName, pUser->m_szUserName, MAX_CHARACTER_NAME);
						pMemberList->m_GuildMemberData[CurNum].m_bLevel			=	pUser->m_uLevel;
						pMemberList->m_GuildMemberData[CurNum].m_bMenPaiID		=	pUser->m_bMenPaiID;
						pMemberList->m_GuildMemberData[CurNum].m_iCurContribute	=	pUser->m_iCurContribute;
						pMemberList->m_GuildMemberData[CurNum].m_iMaxContribute	=	pUser->m_iMaxContribute;
						pMemberList->m_GuildMemberData[CurNum].m_iJoinTime		=	pUser->m_iJoinTime;
						pMemberList->m_GuildMemberData[CurNum].m_iLogOutTime	=	pUser->m_uLastLoginTime;
						pMemberList->m_GuildMemberData[CurNum].m_bIsOnline		=	pUser->m_bOnlineFlag;
						//如果在线
						if(pUser->m_bOnlineFlag && pUser->m_Position>GUILD_POSITION_TRAINEE)
						{
							pMemberList->m_uValidMemberCount++;							
						}
						pMemberList->m_GuildMemberData[CurNum].m_bPosition		=	(BYTE)pUser->m_Position;
						pMemberList->m_uMemberCount++;
						CurNum++;
					}
				}
				Assert(CurNum == pMemberList->m_uMemberCount);
				pServerPlayer->SendPacket( &Msg );
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s GUILD_PACKET_WG_MEMBER_LIST!", 
					pUser->GetName() );
				return PACKET_EXE_CONTINUE;
			}
			else if(pAskInfo->m_Type == GUILD_CGW_ASKINFO::GUILD_INFO)
			{//帮会信息
				WGGuild Msg;
				Msg.SetPlayerID( pUser->GetPlayerID() );
				Msg.SetGUID(guid);

				if(!pGuild->CanDoIt(Guild::OPT_ASK_DETAIL_INFO, pGuildUser->m_Position, (GUILD_AUTHORITY)pGuildUser->m_uAccess))
				{
					Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Can Not Get Guild Info!", 
						pUser->GetName() );
					return PACKET_EXE_CONTINUE;
				}

				Msg.GetGuildPacket()->m_uPacketType = GUILD_PACKET_WG_GUILD_INFO;
				GUILD_WGC_GUILD_INFO* pGuildInfo = (GUILD_WGC_GUILD_INFO*)(Msg.GetGuildPacket()->GetPacket(GUILD_PACKET_WG_GUILD_INFO));

				memcpy(pGuildInfo->m_GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME_SIZE);
				memcpy(pGuildInfo->m_GuildCreator, pGuild->GetCreatorName(), MAX_CHARACTER_NAME);
				memcpy(pGuildInfo->m_GuildChairMan, pGuild->GetGuildChiefName(), MAX_CHARACTER_NAME);
				if(pGuild->GetCity())
				{
					strncpy(pGuildInfo->m_CityName, pGuild->GetCity()->GetCityName(), MAX_CITY_NAME_SIZE);
					pGuildInfo->m_nPortSceneID	=	pGuild->GetCity()->GetPortSceneID();
				}
				else
				{
					memset(pGuildInfo->m_CityName, 0, MAX_CITY_NAME_SIZE);
					pGuildInfo->m_nPortSceneID = -1;
				}
				pGuildInfo->m_nLevel		=	pGuild->GetGuildLevel();
				pGuildInfo->m_MemNum		=	pGuild->GetGuildUserCount();
				pGuildInfo->m_Longevity		=	pGuild->GetLongevity();
				pGuildInfo->m_Contribute	=	pGuild->GetContribute();		//贡献度
				pGuildInfo->m_Honor			=	pGuild->GetHonor();				//人气
				pGuildInfo->m_FoundedMoney	=	pGuild->GetMoney();				//帮派资金
				pGuildInfo->m_nIndustryLevel=	pGuild->GetIndustry();			//工业度
				pGuildInfo->m_nAgrLevel		=	pGuild->GetAgr();				//农业度
				pGuildInfo->m_nComLevel		=	pGuild->GetCom();				//商业度
				pGuildInfo->m_nDefLevel		=	pGuild->GetDef();				//防卫度
				pGuildInfo->m_nTechLevel	=	pGuild->GetTech();				//科技度
				pGuildInfo->m_nAmbiLevel	=	pGuild->GetAmbi();				//扩张度
				pGuildInfo->m_bAccess		=	pGuildUser->m_uAccess;

				pServerPlayer->SendPacket( &Msg );
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s GUILD_PACKET_WG_GUILD_INFO!", 
					pUser->GetName() );
				return PACKET_EXE_CONTINUE;
			}
			else if(pAskInfo->m_Type == GUILD_CGW_ASKINFO::GUILD_APPOINT_POS)
			{
				WGGuild Msg;
				Msg.SetPlayerID( pUser->GetPlayerID() );
				Msg.SetGUID(guid);

				if(!pGuild->CanDoIt(Guild::OPT_ASK_APPOINT_POS_INFO, pGuildUser->m_Position, (GUILD_AUTHORITY)pGuildUser->m_uAccess))
				{
					Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Can Not Get Appoint Info!", 
						pUser->GetName() );
					return PACKET_EXE_CONTINUE;
				}

				Msg.GetGuildPacket()->m_uPacketType = GUILD_PACKET_WG_APPOINT_INFO;
				GUILD_WGC_APPOINT_INFO* pAppointInfo = (GUILD_WGC_APPOINT_INFO*)(Msg.GetGuildPacket()->GetPacket(GUILD_PACKET_WG_APPOINT_INFO));

				INT iPosNum = 0;
				for(INT i = GUILD_POSITION_MEMBER; i<=GUILD_POSITION_ASS_CHIEFTAIN; i++ )
				{//职位名全发过来
					pAppointInfo->m_PosList[iPosNum].m_PosID = i;
					if(pGuild->GetPosName(i))
					{
						memcpy(pAppointInfo->m_PosList[iPosNum].m_PosName, pGuild->GetPosName(i), MAX_GUILD_POS_NAME_SIZE);
						iPosNum++;
					}
				}
				pAppointInfo->iPosNum = iPosNum;
				pServerPlayer->SendPacket( &Msg );
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s GUILD_APPOINT_POS!", 
					pUser->GetName() );
				return PACKET_EXE_CONTINUE;
			}
		}
		break;
	case GUILD_PACKET_GW_APPOINT:
		{
			GUILD_CGW_APPOINT* pAppointInfo = (GUILD_CGW_APPOINT*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_APPOINT));
			Guild* pGuild = g_pGuildManager->GetGuild(pAppointInfo->m_GuildGUID);
			Assert(pGuild);
			if(!pGuild)
			{
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Invalid guild!", 
					pUser->GetName() );
				break;
			}

			const GUILDUSER*	pGuildUser = pGuild->GetGuildUser(guid);
			Assert(pGuildUser);
			if(!pGuildUser)
			{
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Invalid User!", 
					pUser->GetName() );
				break;
			}
	
			const GUILDUSER*	pCandGuildUser = pGuild->GetGuildUser(pAppointInfo->m_CandidateGUID);
			Assert(pCandGuildUser);
			if(!pCandGuildUser)
			{
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s Invalid User!", 
					pUser->GetName() );
				break;
			}

			if( pGuild->IsAuthorized(pUser->GetGUID(), GUILD_AUTHORITY_ASSIGN) == FALSE )
			{
				GuildErr = GUILD_ERROR_UNAUTHORIZED;
			}

			//如果任命自己
			if(pGuildUser == pCandGuildUser)
			{
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s appoint himself!", 
					pUser->GetName() );
				break;
			}

			GUILD_POSITION OldPos = pCandGuildUser->m_Position;
			
			if(OldPos == pAppointInfo->m_NewPosition)
			{
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User Name=%s No Change!", 
					pUser->GetName() );
				break;
			}

			GUILD_ADMIN_t::ORESULT bSuccess = pGuild->OnUserPositionChange(pAppointInfo->m_CandidateGUID, (GUILD_POSITION)pAppointInfo->m_NewPosition);
			if(bSuccess == GUILD_ADMIN_t::RET_SUCCESS)
			{
				WGGuildReturn Msg;
				_GUILD_RETURN GuildReturn;
				if(OldPos>pAppointInfo->m_NewPosition)
				{//降职
					GuildReturn.m_ReturnType = GUILD_RETURN_DEMOTE;
					GuildReturn.m_GuildID	=	pAppointInfo->m_GuildGUID;
					GuildReturn.m_GUID		=	pAppointInfo->m_CandidateGUID;
					GuildReturn.m_PosID		=	pAppointInfo->m_NewPosition;
					
					GuildReturn.m_SourNameSize = (UCHAR)strlen(pUser->GetName());
					strncpy((CHAR*)GuildReturn.m_SourName, pUser->GetName(), GuildReturn.m_SourNameSize);
					
					GuildReturn.m_DestNameSize = (UCHAR)strlen(pCandGuildUser->m_szUserName);
					strncpy((CHAR*)GuildReturn.m_DestName, pCandGuildUser->m_szUserName, GuildReturn.m_DestNameSize);
					
					GuildReturn.m_PositionNameSize = MAX_GUILD_POS_NAME_SIZE;
					strncpy((CHAR*)GuildReturn.m_PositionName, pGuild->GetPosName(pAppointInfo->m_NewPosition), GuildReturn.m_PositionNameSize);
					
					GuildReturn.m_GuildNameSize = (UCHAR)strlen(pGuild->GetGuildName());
					strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), GuildReturn.m_GuildNameSize);
					
					Msg.SetGuildReturn( &GuildReturn );

					//通知自己
					Msg.SetPlayerID( pUser->GetPlayerID() );
					Msg.SetGUID(guid);
					pServerPlayer->SendPacket( &Msg );

					//通知被任命的人
					USER* pNewUser = g_pOnlineUser->FindUser( pAppointInfo->m_CandidateGUID );
					if(pNewUser)
					{
						ServerPlayer* pTmpServerPlayer = NULL;
						pTmpServerPlayer = g_pServerManager->GetServerPlayer(pNewUser->GetServerID());
						if( pTmpServerPlayer != NULL )
						{
							Msg.SetPlayerID( pNewUser->GetPlayerID() );
							pTmpServerPlayer->SendPacket( &Msg );
						}
					}

				}
				else
				{//升职
					GuildReturn.m_ReturnType = GUILD_RETURN_PROMOTE;
					GuildReturn.m_GuildID	=	pAppointInfo->m_GuildGUID;
					GuildReturn.m_GUID		=	pAppointInfo->m_CandidateGUID;
					GuildReturn.m_PosID		=	pAppointInfo->m_NewPosition;
					
					GuildReturn.m_SourNameSize = (UCHAR)strlen(pUser->GetName());
					strncpy((CHAR*)GuildReturn.m_SourName, pUser->GetName(), GuildReturn.m_SourNameSize);
					
					GuildReturn.m_DestNameSize = (UCHAR)strlen(pCandGuildUser->m_szUserName);
					strncpy((CHAR*)GuildReturn.m_DestName, pCandGuildUser->m_szUserName, GuildReturn.m_DestNameSize);
					
					GuildReturn.m_PositionNameSize = MAX_GUILD_POS_NAME_SIZE;
					strncpy((CHAR*)GuildReturn.m_PositionName, pGuild->GetPosName(pAppointInfo->m_NewPosition), GuildReturn.m_PositionNameSize);
					
					GuildReturn.m_GuildNameSize = (UCHAR)strlen(pGuild->GetGuildName());
					strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), GuildReturn.m_GuildNameSize);
					
					Msg.SetGuildReturn( &GuildReturn );

					//通知自己
					Msg.SetPlayerID( pUser->GetPlayerID() );
					pServerPlayer->SendPacket( &Msg );

					//通知被任命的人
					USER* pNewUser = g_pOnlineUser->FindUser( pAppointInfo->m_CandidateGUID );
					if(pNewUser)
					{
						ServerPlayer* pTmpServerPlayer = NULL;
						pTmpServerPlayer = g_pServerManager->GetServerPlayer(pNewUser->GetServerID());
						if( pTmpServerPlayer != NULL )
						{
							Msg.SetPlayerID( pNewUser->GetPlayerID() );
							pTmpServerPlayer->SendPacket( &Msg );
						}
					}
				}
				
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X recruited a guild!", 
					guid );
				return PACKET_EXE_CONTINUE;
			}
			else if(bSuccess == GUILD_ADMIN_t::RET_NOT_EXIST)
			{
//				GuildErr = GUILD_ERROR_POS_FULL;
			}
			else if(bSuccess == GUILD_ADMIN_t::RET_POS_FULL)
			{
				GuildErr = GUILD_ERROR_POS_FULL;
			}
			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X join a guild failed!", 
				guid );
		}
		break;
	case GUILD_PACKET_GW_ADJUSTAUTHORITY:
		{
		}
		break;
	case GUILD_PACKET_GW_RECRUIT:
		{
			if( guildID == INVALID_ID )
			{ // 已有帮会，这里应该已经由 Server 正确处理，所以不返回错误结果
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to recruit a user!", 
					guid );
				break;
			}

			Guild* pGuild = g_pGuildManager->GetGuild( guildID );
			if( pGuild == NULL )
			{
				Assert(FALSE);
				GuildErr = GUILD_ERROR_NOT_EXIST;
			}
			else
			{
				switch( pGuild->GetGuildStatus() )
				{
				case GUILD_STATUS_NORMAL:
					{
						if( pGuild->IsAuthorized(pUser->GetGUID(), GUILD_AUTHORITY_RECRUIT) == FALSE )
						{
							GuildErr = GUILD_ERROR_UNAUTHORIZED;
						}
						else
						{
							GUILD_CGW_RECRUIT* pGuildRecruit;
							pGuildRecruit = (GUILD_CGW_RECRUIT*)pGuildPacket->GetPacket(GUILD_PACKET_GW_RECRUIT);
							Assert( pGuildRecruit );

							const GUILDUSER* pGuildUser = pGuild->GetGuildUser( pGuildRecruit->m_ProposerGUID );
							if( pGuildUser == NULL )
							{
								Assert(FALSE);
								GuildErr = GUILD_ERROR;
							}
							else if( pGuildUser->m_Position != GUILD_POSITION_TRAINEE )
							{ // 对一个正式帮众进行此操作，也许另一个人恰好进行了招人操作
								GuildErr = GUILD_ERROR_ALREADY_MEMBER;
							}
							else
							{
								//全世界所有帮会中的申请列表遍历一边，呵呵，够狠
								for( INT i=0; i<MAX_GUILD_SIZE; i++ )
								{
									Guild* pDeleteGuild = g_pGuildManager->GetGuild( i );
									if(pDeleteGuild->GetGuildID() != INVALID_ID && pDeleteGuild->GetGuildID() != pGuild->GetGuildID())
									{
										for(INT j = 0; j<USER_ARRAY_SIZE;j++)
										{
											GUILDUSER* pDeleteUser = pDeleteGuild->GetGuildUserByIndex(j);
											if(pDeleteUser->m_UserGUID != INVALID_ID)
											{
												if(pDeleteUser->m_UserGUID == pGuildRecruit->m_ProposerGUID
													&&pDeleteUser->m_Position == GUILD_POSITION_TRAINEE)
												{//删掉此人
													pDeleteGuild->OnUserLeaveByIndex(j);
													break;
												}
											}
										}
									}
								}

								GuildErr = pGuild->OnRecruitUser( pGuildUser->m_UserGUID );
								if(GuildErr == GUILD_ERROR_NOTHING)
								{
									USER* pRecruitUser = g_pOnlineUser->FindUser( pGuildUser->m_szUserName );
									if( !pRecruitUser )
									{
										//发一封可执行邮件
										//在下次次玩家上线时会由Server执行此邮件对应的脚本，这里只是更改此人的工会ID
										g_pMailCenter->SendScriptMail( pGuildUser->m_szUserName,
											MAIL_UPDATE_ATTR, MAIL_ATTR_GUILD, guildID);

										//欢迎你加入BB，大家同舟共济相携行走江湖。
										CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
										sprintf(szMailContent, "欢迎你加入%s，大家同舟共济相携行走江湖。", pGuild->GetGuildName());
										g_pMailCenter->SendNormalMail(pGuildUser->m_szUserName, szMailContent);
									}

									WGGuildReturn Msg;
									Msg.SetGUID(pGuildUser->m_UserGUID);
									_GUILD_RETURN GuildReturn;
									GuildReturn.m_ReturnType = GUILD_RETURN_RECRUIT;
									GuildReturn.m_GUID = pGuildUser->m_UserGUID;
									GuildReturn.m_GUIDChanged = guid;
									GuildReturn.m_GuildID	=	guildID;
									
									GuildReturn.m_SourNameSize = (UCHAR)strlen(pUser->GetName());
									strncpy((CHAR*)GuildReturn.m_SourName, pUser->GetName(), GuildReturn.m_SourNameSize);
									
									GuildReturn.m_DestNameSize = (UCHAR)strlen(pGuildUser->m_szUserName);
									strncpy((CHAR*)GuildReturn.m_DestName, pGuildUser->m_szUserName, GuildReturn.m_DestNameSize);
									
									GuildReturn.m_GuildNameSize = (UCHAR)strlen(pGuild->GetGuildName());
									strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), GuildReturn.m_GuildNameSize);
								
									Msg.SetGuildReturn( &GuildReturn );
									pGuild->BroadCast_Return( &Msg );
									Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X recruited a guild!", 
										guid );
									return PACKET_EXE_CONTINUE;
								}
							}
						}
					}
					break;
				default:
					Assert(FALSE);
				}
			}

			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X Recruit a guild failed!", 
				guid );
		}
		break;
	case GUILD_PACKET_GW_EXPEL:
		{
			if( guildID == INVALID_ID )
			{ // 已有帮会，这里应该已经由 Server 正确处理，所以不返回错误结果
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to expel a user!", 
					guid );
				break;
			}

			Guild* pGuild = g_pGuildManager->GetGuild( guildID );
			if( pGuild == NULL )
			{
				Assert(FALSE);
				GuildErr = GUILD_ERROR_NOT_EXIST;
			}
			else
			{
				switch( pGuild->GetGuildStatus() )
				{
				case GUILD_STATUS_NORMAL:
					{
						if( pGuild->IsAuthorized(pUser->GetGUID(), GUILD_AUTHORITY_EXPEL) == FALSE )
						{
							GuildErr = GUILD_ERROR_UNAUTHORIZED;
						}
						else
						{
							GUILD_CGW_EXPEL* pGuildExpel;
							pGuildExpel = (GUILD_CGW_EXPEL*)pGuildPacket->GetPacket(GUILD_PACKET_GW_EXPEL);
							Assert( pGuildExpel );

							const GUILDUSER* pGuildUser = pGuild->GetGuildUser( pGuildExpel->m_GuildUserGUID );
							if( pGuildUser == NULL )
							{
								Assert(FALSE);
								GuildErr = GUILD_ERROR;
							}
							else if( pGuildUser->m_Position
								>= pGuild->GetGuildUser(pUser->GetGUID())->m_Position )
							{ //只能对比自己级别低的人进行操作，以后这个条件判断可能要进行细化
								//只能清除普通帮众
								GuildErr = GUILD_ERROR_UNAUTHORIZED;
							}
							else
							{
								CHAR szTempName[MAX_CHARACTER_NAME];
								strncpy((CHAR*)szTempName, pGuildUser->m_szUserName, sizeof(szTempName) - 1);
								BOOL bIsPropose = (pGuildUser->m_Position == GUILD_POSITION_TRAINEE)?TRUE:FALSE;
								GuildErr = pGuild->OnUserLeave( pGuildUser->m_UserGUID );

								if(GuildErr == GUILD_ERROR_NOTHING)
								{
									WGGuildReturn Msg;
									Msg.SetGUID(pGuildUser->m_UserGUID);

									_GUILD_RETURN GuildReturn;

									if(bIsPropose)
									{
										GuildReturn.m_ReturnType = GUILD_RETURN_REJECT;
										GuildReturn.m_GUID = pGuildExpel->m_GuildUserGUID;
										
										GuildReturn.m_GuildNameSize = (UCHAR)strlen(pGuild->GetGuildName());
										strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), GuildReturn.m_GuildNameSize);
										
										GuildReturn.m_DestNameSize = (UCHAR)strlen(szTempName);
										strncpy((CHAR*)GuildReturn.m_DestName, szTempName, GuildReturn.m_DestNameSize);
										
										Msg.SetGuildReturn( &GuildReturn );
										Msg.SetPlayerID( pUser->GetPlayerID() );
										pServerPlayer->SendPacket( &Msg );
									}
									else
									{
										GuildReturn.m_ReturnType = GUILD_RETURN_EXPEL;
										GuildReturn.m_GUID = pGuildExpel->m_GuildUserGUID;
										
										GuildReturn.m_SourNameSize = (UCHAR)strlen(pUser->GetName());
										strncpy((CHAR*)GuildReturn.m_SourName, pUser->GetName(), GuildReturn.m_SourNameSize);
										
										GuildReturn.m_DestNameSize = (UCHAR)strlen(szTempName);
										strncpy((CHAR*)GuildReturn.m_DestName, szTempName, GuildReturn.m_DestNameSize);
										
										Msg.SetGuildReturn( &GuildReturn );
										pGuild->BroadCast_Return( &Msg );
									}

									USER* pDestUser = g_pOnlineUser->FindUser( pGuildExpel->m_GuildUserGUID );
									if( pDestUser != NULL )
									{
										pDestUser->CleanUpGuildData();
										pServerPlayer = g_pServerManager->GetServerPlayer(pDestUser->GetServerID());

										if( pServerPlayer != NULL )
										{
											Msg.SetPlayerID( pDestUser->GetPlayerID() );
											pServerPlayer->SendPacket( &Msg );
										}
									}
									else
									{
										//发一封可执行邮件
										//在下次次玩家上线时会由Server执行此邮件对应的脚本，这里只是更改此人的工会ID
										g_pMailCenter->SendScriptMail( szTempName,
											MAIL_UPDATE_ATTR, MAIL_ATTR_GUILD, INVALID_ID);

										if(bIsPropose)
										{
											//BB帮会拒绝了你的申请。
											CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
											sprintf(szMailContent, "%s帮会拒绝了你的申请。", pGuild->GetGuildName());
											g_pMailCenter->SendNormalMail(szTempName, szMailContent);
										}
										else
										{
											//你与BB志不同道不合，你已经离开了BB了。
											CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
											sprintf(szMailContent, "你与%s志不同道不合，你已经离开了%s了。", pGuild->GetGuildName(), pGuild->GetGuildName());
											g_pMailCenter->SendNormalMail(szTempName, szMailContent);

										}
									}

									Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X expelled a guild!", 
										guid );
									return PACKET_EXE_CONTINUE;
								}
							}
						}
					}
					break;
				default:
					Assert(FALSE);
				}
			}
			
			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X Recruit a guild failed!", 
				guid );
		}
		break;
	case GUILD_PACKET_GW_WITHDRAW:
		{
		}
		break;
	case GUILD_PACKET_GW_DEPOSIT:
		{
		}
		break;
	case GUILD_PACKET_GW_LEAVE:
		{
			if( guildID == INVALID_ID )
			{ // 已有帮会，这里应该已经由 Server 正确处理，所以不返回错误结果
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to leave from nil guild!", 
					guid );
				break;
			}

			Guild* pGuild = g_pGuildManager->GetGuild( guildID );
			if( pGuild == NULL )
			{
				Assert(FALSE);
				GuildErr = GUILD_ERROR_NOT_EXIST;
				break;
			}
			else if( pGuild->IsAuthorized(pUser->GetGUID(), GUILD_AUTHORITY_LEAVE) == FALSE )
			{
				GuildErr = GUILD_ERROR_UNAUTHORIZED;
				break;
			}

			GuildErr = pGuild->OnUserLeave(pUser->GetGUID());

			if(GuildErr == GUILD_ERROR_NOTHING)
			{
				pUser->CleanUpGuildData();
				WGGuildReturn Msg;
				Msg.SetPlayerID( pUser->GetPlayerID() );
				Msg.SetGUID(guid);
				_GUILD_RETURN GuildReturn;
				GuildReturn.m_ReturnType = GUILD_RETURN_LEAVE;
				GuildReturn.m_GUID = pUser->GetGUID();
				
				GuildReturn.m_GuildNameSize = (UCHAR)strlen(pGuild->GetGuildName());
				strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), GuildReturn.m_GuildNameSize);
				
				GuildReturn.m_DestNameSize = (UCHAR)strlen(pUser->GetName());
				strncpy((CHAR*)GuildReturn.m_DestName, pUser->GetName(), GuildReturn.m_DestNameSize);
				
				Msg.SetGuildReturn( &GuildReturn );
				pServerPlayer->SendPacket( &Msg );
				pGuild->BroadCast_Return( &Msg );
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X left a guild!", 
					guid );
				return PACKET_EXE_CONTINUE;
			}

			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X Leave a guild failed!", 
				guid );
		}
		break;
	case GUILD_PACKET_GW_DISMISS:
		{
			if( guildID == INVALID_ID )
			{ // 已有帮会，这里应该已经由 Server 正确处理，所以不返回错误结果
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to leave from nil guild!", 
					guid );
				break;
			}

			Guild* pGuild = g_pGuildManager->GetGuild( guildID );
			if( pGuild == NULL )
			{
				Assert(FALSE);
				GuildErr = GUILD_ERROR_NOT_EXIST;
				break;
			}
			else if( pUser->GetGUID() != pGuild->GetChieftainGUID() )
			{
				GuildErr = GUILD_ERROR_UNAUTHORIZED;
				break;
			}

			GuildErr = g_pGuildManager->DestroyGuild( pUser->GetGUID(), guildID );
			if( GuildErr == GUILD_ERROR_NOTHING )
			{
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X left a guild!", 
					guid );
				return PACKET_EXE_CONTINUE;
			}
		}
		break;
	case GUILD_PACKET_GW_DEMISE:
		{
			if( guildID == INVALID_ID )
			{ // 已有帮会，这里应该已经由 Server 正确处理，所以不返回错误结果
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to leave from nil guild!", 
					guid );
				break;
			}

			Guild* pGuild = g_pGuildManager->GetGuild( guildID );
			if( pGuild == NULL )
			{
				Assert(FALSE);
				GuildErr = GUILD_ERROR_NOT_EXIST;
				break;
			}
			else if( pUser->GetGUID() != pGuild->GetChieftainGUID() )
			{
				GuildErr = GUILD_ERROR_UNAUTHORIZED;
				break;
			}

			GuildErr = pGuild->OnDemise();
			if(GuildErr == GUILD_ERROR_NOTHING)
			{
				USER* pNewChief = g_pOnlineUser->FindUser( pGuild->GetChieftainGUID( ) );
				if(pNewChief == NULL)
				{
					//你的权限改变了，恭喜你成为帮会帮主。
					CHAR szMailContent[MAX_MAIL_CONTEX] = {0};
					sprintf(szMailContent, "你的权限改变了，恭喜你成为帮会帮主。");
					g_pMailCenter->SendNormalMail(pGuild->GetGuildChiefName(), szMailContent);
				}

				WGGuildReturn Msg;
				Msg.SetPlayerID( pUser->GetPlayerID() );
				Msg.SetGUID(guid);
				_GUILD_RETURN GuildReturn;
				GuildReturn.m_ReturnType	=	GUILD_RETURN_DEMISE;
				GuildReturn.m_GuildID		=	guildID;
				GuildReturn.m_GUID			=	pUser->GetGUID();
				GuildReturn.m_GUIDChanged	=	pGuild->GetChieftainGUID();

				GuildReturn.m_SourNameSize	=	(UCHAR)strlen(pUser->GetName());
				strncpy((CHAR*)GuildReturn.m_SourName, pUser->GetName(), GuildReturn.m_SourNameSize);

				GuildReturn.m_DestNameSize	=	(UCHAR)strlen(pGuild->GetGuildChiefName());
				strncpy((CHAR*)GuildReturn.m_DestName, pGuild->GetGuildChiefName(), GuildReturn.m_DestNameSize);

				GuildReturn.m_GuildNameSize	=	(UCHAR)strlen(pGuild->GetGuildName());
				strncpy((CHAR*)GuildReturn.m_GuildName, pGuild->GetGuildName(), GuildReturn.m_GuildNameSize);

				GuildReturn.m_ChangedPositionNameSize	=	(UCHAR)strlen(pGuild->GetPosName(GUILD_POSITION_CHIEFTAIN));
				strncpy((CHAR*)GuildReturn.m_ChangedPositionName, pGuild->GetPosName(GUILD_POSITION_CHIEFTAIN), GuildReturn.m_ChangedPositionNameSize);

				GuildReturn.m_PositionNameSize	=	(UCHAR)strlen(pGuild->GetPosName(GUILD_POSITION_ASS_CHIEFTAIN));
				strncpy((CHAR*)GuildReturn.m_PositionName, pGuild->GetPosName(GUILD_POSITION_ASS_CHIEFTAIN), GuildReturn.m_PositionNameSize);


				Msg.SetGuildReturn( &GuildReturn );
				pServerPlayer->SendPacket( &Msg );
				pGuild->BroadCast_Return( &Msg );
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X left a guild!", 
					guid );
				return PACKET_EXE_CONTINUE;
			}
		}
		break;
	case GUILD_PACKET_GW_CHANGEDESC:
		{
			if( guildID != INVALID_ID )
			{ // 已有帮会，这里应该已经由 Server 正确处理，所以不返回错误结果
				Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X attempt to create an extra guild!", 
					guid );
				GuildErr = GUILD_ERROR_GUILD_ALREADY_EXIST;
				break;
			}

			GUILD_CGW_CHANG_DESC* pGuildChangDesc = (GUILD_CGW_CHANG_DESC*)pGuildPacket->GetPacket(GUILD_PACKET_GW_CHANGEDESC);
			Assert( pGuildChangDesc );

			WGGuildReturn Msg;
			Msg.SetPlayerID( pUser->GetPlayerID() );
			Msg.SetGUID(guid);
			Guild* pGuild = g_pGuildManager->GetGuild(guildID);
			pGuild->SetGuildDesc(pGuildChangDesc->m_GuildDesc);
			_GUILD_RETURN GuildReturn;
			GuildReturn.m_GuildID = pUser->GetGuildID();
			GuildReturn.m_ReturnType = GUILD_RETURN_CHANGEDESC;
			Msg.SetGuildReturn( &GuildReturn );
			pServerPlayer->SendPacket( &Msg );
			Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X created a guild success!", 
				guid );
			return PACKET_EXE_CONTINUE;

		}
		break;

	default:
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	WGGuildError Msg;
	Msg.SetPlayerID( pUser->GetPlayerID() );
	Msg.SetErrorCode( GuildErr );
	Msg.SetGUID(guid);
	pServerPlayer->SendPacket( &Msg );
	Log::SaveLog( WORLD_LOGFILE, "GWGuildHandler...User GUID=%X do a guild action failed!", 
		guid );
	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
