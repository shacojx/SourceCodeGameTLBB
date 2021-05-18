#include "stdafx.h"
/********************************************************************************
 *	文件名：	CGStopTeamFollowHandler.cpp
 *	全路径：	d:\Prj\Server\Server\Packets\CGStopTeamFollowHandler.cpp
 *	创建时间：	2005 年 11 月 23 日	22:55
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "CGStopTeamFollow.h"

#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "Scene.h"

#include "PacketFactoryManager.h"
#include "ServerManager.h"

#include "GWStopTeamFollow.h"
#include "GCReturnTeamFollow.h"
#include "GCTeamFollowList.h"

uint CGStopTeamFollowHandler::Execute( CGStopTeamFollow* pPacket, Player* pPlayer )
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

	if ( pHuman->__GetTeamFollowFlag() )
	{
		pHuman->__StopTeamFollow();

		g_pLog->FastSaveLog( LOG_FILE_1, "CGStopTeamFollow: GUID=%X success.", 
			pHuman->GetGUID() );
	}
	else
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGStopTeamFollow: GUID=%X doesn't in team follow mode.", 
			pHuman->GetGUID() );
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
