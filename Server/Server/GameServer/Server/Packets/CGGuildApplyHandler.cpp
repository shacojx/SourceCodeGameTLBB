#include "stdafx.h"
//客户端向服务器申请建立工会
#include "CGGuildApply.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "GWGuild.h"
UINT CGGuildApplyHandler::Execute( CGGuildApply* pPacket, Player* pPlayer )
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
	BYTE	GuildNameSize		=	pPacket->GetGuildNameSize();	
	CHAR*	pszGuildName		=	pPacket->GetGuildName();	
	if( GuildNameSize >= MAX_GUILD_NAME_SIZE ) GuildNameSize = MAX_GUILD_NAME_SIZE-1;
	pszGuildName[MAX_GUILD_NAME_SIZE-1] = 0;
	if( !StrSafeCheck( pszGuildName, GuildNameSize ) )
	{
		Assert( FALSE );
	}

	BYTE	GuildDescSize		=	pPacket->GetGuildDescSize();	
	CHAR*	pszGuildDesc		=	pPacket->GetGuildDesc();		
	if( GuildDescSize >= MAX_GUILD_DESC_SIZE ) GuildDescSize = MAX_GUILD_DESC_SIZE-1;
	pszGuildDesc[MAX_GUILD_DESC_SIZE-1] = 0;
	if( !StrSafeCheck( pszGuildDesc, GuildDescSize ) )
	{
		Assert( FALSE );
	}

	GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);

	pMsg->SetGUID( pHuman->GetGUID() );
	_GUILD_CGW_PACKET* pGuildPacket = pMsg->GetGuildPacket();
	pGuildPacket->m_uPacketType = GUILD_PACKET_GW_CREATE;

	GUILD_CGW_CREATE* pPacketCreate;
	pPacketCreate = (GUILD_CGW_CREATE*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_CREATE));

	pPacketCreate->m_NameSize = GuildNameSize;
	strncpy(pPacketCreate->m_szGuildName, pszGuildName, GuildNameSize);

	pPacketCreate->m_DescSize = GuildDescSize;
	strncpy(pPacketCreate->m_szGuildDesc, pszGuildDesc, GuildDescSize);

	pPacketCreate->m_Camp = pHuman->GetCampData()->m_nCampID;

	g_pServerManager->SendPacket( pMsg, INVALID_ID );

	
	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopDescHandler::Name=%s"
		,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
