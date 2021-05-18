#include "stdafx.h"
/********************************************************************************
 *	文件名：	WGFingerHandler.cpp
 *	全路径：	d:\Prj\Server\Server\Packets\WGFingerHandler.cpp
 *	创建时间：	2006 年 4 月 27 日	16:22
 *
 *	功能说明：	玩家查找的返回信息
 *	修改记录：
*********************************************************************************/

#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"
#include "ServerManager.h"

#include "WGFinger.h"
#include "GCFinger.h"

UINT WGFingerHandler::Execute(WGFinger* pPacket,Player* pPlayer)
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetFinger()->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGFingerHandler::Execute pGamePlayer==NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if (pGamePlayer->m_HumanGUID != pPacket->GetGUID())
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGFingerHandler::Execute pGamePlayer->m_HumanGUID[%d] != pPacket->GetGUID()[%d]",pGamePlayer->m_HumanGUID, pPacket->GetGUID());
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );
	Scene* pScene = pHuman->getScene();
	if( !pScene )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGFingerHandler::Execute pHuman->getScene() == NULL" );
		return PACKET_EXE_CONTINUE;
	}

	WG_FINGER* pRecv = pPacket->GetFinger();

	if( pPlayer->IsServerPlayer() )
	{//服务器收到世界服务器发来的数据
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

		pScene->SendPacket( pPacket, PlayerID );

		g_pLog->FastSaveLog( LOG_FILE_1, "WGFingerHandler: ServerPlayer (Type=%d) ",
			pRecv->m_Type );

		return PACKET_EXE_NOTREMOVE;
	}
	else if( pPlayer->IsGamePlayer() )
	{//场景收到Cache里的消息
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

		GCFinger Msg;
		GC_FINGER* pSend = Msg.GetFinger();
		pSend->CleanUp();
		pSend->m_Type = pRecv->m_Type;

		switch( pRecv->m_Type )
		{
		case FRET_PLAYERLIST:
			{
				RETURN_FINGER_PLAYERLIST& PlayerList = pRecv->m_FingerPlayerList;
				INT nCount;
				nCount = PlayerList.GetPlayerCount();

				pSend->m_FingerPlayerList.CleanUp();

				for( INT i=0; i<nCount; ++i )
				{
					pSend->m_FingerPlayerList.AddPlayerInfo( PlayerList.GetPlayerInfo(i) );
				}

				pSend->m_FingerPlayerList.SetFingerFlag( PlayerList.GetFingerFlag() );
				pSend->m_FingerPlayerList.SetPosition( PlayerList.GetPosition() );

				g_pLog->FastSaveLog( LOG_FILE_1, "WGFingerHandler: Type=%d Count=%d",
					pRecv->m_Type, PlayerList.GetPlayerCount() );
			}
			break;
		case FRET_ERR_NORESULT:
			g_pLog->FastSaveLog( LOG_FILE_1, "WGFingerHandler: Type=%d",
				pRecv->m_Type );
			break;
		default:
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}

		pGamePlayer->SendPacket( &Msg );
		return PACKET_EXE_CONTINUE;
	}
	else
	{
		Assert(FALSE);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
