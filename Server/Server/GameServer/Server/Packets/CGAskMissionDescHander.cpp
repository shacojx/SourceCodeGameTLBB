#include "stdafx.h"
#include "CGAskMissionDesc.h"
#include "OResultDef.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GCRetMissionDesc.h"


using namespace Packets;

uint CGAskMissionDescHandler::Execute(CGAskMissionDesc* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer != NULL ) ;
	if ( pGamePlayer == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskMissionDescHandler: Error (pGamePlayer == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman != NULL ) ;
	if ( pHuman == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskMissionDescHandler: Error (pHuman == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Scene* pScene = pHuman->getScene() ;
	Assert( pScene != NULL ) ;
	if ( pScene == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskMissionDescHandler: Error Obj = %d (pScene == NULL) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	if ( MyGetCurrentThreadID() != pScene->m_ThreadID )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskMissionDescHandler: Error Obj = %d (MyGetCurrentThreadID() != pScene->m_ThreadID) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	UINT nMissionIndex = pHuman->GetMissionIndexByID( pPacket->getMissionIndex() );
	const _OWN_MISSION* pMission =	pHuman->GetMission( nMissionIndex ) ;

	_MISSION_LIST_t Mis;
	if( g_TableInit.GetMissionListInfo( pMission->m_idScript, Mis ) )
	{
		_MISSION_DATA_t* pData = g_MissionDataTable.Get( Mis.nMissionID );
		Assert( pData );

		_MISSION_DIALOG_t* pDialog = g_MissionDialogTable.Get( Mis.nDialog );
		Assert( pDialog );

		_MISSION_REWARD_t* pReward = NULL;
		if( Mis.nReward != -1 )
		{
			pReward = g_MissionRewardTable.Get( Mis.nReward );
			Assert( pReward );
		}

		//GetDestScene
		//Scene* pDestScene = (Scene*)( g_pSceneManager->GetScene( Mis.nDestScene ) );
		//Assert( pDestScene );

		GCRetMissionDesc msg;
		msg.SetMissionIndex( nMissionIndex );

		BOOL bSend = FALSE;
		BOOL bConvert;
		CHAR szFormatString[2048];
		//format name
		pScene->FormatMissionString( pDialog->szMissionName, szFormatString, pHuman, &Mis, pData, pReward, bConvert );
		if( bConvert )
		{
			msg.SetMissionName( szFormatString );
			bSend = TRUE;
		}
		//format desc
		pScene->FormatMissionString( pDialog->szMissionDesc, szFormatString, pHuman, &Mis, pData, pReward, bConvert );
		if( bConvert )
		{
			msg.SetMissionDesc( szFormatString );
			bSend = TRUE;
		}
		//format target
		pScene->FormatMissionString( pDialog->szMissionTarget, szFormatString, pHuman, &Mis, pData, pReward, bConvert );
		if( bConvert )
		{
			msg.SetMissionTarget( szFormatString );
			bSend = TRUE;
		}

		if( bSend )
		{
			pGamePlayer->SendPacket(&msg);
		}
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;

}
