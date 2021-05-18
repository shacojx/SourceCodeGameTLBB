#include "stdafx.h"
/********************************************************************************
 *	文件名：	CGAskTeamFollowHandler.cpp
 *	全路径：	d:\Prj\Server\Server\Packets\CGAskTeamFollowHandler.cpp
 *	创建时间：	2005 年 11 月 15 日	18:07
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "CGAskTeamFollow.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "ScanOperator_ActiveTeammates.h"
#include "GWEnterTeamFollow.h"
#include "GCAskTeamFollow.h"
#include "ServerManager.h"
#include "GCReturnTeamFollow.h"

uint CGAskTeamFollowHandler::Execute( CGAskTeamFollow* pPacket, Player* pPlayer )
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

	if ( pHuman->__GetTeamMemberCount() < 2 )
	{ // 人数太少
		return PACKET_EXE_CONTINUE;
	}

	if ( pHuman->__IsTeamLeader() == FALSE )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskTeamFollowHandler: GUID=%X send a fake order.", 
			pHuman->GetGUID() );
		return PACKET_EXE_CONTINUE;
	}

	if ( pHuman->__GetTeamFollowFlag()==FALSE )
	{
		_FOLLOWEDMEMBER FollowedMember;

		FollowedMember.m_GUID = pHuman->GetGUID();
		FollowedMember.m_pHuman = pHuman;

		pHuman->GetHumanAI()->PushCommand_Idle(); // 中止当前逻辑
		pHuman->__SetTeamFollowFlag( TRUE );
		pHuman->__AddFollowedMember( FollowedMember );

		GCReturnTeamFollow Msg; // 通知客户端进入跟随状态
		Msg.SetGUID( pHuman->GetGUID() );
		Msg.SetReturn( TF_RESULT_FOLLOW_FLAG );
		pGamePlayer->SendPacket( &Msg );

		// 这里需要通知 world GWEnterTeamFollow
		GWEnterTeamFollow* pMsg = (GWEnterTeamFollow*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_ENTER_TEAMFOLLOW);

		pMsg->SetGUID( pHuman->GetGUID() );

		g_pServerManager->SendPacket( pMsg, INVALID_ID );
	}

	ScanOperator_ActiveTeammates ScanOp;
	SCANOPERATOR_ACTIVETEAMMATES_INIT init;

	init.m_bScanHuman = TRUE;
	init.m_fRadius = (FLOAT)g_Config.m_ConfigInfo.m_nAvailableFollowDist;
	init.m_MemberGUID = pHuman->GetGUID();
	init.m_nZoneRadius = (INT)init.m_fRadius/g_Config.m_ConfigInfo.m_ZoneSize + 1;
	init.m_Position = *(pHuman->__GetWorldPos());
	init.m_pScene = pScene;
	init.m_TeamID = pHuman->__GetTeamID();
	init.m_ZoneID = pHuman->getZoneID();

	ScanOp.Init( &init );

	if ( pScene->Scan( &ScanOp ) )
	{
		if ( ScanOp.m_nActiveTeammateCount>1 )
		{ // 等于 1 的时候是队长自己
			for( INT i=0; i<ScanOp.m_nActiveTeammateCount; ++i )
			{
				if ( ScanOp.m_aActiveTeammate[i]==NULL )
				{
					Assert(FALSE);
					continue;
				}

				if ( !ScanOp.m_aActiveTeammate[i]->IsAlive() )
				{ // 挂了
					continue;
				}

				if ( ScanOp.m_aActiveTeammate[i]->__GetTeamFollowFlag() )
				{ // 已经进入跟随模式
					continue;
				}

				// 如果队员处于摆摊状态，不发送
				if( ScanOp.m_aActiveTeammate[i]->m_StallBox.GetStallStatus() == ServerStallBox::STALL_OPEN)
				{
					continue;
				}

				// 给客户端发送邀请 GCAskTeamFollow
				GCAskTeamFollow Msg;
				ScanOp.m_aActiveTeammate[i]->GetPlayer()->SendPacket( &Msg );
			}
		}
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskTeamFollowHandler: GUID=%X", 
		pHuman->GetGUID() );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
