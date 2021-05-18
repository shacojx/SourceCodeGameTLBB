#include "stdafx.h"


#include "GWMail.h"
#include "Log.h"
#include "MailCenter.h"
#include "OnlineUser.h"

uint GWMailHandler::Execute( GWMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	USER* pUser = g_pOnlineUser->FindUser( pPacket->GetMail()->m_szDestName );
	if( pUser )
	{//当前用户在线
		if( pUser->RecvMail( pPacket->GetMail() ) == FALSE )
		{
			Log::SaveLog( WORLD_LOGFILE, "GWMailHandler...dName=%s refuse sName=%s's mail.",
				pUser->GetName(), pPacket->GetMail()->m_szSourName );

			return PACKET_EXE_CONTINUE;
		}
	}
	else
	{
		g_pMailCenter->MailTo( pPacket->GetMail() );
	}

	Log::SaveLog( WORLD_LOGFILE, "GWMailHandler...sName=%s dName=%s",
		(pPacket->GetMail()->m_SourSize > 0)?pPacket->GetMail()->m_szSourName:"",
		pPacket->GetMail()->m_szDestName );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
