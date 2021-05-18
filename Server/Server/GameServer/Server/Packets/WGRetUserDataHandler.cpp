#include "stdafx.h"


#include "WGRetUserData.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "IncomingPlayerManager.h"
#include "GCConnect.h"
#include "GUIDManager.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "HumanDB.h"
#include "GWNotifyUser.h"
#include "PacketFactoryManager.h"

uint WGRetUserDataHandler::Execute( WGRetUserData* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID ;
	GamePlayer* pGamePlayer = NULL ;

	Assert( pPlayer ) ;
	
	if( pPlayer->IsServerPlayer() )
	{
		PlayerID = pPacket->GetPlayerID() ;
		pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID) ;
		if( pGamePlayer==NULL )
		{
			Assert(FALSE) ;
			return PACKET_EXE_CONTINUE ;
		}
	}
	else if( pPlayer->IsGamePlayer() )
	{
		PlayerID = pPlayer->PlayerID() ;
		pGamePlayer = (GamePlayer*)pPlayer ;
	}
	else
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	if( pGamePlayer->GetPlayerStatus()==PS_SERVER_WAITING_FOR_SERVER_DATA )
	{//在ServerManager模块中执行部分
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID ) ;

		pGamePlayer->SetPlayerStatus( PS_SERVER_WAITING_FOR_SERVER_DATA_INCOMING ) ;

		g_pIncomingPlayerManager->SendPacket( pPacket, PlayerID ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGRetUserDataHandler::Execute(ret=%d) GUID=%X ServerPlayer...OK",
			pPacket->GetReturn(), pGamePlayer->m_HumanGUID ) ;

		return PACKET_EXE_NOTREMOVE ;
	}
	else if( pGamePlayer->GetPlayerStatus()==PS_SERVER_WAITING_FOR_SERVER_DATA_INCOMING )
	{//在IncomingPlayerManager模块中执行的
		Assert( MyGetCurrentThreadID()==g_pIncomingPlayerManager->m_ThreadID ) ;
	
		switch( pPacket->GetReturn() )
		{
		case UDR_NOTFINDUSERDATA:
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:WGRetUserDataHandler::Execute UDR_NOTFINDUSERDATA(PID=%d,GUID=%X)",
					pPacket->GetPlayerID(),pGamePlayer->m_HumanGUID ) ;

				return PACKET_EXE_ERROR ;
			}
			break ;
		case UDR_USERDATA:
			{
				GCConnect Msg ;
				Msg.SetServerID( g_pServerManager->GetServerID() ) ;
				//测试数据
				SceneID_t sceneid = pPacket->GetUserData()->m_Human.m_StartScene ;
				WORLD_POS enterpos = pPacket->GetUserData()->m_Human.m_Position ;
				Msg.SetSceneID( sceneid ) ;
				Msg.SetWorldPos( &enterpos ) ;
				Msg.SetEstate( 0 );
				//测试数据

				pGamePlayer->InitHuman( pPacket->GetUserData(),UDR_USERDATA, pPacket->GetPlayerAge() ) ;

				pGamePlayer->SetPlayerStatus( PS_SERVER_WAITING_FOR_ENTER ) ;

				pGamePlayer->SendPacket( &Msg ) ;

				g_pLog->FastSaveLog( LOG_FILE_1, "WGRetUserDataHandler::Execute(ret=%d) GUID=%X SceneID=%d Pos=(%.2f,%.2f)...OK ",
					pPacket->GetReturn(), pGamePlayer->m_HumanGUID, sceneid, enterpos.m_fX, enterpos.m_fZ ) ;
			
				return PACKET_EXE_CONTINUE ;
			}
			break ;
		
		case UDR_USERDATALIVING:
			{
				//取得已经用户此GUID数据的Obj_Human信息
				Obj_Human* pOldHuman = (Obj_Human*)(g_pGUIDManager->Get(pGamePlayer->m_HumanGUID)) ;
				if( pOldHuman==NULL )
				{
					GWNotifyUser* pMsg = (GWNotifyUser*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_NOTIFYUSER)) ;
					pMsg->SetGUID( pGamePlayer->m_HumanGUID ) ;
					pMsg->SetPlayerID( -1 ) ;
					pMsg->SetStatus( GWNotifyUser::NUS_NEED_WORLD_KICK ) ;
					g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:WGRetUserDataHandler::Execute Cannot find HumanData (GUID=%X)",
						pGamePlayer->m_HumanGUID ) ;

					return PACKET_EXE_ERROR ;
				}
				//取得拥有此GUID的Obj_Human所在的场景指针
				Scene* pScene = pOldHuman->getScene() ;
				if( pScene==NULL )
				{
					GamePlayer* pOldPlayer = (GamePlayer*)(pOldHuman->GetPlayer()) ;
					if( pOldPlayer && pOldPlayer->GetPlayerStatus()==PS_SERVER_ENTER_RECYCLE )
					{
						g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:WGRetUserDataHandler::Execute PS_SERVER_ENTER_RECYCLE (GUID=%X)",
							pGamePlayer->m_HumanGUID ) ;
					}
					else
					{
						GWNotifyUser* pMsg = (GWNotifyUser*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_NOTIFYUSER)) ;
						pMsg->SetGUID( pGamePlayer->m_HumanGUID ) ;
						pMsg->SetPlayerID( -1 ) ;
						pMsg->SetStatus( GWNotifyUser::NUS_NEED_WORLD_KICK ) ;
						g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

						g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:WGRetUserDataHandler::Execute Scene==NULL (GUID=%X)",
							pGamePlayer->m_HumanGUID ) ;
					}

					return PACKET_EXE_ERROR ;
				}

				INT nStatus = ((GamePlayer*)(pOldHuman->GetPlayer()))->GetPlayerStatus() ;
				if( nStatus != PS_SERVER_NORMAL )
				{
					GWNotifyUser* pMsg = (GWNotifyUser*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_NOTIFYUSER)) ;
					pMsg->SetGUID( pGamePlayer->m_HumanGUID ) ;
					pMsg->SetPlayerID( -1 ) ;
					pMsg->SetStatus( GWNotifyUser::NUS_NEED_WORLD_KICK ) ;
					g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:WGRetUserDataHandler::Execute GetPlayerStatus() != PS_SERVER_NORMAL (GUID=%X Status=%d)",
						pGamePlayer->m_HumanGUID, nStatus ) ;

					((GamePlayer*)(pOldHuman->GetPlayer()))->SetPlayerStatus(PS_SERVER_ANOTHER_GUID_ALSO_KICK) ;

					//向此场景发送当前消息
					pScene->SendPacket( pPacket, pOldHuman->GetPlayerID() ) ;

					return PACKET_EXE_NOTREMOVE_ERROR ;
				}

				((GamePlayer*)(pOldHuman->GetPlayer()))->SetPlayerStatus(PS_SERVER_ANOTHER_GUID_ENTER) ;
			
				//向此场景发送当前消息
				pScene->SendPacket( pPacket, pOldHuman->GetPlayerID() ) ;

				g_pLog->FastSaveLog( LOG_FILE_1, "WGRetUserDataHandler::Execute(ret=%d) UDR_USERDATALIVING GUID=%X SceneID=%d ...OK ",
					pPacket->GetReturn(), pGamePlayer->m_HumanGUID, pScene->SceneID() ) ;

				return PACKET_EXE_NOTREMOVE ;
			}
			break ;
		case UDR_USERSERVERCRASH: //取消对Server Crash 情况处理
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:WGRetUserDataHandler::Execute UDR_USERSERVERCRASH(PID=%d, GUID=%X)",
					pPacket->GetPlayerID(), pGamePlayer->m_HumanGUID ) ;

				return PACKET_EXE_ERROR ;
			}
			break;
		case UDR_KEYERROR:
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:WGRetUserDataHandler::Execute UDR_KEYERROR(PID=%d, GUID=%X)",
					pPacket->GetPlayerID(), pGamePlayer->m_HumanGUID ) ;

				return PACKET_EXE_ERROR ;
			}
			break ;
		default :
			{
				Assert(FALSE) ;
			}
			break ;
		}
	}
	else if( pGamePlayer->GetPlayerStatus()==PS_SERVER_ANOTHER_GUID_ENTER )
	{//旧的玩家所在的场景来执行 Scene
		Obj_Human* pHuman = pGamePlayer->GetHuman() ;
		Assert( pHuman ) ;

		Scene* pScene = pHuman->getScene() ;
		if( pScene==NULL )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR WGRetUserDataHandler::Scene==NULL(ret=%d) GUID=%X",
				pPacket->GetReturn(), pGamePlayer->m_HumanGUID) ;
			return PACKET_EXE_CONTINUE ;
		}

		//检测执行线程的数据是否正确
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
		
		GamePlayer* pNewPlayer = g_pPlayerPool->GetPlayer( pPacket->GetPlayerID() ) ;
		Assert( pNewPlayer ) ;

		Obj_Human* pNewHuman = pNewPlayer->GetHuman() ;
		Assert( pNewHuman ) ;
		{//重设消息数据
			pPacket->SetReturn( UDR_USERDATA ) ;
			FULLUSERDATA* pData = pPacket->GetUserData() ;

			memcpy( &(pData->m_Human), pHuman->GetDB()->GetHumanDB(), sizeof(_HUMAN_DB_LOAD) ) ;
			memcpy( &(pData->m_Equip), pHuman->GetDB()->GetEquipDB(), sizeof(_EQUIP_DB_LOAD) ) ;
			memcpy( &(pData->m_Bag), pHuman->GetDB()->GetBag(), sizeof(_BAG_DB_LOAD) ) ;
			memcpy( &(pData->m_Bank), pHuman->GetDB()->GetBankDB(), sizeof(_BANK_DB_LOAD) ) ;
			memcpy( &(pData->m_Skill), pHuman->GetDB()->GetSkillDB(), sizeof(_SKILL_DB_LOAD) ) ;
			memcpy( &(pData->m_Cooldown), pHuman->GetDB()->GetCooldownDB(), sizeof(_COOLDOWN_DB_LOAD_FOR_HUMAN) ) ;
			memcpy( &(pData->m_XinFa), pHuman->GetDB()->GetXinFaDB(), sizeof(_XINFA_DB_LOAD) ) ;
			memcpy( &(pData->m_Impact), pHuman->GetDB()->GetImpactDB(), sizeof(_IMPACT_DB_LOAD) ) ;
			memcpy( &(pData->m_Ability), pHuman->GetDB()->GetAbilityDB(), sizeof(_ABILITY_DB_LOAD) ) ;
			memcpy( &(pData->m_Mission), pHuman->GetDB()->GetMissionDB(), sizeof(_MISSION_DB_LOAD) ) ;
			memcpy( &(pData->m_Setting), pHuman->GetDB()->GetSettingDB(), sizeof(_SETTING_DB_LOAD) ) ;
			memcpy( &(pData->m_PetList), pHuman->GetDB()->m_dbPetList, sizeof(_PET_DB_LIST_LOAD) ) ;
			memcpy( &(pData->m_Relation), pHuman->GetDB()->GetRelationDB(), sizeof(_RELATION_DB_LOAD) ) ;
			memcpy( &(pData->m_PrivateInfo), pHuman->GetDB()->GetPrivateInfoDB(), sizeof(_PRIVATE_INFO_DB_LOAD) ) ;
			// pData->m_bIsPasswdUnlock = pHuman->__IsPasswordUnlock(); // 此时不予设置
		}

		pGamePlayer->SetDirty( TRUE ) ;

		g_pIncomingPlayerManager->SendPacket( pPacket, pNewPlayer->PlayerID() ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGRetUserDataHandler::Execute(ret=%d) GUID=%X SceneID=%d IncomingPlayerManager...OK ",
			pPacket->GetReturn(), pGamePlayer->m_HumanGUID, pScene->SceneID() ) ;

		return PACKET_EXE_NOTREMOVE_ERROR ;
	}
	else if( pGamePlayer->GetPlayerStatus()==PS_SERVER_ANOTHER_GUID_ALSO_KICK )
	{
		Obj_Human* pHuman = pGamePlayer->GetHuman() ;
		Assert( pHuman ) ;

		Scene* pScene = pHuman->getScene() ;
		if( pScene==NULL )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR WGRetUserDataHandler::Scene==NULL(statuc=%d) GUID=%X",
				PS_SERVER_ANOTHER_GUID_ALSO_KICK, pGamePlayer->m_HumanGUID) ;
			return PACKET_EXE_CONTINUE ;
		}

		//检测执行线程的数据是否正确
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGRetUserDataHandler::Execute(status=%d) GUID=%X SceneID=%d Kickout OldHuman...OK ",
			PS_SERVER_ANOTHER_GUID_ALSO_KICK, pGamePlayer->m_HumanGUID, pScene->SceneID() ) ;

		return PACKET_EXE_ERROR ;
	}

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

