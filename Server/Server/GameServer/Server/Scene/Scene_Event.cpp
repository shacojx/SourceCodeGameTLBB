#include "stdafx.h"

#include "Scene.h"
#include "GamePlayer.h"
#include "SceneManager.h"
#include "PacketFactoryManager.h"
#include "GGSceneNotify.h"
#include "GUIDManager.h"
#include "Obj_Human.h"
#include "GamePlayer.h"
#include "GCNotifyChangeScene.h"
#include "TimeManager.h"
#include "GCExchangeError.h"
#include "ScenePlayerManager.h"
#include "MonsterManager.h"
#include "PetManager.h"
#include "GCCityNotify.h"
#include "ItemBoxManager.h"


#define SCENE_SCRIPTID 888888

VOID Scene::OnSceneInit( )
{
__ENTER_FUNCTION

	UINT uTime = g_pTimeManager->CurrentTime() ;

	if( m_CopyData.m_SourSceneID != INVALID_ID )
	{
		Obj_Human* pHuman = (Obj_Human*)(g_pGUIDManager->Get(m_CopyData.m_TeamLeader)) ;
		if( pHuman )
		{//根据副本创建者的GUID，设置此副本的队伍号信息
			m_CopyData.m_TeamID = pHuman->GetTeamInfo()->GetTeamID() ;
		}
		else
		{
			Assert(FALSE) ;
		}

		Scene* pScene = g_pSceneManager->GetScene(m_CopyData.m_SourSceneID) ;
		if( pScene )
		{
			GGSceneNotify* pMsg = (GGSceneNotify*)(g_pPacketFactoryManager->CreatePacket(PACKET_GG_SCENENOTIFY));
			pMsg->SetSourSceneID( m_CopyData.m_SourSceneID ) ;
			pMsg->SetDestSceneID( SceneID() ) ;

			pScene->SendPacket( pMsg, INVALID_ID_EX ) ;
		}
	}

	if( m_CopyData.m_SceneTimer > 0 )
	{
		BeginSceneTimer( m_CopyData.m_SceneTimer, uTime ) ;
	}

	if(m_SceneInitData.m_nDataType == SCENE_TYPE_CIT)
	{//此场景是城市场景，特殊处理
		//设置入口场景的连接
		GCCityNotify MsgNotifySuccess;
		Obj_Human* pHuman = (Obj_Human*)(g_pGUIDManager->Get(m_CityData.m_Creator)) ;
		pHuman->GetPlayer()->SendPacket(&MsgNotifySuccess);

		Scene* pScene = g_pSceneManager->GetScene( m_SceneInitData.m_CityData.m_PortSceneID )  ;
		if( pScene == NULL )
		{
			Assert(FALSE);
			return;
		}

		CITYSCENE_DATA* pCitySceneData = pScene->GetCityData();
		if(!pCitySceneData->AddPort(SceneID()))
		{
			Assert(FALSE);
			return;
		}
	}

	m_pLuaInterface->ExeScript_D( SCENE_SCRIPTID, "OnSceneInit", (INT)SceneID() ) ;

__LEAVE_FUNCTION
}

VOID Scene::OnSceneTimer( UINT uTime )
{
__ENTER_FUNCTION

	m_pLuaInterface->ExeScript_DD( SCENE_SCRIPTID,"OnSceneTimer", (INT)SceneID(), (INT)uTime );

__LEAVE_FUNCTION
}

VOID Scene::OnSceneQuit( )
{
__ENTER_FUNCTION

	m_pLuaInterface->ExeScript_D( SCENE_SCRIPTID, "OnSceneQuit", (INT)SceneID() ) ;

	GetScenePlayerManager()->RemoveAllPlayer() ;
	GetMonsterManager()->RemoveAllMonster() ;
	m_pPlatformManager->RemoveAllPlatform() ;
	GetPetManager()->RemoveAllPet() ;
	GetItemBoxManager()->RemoveAllItemBox() ;


__LEAVE_FUNCTION
}

VOID Scene::OnScenePlayerEnter( GamePlayer* pGamePlayer )
{
__ENTER_FUNCTION
	Obj_Human* pHuman = pGamePlayer->GetHuman();
	if(NULL!=pHuman)
	{
		m_pLuaInterface->ExeScript_DD( SCENE_SCRIPTID,"OnScenePlayerEnter", (INT)SceneID(), (INT)pHuman->GetID() );
	}
__LEAVE_FUNCTION
}

VOID Scene::OnSceneHumanLevelUp( Obj_Human* pHuman )
{
__ENTER_FUNCTION

	m_pLuaInterface->ExeScript_DDD( SCENE_SCRIPTID,"OnSceneHumanLevelUp", (INT)SceneID(), (INT)pHuman->GetID(), (INT)pHuman->GetLevel() );

__LEAVE_FUNCTION
}

