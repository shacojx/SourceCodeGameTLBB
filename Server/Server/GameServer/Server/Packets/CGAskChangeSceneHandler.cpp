#include "stdafx.h"

#include "CGAskChangeScene.h"
#include "GamePlayer.h"
#include "Log.h"
#include "ServerManager.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GWAskChangeScene.h"
#include "SceneManager.h"
#include "PacketFactoryManager.h"
#include "Obj_Pet.h"
#include "PetManager.h"



uint CGAskChangeSceneHandler::Execute( CGAskChangeScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	if (pGamePlayer->m_HumanGUID == INVALID_ID)
	{
		return PACKET_EXE_ERROR;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Scene* pScene = pHuman->getScene();
	if( !pScene )
	{
		return PACKET_EXE_ERROR;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

 
	SceneID_t SourSceneID = pPacket->GetSourSceneID( ) ;
	SceneID_t DestSceneID = pPacket->GetDestSceneID( ) ;

	ID_t SourServerID = g_pSceneManager->GetSceneInfo(SourSceneID)->m_ServerID ;
	ID_t DestServerID = g_pSceneManager->GetSceneInfo(DestSceneID)->m_ServerID ;


	GWAskChangeScene* pMsg = (GWAskChangeScene*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_ASKCHANGESCENE)) ;
	pMsg->SetPlayerID( pGamePlayer->PlayerID() ) ;
	pMsg->SetGUID( pGamePlayer->m_HumanGUID ) ;
	pMsg->SetSourSceneID( SourSceneID ) ;
	pMsg->SetDestSceneID( DestSceneID ) ;
	pMsg->SetKey((UINT)rand()) ;
	if( SourServerID == DestServerID )
	{//当前场景和目的场景在同一个游戏服务器程序内
		pMsg->SetStatus( GWAskChangeScene::CSS_SAMESCENE ) ;
	}
	else
	{//当前场景和目的场景位于不同地游戏服务器程序内
		pMsg->SetStatus( GWAskChangeScene::CSS_DIFFSERVER ) ;
		
		FULLUSERDATA* pData = pMsg->GetUserData() ;
		

		memcpy( &(pData->m_Human), pHuman->GetDB()->GetHumanDB(), sizeof(_HUMAN_DB_LOAD) ) ;
		memcpy( &(pData->m_Bank), pHuman->GetDB()->GetBankDB(), sizeof(_BANK_DB_LOAD) ) ;
		memcpy( &(pData->m_Bag), pHuman->GetDB()->GetBag(), sizeof(_BAG_DB_LOAD) ) ;
		memcpy( &(pData->m_Equip), pHuman->GetDB()->GetEquipDB(), sizeof(_EQUIP_DB_LOAD) ) ;
		memcpy( &(pData->m_Skill), pHuman->GetDB()->GetSkillDB(), sizeof(_SKILL_DB_LOAD) ) ;
		memcpy( &(pData->m_Cooldown), pHuman->GetDB()->GetSkillDB(), sizeof(_COOLDOWN_DB_LOAD_FOR_HUMAN) ) ;
		memcpy( &(pData->m_XinFa), pHuman->GetDB()->GetXinFaDB(), sizeof(_XINFA_DB_LOAD) ) ;
		memcpy( &(pData->m_Impact), pHuman->GetDB()->GetImpactDB(), sizeof(_IMPACT_DB_LOAD) ) ;
		memcpy( &(pData->m_Ability), pHuman->GetDB()->GetAbilityDB(), sizeof(_ABILITY_DB_LOAD) ) ;
		memcpy( &(pData->m_Mission), pHuman->GetDB()->GetMissionDB(), sizeof(_MISSION_DB_LOAD) ) ;
		memcpy( &(pData->m_Setting), pHuman->GetDB()->GetSettingDB(), sizeof(_SETTING_DB_LOAD) ) ;
		memcpy( &(pData->m_PetList), pHuman->GetDB()->m_dbPetList, sizeof(_PET_DB_LIST_LOAD) ) ;
		memcpy( &(pData->m_Relation), pHuman->GetDB()->GetRelationDB(), sizeof(_RELATION_DB_LOAD) ) ;
		memcpy( &(pData->m_PrivateInfo), pHuman->GetDB()->GetPrivateInfoDB(), sizeof(_PRIVATE_INFO_DB_LOAD) ) ;

		//切换场景humanDB版本号+1
		pData->m_Human.m_DBVersion++;
		pData->m_bIsPasswdUnlock = pHuman->__IsPasswordUnlock();
	}

	g_pServerManager->SendPacket( pMsg, INVALID_ID ) ;

	//将玩家从地图中删除，防止别的玩家向其发起某些动作或发送消息
	//在此操作后，玩家的角色不会继续接收到广播消息和做逻辑操作
	//ZoneID_t zid = pHuman->getZoneID() ;
	//pScene->Unregister( pHuman, zid ) ;
	//pHuman->setZoneID( INVALID_ID ) ;
	pHuman->SetActiveFlag( FALSE );
	pGamePlayer->SetPlayerStatus( PS_SERVER_WAITING_FOR_CHANGESCENE ) ;

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskChangeSceneHandler: GUID=%X S=%d D=%d PID=%d",
		pGamePlayer->m_HumanGUID, SourSceneID, DestSceneID, pGamePlayer->PlayerID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
