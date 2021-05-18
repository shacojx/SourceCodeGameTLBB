#include "stdafx.h"
#include "WGWorldTime.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GCWorldTime.h"
#include "PacketFactoryManager.h"



UINT	WGWorldTimeHandler::Execute(WGWorldTime* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION

		Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );
		Assert(pPacket);

		g_WorldTime = pPacket->GetWorldTime();

		for( INT i=0; i<MAX_SCENE; i++ )
		{
			Scene* pScene = g_pSceneManager->GetScene( (SceneID_t)i ) ;
			if( pScene==NULL )
				continue ;

			if( pScene->GetPlayerManager()->GetCount()<=0 )
				continue ;
			
			GCWorldTime* pMsg = (GCWorldTime*)(g_pPacketFactoryManager->CreatePacket(PACKET_GC_WORLDTIME)) ;

			pMsg->SetWorldTime(g_WorldTime);
			pMsg->SetSceneID(pScene->SceneID());

			pScene->SendPacket(pMsg,INVALID_ID);

		}
		
		g_pLog->FastSaveLog( LOG_FILE_1,"WGWorldTimeHandler::Execute()....OK!");

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}