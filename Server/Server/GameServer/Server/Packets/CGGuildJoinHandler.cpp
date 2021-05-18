#include "stdafx.h"
//客户端向服务器申请建立工会
#include "CGGuildJoin.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "GWGuild.h"
UINT CGGuildJoinHandler::Execute( CGGuildJoin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}
	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	GuildID_t	GuildID = pPacket->GetGuildID();

	GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);

	pMsg->SetGUID( pHuman->GetGUID() );
	_GUILD_CGW_PACKET* pGuildPacket = pMsg->GetGuildPacket();
	pGuildPacket->m_uPacketType = GUILD_PACKET_GW_JOIN;

	GUILD_CGW_JOIN* pPacketJoin = (GUILD_CGW_JOIN*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_JOIN));

	pPacketJoin->m_GuildGUID = GuildID;
	pPacketJoin->m_Camp = pHuman->GetCampData()->m_nCampID;

	g_pServerManager->SendPacket( pMsg, INVALID_ID );
	
	g_pLog->FastSaveLog( LOG_FILE_1, "CGGuildJoinHandler::Name=%s"
		,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
