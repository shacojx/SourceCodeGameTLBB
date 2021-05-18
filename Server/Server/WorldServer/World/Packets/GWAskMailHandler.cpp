#include "stdafx.h"


#include "GWAskMail.h"
#include "Log.h"
#include "MailCenter.h"
#include "ServerPlayer.h"
#include "ServerManager.h"
#include "OnlineUser.h"
#include "WGMail.h"
#include "WGNotifyMail.h"

uint GWAskMailHandler::Execute( GWAskMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetGUID() ) ;
	if( pUser==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	ID_t ServerID = pUser->GetServerID();
	ServerPlayer* pServerPlayer = g_pServerManager->GetServerPlayer( ServerID ) ;
	if( pServerPlayer==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_CONTINUE ;
	}

	WGMail Msg ;
	Msg.SetPlayerID( pUser->GetPlayerID() ) ;
	Msg.SetGUID( pPacket->GetGUID() );

	if( pPacket->GetAskType()==ASK_TYPE_LOGIN )
	{
		//处理脚步邮件
		g_pMailCenter->AskScriptMail( pUser->GetName(), Msg.GetMailList() ) ;

		//处理普通邮件
		UINT uMailCount = g_pMailCenter->CheckMail(pUser->GetName());
		if (uMailCount > 0)
		{
			WGNotifyMail NotifyMsg;
			NotifyMsg.SetPlayerID( pUser->GetPlayerID() );
			NotifyMsg.SetGUID( pUser->GetGUID() );
			NotifyMsg.SetMailCount( (UCHAR)(uMailCount>255?255:uMailCount) );
			pServerPlayer->SendPacket( &NotifyMsg );
		}

		if( Msg.GetMailList()->m_Count < 1 )
		{ // 如果没有可执行的脚本邮件,则此消息不往回发.否则会转给客户端,把普通邮件的剩余数量错误设置为0
			return PACKET_EXE_CONTINUE;
		}
	}
	else if( pPacket->GetAskType()==ASK_TYPE_MAIL )
	{
		while(g_pMailCenter->AskMail( pUser->GetName(), Msg.GetMailList() ))
		{
			INT nCount;
			nCount = Msg.GetMailList()->m_Count;
			if( nCount != 1 )
			{ // 这里如果不是 1 封的话处理方式应该有所不同
				Assert( nCount == 1 );
				return PACKET_EXE_CONTINUE;
			}

			if( pUser->ValidateMail( &(Msg.GetMailList()->m_aMail[0]) ) == 0 )
			{ // 邮件有效
				break;
			}
			else
			{ // 无效继续收
				Msg.GetMailList()->CleanUp();
			}
		}

		if( Msg.GetMailList()->m_Count < 1 )
		{ // 一封有效邮件都没有
			return PACKET_EXE_CONTINUE;
		}
	}
	else
	{
		Assert(FALSE) ;
	}

	pServerPlayer->SendPacket( &Msg ) ;

	Log::SaveLog( WORLD_LOGFILE, "GWAskMailHandler...Name=%s MailCount=%d LeftCount=%d",
		pUser->GetName(), Msg.GetMailList()->m_Count, Msg.GetMailList()->m_TotalLeft );

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
