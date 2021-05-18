#include "stdafx.h"
/********************************************************************************
 *	文件名：	WGTeamListHandler.cpp
 *	全路径：	d:\Prj\Server\Server\Packets\WGTeamListHandler.cpp
 *	创建时间：	2006 年 1 月 16 日	10:10
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "WGTeamList.h"
#include "GCTeamList.h"
#include "GCNotifyTeamInfo.h"

#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"

uint WGTeamListHandler::Execute( WGTeamList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGTeamListHandler::Execute pGamePlayer==NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if (pGamePlayer->m_HumanGUID != pPacket->GetGUID())
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGTeamListHandler::Execute pGamePlayer->m_HumanGUID[%d] != pPacket->GetGUID()[%d]",pGamePlayer->m_HumanGUID, pPacket->GetGUID());
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );
	Scene* pScene = pHuman->getScene();
	if( !pScene )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGTeamListHandler::Execute pHuman->getScene() == NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if( pPlayer->IsServerPlayer() )
	{//服务器收到世界服务器发来的数据
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamListHandler: ServerPlayer (GUID=%X, TeamID=%d) ",
			pHuman->GetGUID(), pPacket->GetTeamID() );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//场景收到Cache里的消息
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		TeamInfo* pTeamInfo = pHuman->GetTeamInfo();
		if ( pTeamInfo->HasTeam() )
		{ // 只要客户端请求完成的队伍信息，就清空服务器端的队伍列表
			pTeamInfo->DisMiss();
		}

		pTeamInfo->SetTeamID( pPacket->GetTeamID() );
		pHuman->GetDB()->SetDBTeamID( pPacket->GetTeamID() ) ;
		pTeamInfo->SetMySceneID( pScene->SceneID() );

		GCTeamList Msg;
		Msg.SetTeamID( pPacket->GetTeamID() );

		for( UINT i=0; i<pPacket->GetMemberCount(); ++i )
		{
			const TEAM_LIST_ENTRY& entry = pPacket->GetTeamMember( i );
			TEAM_LIST_ENTRY newEntry;

			TEAMMEMBER Member;
			Member.m_GUID = entry.GetGUID();
			Member.m_SceneID = entry.GetSceneID();
			if ( Member.m_GUID == pGamePlayer->m_HumanGUID )
			{
				Member.m_ObjID = pHuman->GetID();
			}
			else if ( Member.m_SceneID == pScene->SceneID() )
			{
				GamePlayer* ptempGamePlayer = g_pPlayerPool->GetPlayer( entry.GetExtraID() );
				Obj_Human* ptempHuman;

				if ( ptempGamePlayer == NULL )
				{
					Assert(FALSE && "队友失去了连接。");
					Member.m_ObjID = INVALID_ID;
				}
				else if ( (ptempHuman = ptempGamePlayer->GetHuman()) == NULL )
				{
					Assert(FALSE && "队友找不到了……");
					Member.m_ObjID = INVALID_ID;
				}
				else
				{
					Member.m_ObjID = ptempHuman->GetID();
				}
			}
			else
			{
				Member.m_ObjID = INVALID_ID;
			}

			newEntry = entry;
			newEntry.SetExtraID( Member.m_ObjID );

			pTeamInfo->AddMember( &Member );
			Msg.AddTeamMember( newEntry );
		}

		pGamePlayer->SendPacket( &Msg );

		GCNotifyTeamInfo notifyMsg;

		notifyMsg.SetObjID( pHuman->GetID() );
		notifyMsg.SetHaveTeamFlag( pTeamInfo->HasTeam() );

		if ( pTeamInfo->HasTeam() )
		{
			notifyMsg.SetTeamLeaderFlag( pTeamInfo->IsLeader() );
			notifyMsg.SetTeamFullFlag( pTeamInfo->IsFull() );
		}
		else
		{
			notifyMsg.SetTeamLeaderFlag( FALSE );
			notifyMsg.SetTeamFullFlag( FALSE );
		}

		if ( pHuman->getZoneID() != INVALID_ID )
		{
			pScene->BroadCast( &notifyMsg, pHuman );
		}

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamListHandler: GamePlayer (to:%d, GUID=%ld, TeamID=%d) ",
			pPacket->GetPlayerID(), pHuman->GetGUID(), pPacket->GetTeamID() );
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
