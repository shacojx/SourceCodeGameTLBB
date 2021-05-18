#include "stdafx.h"
/********************************************************************************
 *	文件名：	CGReturnTeamFollowHandler.cpp
 *	全路径：	d:\Prj\Server\Server\Packets\CGReturnTeamFollowHandler.cpp
 *	创建时间：	2005 年 11 月 23 日	18:00
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "CGReturnTeamFollow.h"

#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "PacketFactoryManager.h"

#include "GCReturnTeamFollow.h"
#include "GCTeamFollowErr.h"
#include "GCTeamFollowList.h"
#include "GWEnterTeamFollow.h"

uint CGReturnTeamFollowHandler::Execute( CGReturnTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );
	
	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	TeamInfo* pTeamInfo = pHuman->GetTeamInfo();
	if( pTeamInfo->HasTeam() == FALSE )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	const TEAMMEMBER* pLeaderInfo = pTeamInfo->Leader();
	// 这里不对自己是否队长进行检查

	if( pLeaderInfo->m_SceneID != pScene->SceneID() )
	{ // 不在同一个场景
		g_pLog->FastSaveLog( LOG_FILE_1, "CGReturnTeamFollow: GUID=%X is not in the same scene with team leader.", 
			pHuman->GetGUID() );
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pLeader = pScene->GetHumanManager()->GetHuman( pLeaderInfo->m_ObjID );
	if( pLeader==NULL )
	{ // 没有找到队长
		g_pLog->FastSaveLog( LOG_FILE_1, "CGReturnTeamFollow: GUID=%X can't find team leader.", 
			pHuman->GetGUID() );
		return PACKET_EXE_CONTINUE;
	}

	// 如果自己正在摆摊，则不允许跟随
	if(pHuman->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
	{
		GCTeamFollowErr Msg;
		Msg.SetError( TF_ERROR_STALL_OPEN );

		pGamePlayer->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGReturnTeamFollow: GUID=%X can't follow when open stall.", 
			pHuman->GetGUID() );
		return PACKET_EXE_CONTINUE;
	}

	if( pPacket->GetReturn() )
	{ // 同意跟随
		// 检查跟队长之间的距离是否超过 10 米
		if( pHuman->IsInValidRadius(pLeader, (FLOAT)g_Config.m_ConfigInfo.m_nAvailableFollowDist) == FALSE )
		{ // 超过则返回错误信息
			GCTeamFollowErr Msg;
			Msg.SetError( TF_ERROR_TOO_FAR );

			pGamePlayer->SendPacket( &Msg );

			g_pLog->FastSaveLog( LOG_FILE_1, "CGReturnTeamFollow: GUID=%X can't follow for the distance.", 
				pHuman->GetGUID() );
			return PACKET_EXE_CONTINUE;
		}

		// 检查队长是否出于组队跟随状态
		if( pLeader->__GetTeamFollowFlag() == FALSE )
		{ // 队长不处于组队跟随状态，可能队长已经主动取消跟随了
			GCTeamFollowErr Msg;
			Msg.SetError( TF_ERROR_NOT_IN_FOLLOW_MODE );

			pGamePlayer->SendPacket( &Msg );

			g_pLog->FastSaveLog( LOG_FILE_1, "CGReturnTeamFollow: GUID=%X can't follow for not in follow mode.", 
				pHuman->GetGUID() );
			return PACKET_EXE_CONTINUE;
		}

		if ( pHuman->__GetTeamFollowFlag() )
		{ // 如果已经处于此状态，则返回
			return PACKET_EXE_CONTINUE;
		}

		pHuman->__SetTeamFollowFlag( TRUE );

		_FOLLOWEDMEMBER MyInfo;
		MyInfo.m_GUID = pHuman->GetGUID();
		MyInfo.m_pHuman = pHuman;

		// 将新的跟随列表发给可见范围内所有客户端
		// 只有队长有这个列表，这样可以进行优化
		GCTeamFollowList Msg;
		Msg.SetObjID( pLeader->GetID() );

		INT nMaxFollowedCount = pLeader->__GetFollowedMembersCount();

		for( INT i=0; i<nMaxFollowedCount; ++i )
		{ // 所有跟随者加上自己，自己加上每个跟随者
			Obj_Human* pMember;
			const _FOLLOWEDMEMBER* pFollowedMember;

			pFollowedMember = pLeader->__GetFollowedMember(i);

			pMember = pFollowedMember->m_pHuman;
			if( pMember!=NULL )
			{ // 没有断线并且在同一场景
				pMember->__AddFollowedMember( MyInfo );
			}

			pHuman->__AddFollowedMember( *pFollowedMember );
			Msg.AddFollowMember( pFollowedMember->m_GUID );
		}

		pHuman->__AddFollowedMember( MyInfo ); // 自己加上自己
		pHuman->GetHumanAI()->PushCommand_TeamFollow();

		Msg.AddFollowMember( MyInfo.m_GUID );

		//GCNotifyTeamInfo notifyMsg;
		//notifyMsg.SetObjID( pHuman->GetID() );
		//notifyMsg.SetHaveTeamFlag( pTeamInfo->HasTeam() );
		//notifyMsg.SetTeamLeaderFlag( pTeamInfo->IsLeader() );
		//notifyMsg.SetTeamFullFlag( pTeamInfo->IsFull() );
		//notifyMsg.SetTeamFollowFlag( pHuman->__GetTeamFollowFlag() );
		//pScene->BroadCast( &notifyMsg, pHuman );

		pScene->BroadCast( &Msg, pLeader, TRUE ); // 发送跟随列表给所有客户端，以供路线模拟

		GCReturnTeamFollow RetMsg;
		RetMsg.SetReturn( TF_RESULT_ENTER_FOLLOW );
		RetMsg.SetGUID( pHuman->GetGUID() );

		pGamePlayer->SendPacket( &RetMsg ); // 发给自己
		pLeader->GetPlayer()->SendPacket( &RetMsg ); // 发给队长

		// 这里需要通知 world GWEnterTeamFollow
		GWEnterTeamFollow* pMsg = (GWEnterTeamFollow*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_ENTER_TEAMFOLLOW);

		pMsg->SetGUID( pHuman->GetGUID() );

		g_pServerManager->SendPacket( pMsg, INVALID_ID );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGReturnTeamFollow: GUID=%X success.", 
			pHuman->GetGUID() );
		return PACKET_EXE_CONTINUE;
	}
	else
	{ // 不同意跟随
		GCReturnTeamFollow Msg;
		Msg.SetReturn( TF_RESULT_REFUSE_FOLLOW );
		Msg.SetGUID( pHuman->GetGUID() );

		pLeader->GetPlayer()->SendPacket( &Msg );

		g_pLog->FastSaveLog( LOG_FILE_1, "CGReturnTeamFollow: GUID=%X refuse to follow.", 
			pHuman->GetGUID() );
		return PACKET_EXE_CONTINUE;
	}
	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
