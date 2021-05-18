#include "stdafx.h"


#include "WGMail.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "SceneManager.h"

#include "GCMail.h"
#include "GWAskMail.h"

#define SCRIPT_MAIL 888889

uint WGMailHandler::Execute( WGMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	PlayerID_t PlayerID = pPacket->GetPlayerID();
	GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID);
	if( pGamePlayer==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGMailHandler::Execute pGamePlayer==NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if (pGamePlayer->m_HumanGUID != pPacket->GetGUID())
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGMailHandler::Execute pGamePlayer->m_HumanGUID[%d] != pPacket->GetGUID()[%d]",pGamePlayer->m_HumanGUID, pPacket->GetGUID());
		return PACKET_EXE_CONTINUE;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Scene* pScene = pHuman->getScene();
	if( !pScene )
	{
		g_pLog->FastSaveLog( LOG_FILE_3, "WGMailHandler::Execute pHuman->getScene() == NULL" );
		return PACKET_EXE_CONTINUE;
	}

	if( pPlayer->IsServerPlayer() )
	{//服务器收到世界服务器发来的数据
		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID ) ;

		pScene->SendPacket( pPacket, PlayerID ) ;

		g_pLog->FastSaveLog( LOG_FILE_1, "WGMailHandler: ServerPlayer (Name=%s, MailCount=%d, LeftCount=%d)",
			pHuman->GetName(), pPacket->GetMailList()->m_Count, pPacket->GetMailList()->m_TotalLeft ) ;

		return PACKET_EXE_NOTREMOVE ;
	}
	else if( pPlayer->IsGamePlayer() )
	{//场景收到Cache里的消息
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

		MAIL_LIST* pMailList = pPacket->GetMailList() ;
		if( pMailList->m_Count>0 && pMailList->m_aMail[0].m_uFlag==MAIL_TYPE_SCRIPT )
		{//服务器收到脚本邮件，执行之
			for( INT i=0; i<pMailList->m_Count; i++ )
			{
				MAIL* pMail = &(pMailList->m_aMail[i]) ;
				pScene->GetLuaInterface()->ExeScript_DDDDDD( SCRIPT_MAIL, 
															 "ExecuteMail", 
															 pScene->SceneID(), 
															 pHuman->GetID(),
															 pMail->m_uParam0,
															 pMail->m_uParam1,
															 pMail->m_uParam2,
															 pMail->m_uParam3 ) ;
			}

			if( pMailList->m_TotalLeft>0 )
			{//如果还有脚本邮件，则继续向世界数据服务器请求
				GWAskMail Msg ;
				Msg.SetAskType( ASK_TYPE_LOGIN ) ;
				Msg.SetGUID( pHuman->GetGUID() ) ;
				g_pServerManager->SendPacket( &Msg, INVALID_ID ) ;
			}
		}
		else
		{//服务器收到普通邮件，发给客户端
			GCMail Msg ;
			Msg.SetMailList( pMailList ) ;
			pGamePlayer->SendPacket( &Msg ) ;
		}

		g_pLog->FastSaveLog( LOG_FILE_1, "WGMailHandler: GamePlayer (Name=%s, MailCount=%d, LeftCount=%d)",
			pHuman->GetName(), pPacket->GetMailList()->m_Count, pPacket->GetMailList()->m_TotalLeft ) ;
	}
	else
	{
		Assert(FALSE) ;
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
