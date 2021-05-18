#include "stdafx.h"
/********************************************************************************
 *	文件名：	WGTeamFollowListHandler.cpp
 *	全路径：	d:\Prj\Server\Server\Packets\WGTeamFollowListHandler.cpp
 *	创建时间：	2005 年 11 月 23 日	23:46
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "WGTeamFollowList.h"
#include "GCTeamFollowList.h"
#include "GCReturnTeamFollow.h"

#include "Log.h"
#include "ServerManager.h"
#include "PlayerPool.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"

uint WGTeamFollowListHandler::Execute( WGTeamFollowList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGTeamFollowListHandler::Execute pGamePlayer==NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if (pGamePlayer->m_HumanGUID != pPacket->GetGUID())
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGTeamFollowListHandler::Execute pGamePlayer->m_HumanGUID[%d] != pPacket->GetGUID()[%d]",pGamePlayer->m_HumanGUID, pPacket->GetGUID());
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );
	Scene* pScene = pHuman->getScene();
	if( !pScene )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGTeamFollowListHandler::Execute pHuman->getScene() == NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if( pPlayer->IsServerPlayer() )
	{//服务器收到世界服务器发来的数据
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamFollowListHandler: ServerPlayer (GUID=%X) ",
			pHuman->GetGUID() );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//场景收到Cache里的消息
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		TeamInfo* pTeamInfo = pHuman->GetTeamInfo();
		if( pTeamInfo->HasTeam() == FALSE )
		{
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}

		pHuman->__ClearFollowedMembers(); // 清空旧信息

		_FOLLOWEDMEMBER myInfo;
		myInfo.m_GUID = pHuman->GetGUID();
		myInfo.m_pHuman = pHuman;

		for( INT i=0; i<pPacket->GetMemberCount(); ++i )
		{
			_FOLLOWEDMEMBER info;
			const TEAMMEMBER* pMember;

			info.m_GUID = pPacket->GetFollowMember(i);
			info.m_pHuman = NULL;
			pMember = pTeamInfo->GetTeamMemberByGUID( info.m_GUID );

			if ( pMember->m_SceneID == pScene->SceneID() )
			{
				info.m_pHuman = pScene->GetHumanManager()->GetHuman( pMember->m_ObjID );

				if ( info.m_pHuman!=NULL )
				{
					if ( info.m_pHuman->GetGUID()!=pMember->m_GUID )
					{ // 宁缺勿滥
						info.m_pHuman = NULL;
					}
					else
					{ // 让跟随队友把自己加进去，以更新 pHuman 指针
						info.m_pHuman->__AddFollowedMember( myInfo );
					}
				}
			}

			pHuman->__AddFollowedMember( info );
		}

		if ( pPacket->GetMemberCount()>0 )
		{
			pHuman->__SetTeamFollowFlag( TRUE );

			GCReturnTeamFollow Msg; // 通知客户端进入跟随状态
			Msg.SetGUID( pHuman->GetGUID() );
			Msg.SetReturn( TF_RESULT_FOLLOW_FLAG );
			pGamePlayer->SendPacket( &Msg );
		}
		else
		{ // 队长断线退出，World 可能发送一个空的列表过来
			pHuman->__SetTeamFollowFlag( FALSE );
			pHuman->GetHumanAI()->PushCommand_StopTeamFollow();

			GCReturnTeamFollow Msg; // 通知客户端进入跟随状态
			Msg.SetGUID( pHuman->GetGUID() );
			Msg.SetReturn( TF_RESULT_STOP_FOLLOW );
			pGamePlayer->SendPacket( &Msg );
		}

		if ( pTeamInfo->IsLeader() )
		{ // 队长
			GCTeamFollowList Msg;
			Msg.SetObjID( pHuman->GetID() );

			for( INT i=1; i<pHuman->__GetFollowedMembersCount(); ++i )
			{
				Obj_Human* pMember;

				pMember = pHuman->__GetFollowedMember(i)->m_pHuman;
				Msg.AddFollowMember( pHuman->__GetFollowedMember(i)->m_GUID );

				if ( pMember!=NULL && pMember->getZoneID()!=INVALID_ID )
				{ // 必须进入了场景才跟随
					pMember->GetHumanAI()->PushCommand_TeamFollow();
				}
			}

			if ( pHuman->getZoneID() != INVALID_ID )
			{
				pScene->BroadCast( &Msg, pHuman, TRUE );
			}
		}
		else if ( pPacket->GetMemberCount()>0 )
		{ // 是跟随者
			Obj_Human* pLeader = pHuman->__GetFollowedMember(0)->m_pHuman;

			if ( pLeader!=NULL && pLeader->getZoneID()!=INVALID_ID )
			{ // 必须进入了场景才跟随
				pHuman->GetHumanAI()->PushCommand_TeamFollow();
			}
		}

		g_pLog->FastSaveLog( LOG_FILE_1, "WGTeamFollowListHandler: GamePlayer (GUID=%X) ",
			pHuman->GetGUID() );
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
