#include "stdafx.h"

#include "CGGuild.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "GWGuild.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

uint	CGGuildHandler::Execute(CGGuild* pPacket, Player* pPlayer )
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

	_GUILD_CGW_PACKET* pGuildPacket = pPacket->GetGuildPacket();

	GUID_t guid = pHuman->GetGUID();

	switch( pGuildPacket->m_uPacketType )
	{ // 这里可以做一些预判断和处理
	case GUILD_PACKET_CG_ASKLIST:
		{
		}
		break;
	case GUILD_PACKET_CG_CREATE:
		{//合法性验证
			if( pHuman->GetLevel()<40 )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGGuildHandler: GUID=%X too low level", pHuman->GetGUID() );
				return PACKET_EXE_CONTINUE;
			}
			if( pHuman->GetMoney()<500000 )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGGuildHandler: GUID=%X too low money", pHuman->GetGUID() );
				return PACKET_EXE_CONTINUE;
			}
			//if()
		}
		break;
	case GUILD_PACKET_CG_JOIN:
		{
		}
		break;
	case GUILD_PACKET_CG_ASKINFO:
		{
			GUILD_CGW_ASKINFO*  pGuildAskInfo = (GUILD_CGW_ASKINFO*)pGuildPacket->GetPacket(GUILD_PACKET_CG_ASKINFO);
			pGuildAskInfo->m_GuildGUID = pHuman->GetGuildID();
		}
		break;
	case GUILD_PACKET_CG_APPOINT:
		{
		}
		break;
	case GUILD_PACKET_CG_ADJUSTAUTHORITY:
		{
		}
		break;
	case GUILD_PACKET_CG_RECRUIT:
		{
		}
		break;
	case GUILD_PACKET_CG_EXPEL:
		{
		}
		break;
	case GUILD_PACKET_CG_WITHDRAW:
		{
		}
		break;
	case GUILD_PACKET_CG_DEPOSIT:
		{
		}
		break;
	case GUILD_PACKET_CG_LEAVE:
		{
		}
		break;
	case GUILD_PACKET_CG_DISMISS:
		{
		}
		break;
	case GUILD_PACKET_CG_DEMISE:
		{
		}
		break;
	default:
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);
	pMsg->SetGUID(guid);
	pMsg->SetGuildPacket(pGuildPacket);
	pGuildPacket = pMsg->GetGuildPacket();
	pGuildPacket->m_uPacketType += GUILD_PACKET_CG_GW_SEPARATOR + 1;

	g_pServerManager->SendPacket( pMsg, INVALID_ID );

	g_pLog->FastSaveLog( LOG_FILE_1, "CGGuildHandler: GUID=%X", pHuman->GetGUID() );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}