VOID Scene::OnSceneHumanDie(Obj_Human* pHuman, ObjID_t idKiller)
{
__ENTER_FUNCTION

	m_pLuaInterface->ExeScript_DDD( SCENE_SCRIPTID,"OnSceneHumanDie", (INT)SceneID(), (INT)pHuman->GetID(), (INT)idKiller );

__LEAVE_FUNCTION
}

VOID Scene::OnScenePlayerDisconnect( GamePlayer* pGamePlayer )
{
}

VOID Scene::OnScenePlayerLeave( GamePlayer* pGamePlayer ) 
{
__ENTER_FUNCTION

	Obj_Human* pHuman = pGamePlayer->GetHuman();

	//清除场景缓存里面的此用户消息
	MovePacket( pGamePlayer->PlayerID() ) ;


	//交易一方离开场景或掉线
	if(pHuman->m_ExchangBox.m_ObjID != INVALID_ID)
	{//正在跟别人交易中
		ObjID_t	TargetID = pHuman->m_ExchangBox.m_ObjID;
		Obj_Human* pTargetHuman = GetHumanManager()->GetHuman( TargetID );
		if( pTargetHuman != NULL )
		{//对方没掉线
			if( pTargetHuman->m_ExchangBox.m_Status > 0
				&& pTargetHuman->m_ExchangBox.m_ObjID == pHuman->GetID()
				)
			{//对方也确实是在交易状态,并在同自己交易
				GCExchangeError Msg;
				Msg.SetID(EXCHANGE_MSG::ERR_DROP);
				pTargetHuman->GetPlayer()->SendPacket(&Msg);
				pTargetHuman->m_ExchangBox.CleanUp();
			}
		}
		pHuman->m_ExchangBox.CleanUp();
	}


	if(pHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_CLOSE)
	{
		pHuman->m_StallBox.CleanUp();
	}

__LEAVE_FUNCTION
}

VOID Scene::OnSceneNotify( SceneID_t DestSceneID )
{
__ENTER_FUNCTION

	m_pLuaInterface->ExeScript_DD( SCENE_SCRIPTID,"OnSceneNotify", (INT)SceneID(), (INT)DestSceneID );
//	m_pLuaInterface->ExeFile_DD( "scene.lua","OnSceneNotify", (INT)SceneID(), (INT)DestSceneID );

__LEAVE_FUNCTION

	return ;

	////测试数据
	//Scene* pScene = g_pSceneManager->GetScene( DestSceneID ) ;
	//GUID_t TeamLeader = pScene->m_CopyData.m_TeamLeader ;
	//Obj_Human* pHuman = ((Obj_Human*)(g_pGUIDManager->Get(TeamLeader))) ;
	//if( !pHuman )
	//{
	//	Assert(FALSE) ;
	//	return ;
	//}
	//GamePlayer* pGamePlayer = (GamePlayer*)(pHuman->GetPlayer()) ;


	//WORLD_POS New ;
	//New.m_fX = 11.0f ;
	//New.m_fZ = 11.0f ;

	//pHuman->ChangeScene( SceneID(), DestSceneID, New, 9 ) ;

	//GCNotifyChangeScene Msg ;
	//Msg.setCurrentSceneID( SceneID() ) ;
	//Msg.setTargetSceneID( DestSceneID ) ;
	//Msg.setTargetPos( &New ) ;
	//Msg.setTargetDir( 9 ) ;
	//				
	//pGamePlayer->SendPacket( &Msg ) ;
	//测试数据
}

BOOL Scene::OnAcceptMissionCheck(Obj_Human* pHuman,ScriptID_t idMissionScript)
{
__ENTER_FUNCTION

	BOOL bRet = FALSE ;
	bRet = m_pLuaInterface->ExeScript_DDD(	SCENE_SCRIPTID,
											"OnAcceptMissionCheck", 
											(INT)SceneID(), 
											(INT)pHuman->GetID(),
											idMissionScript ) ;
	
	return bRet ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID Scene::OnScenePlayerTimer( Obj_Human* pHuman, UINT uTime )
{
	__ENTER_FUNCTION

		m_pLuaInterface->ExeScript_DDD(	SCENE_SCRIPTID,
		"OnScenePlayerTimer", 
		(INT)SceneID(), 
		(INT)pHuman->GetID(),
		uTime ) ;

	__LEAVE_FUNCTION
}

VOID Scene::OnScenePlayerFirstLogin( Obj_Human* pHuman )
{
	__ENTER_FUNCTION

		m_pLuaInterface->ExeScript_DDD(	SCENE_SCRIPTID,
		"OnScenePlayerFirstLogin", 
		(INT)SceneID(), 
		(INT)pHuman->GetID(),
		pHuman->NowTime() ) ;

	__LEAVE_FUNCTION
}

VOID Scene::OnScenePlayerLogin( Obj_Human* pHuman )
{
	__ENTER_FUNCTION

		m_pLuaInterface->ExeScript_DDD(	SCENE_SCRIPTID,
		"OnScenePlayerLogin", 
		(INT)SceneID(), 
		(INT)pHuman->GetID(),
		pHuman->NowTime() ) ;

	__LEAVE_FUNCTION
}
