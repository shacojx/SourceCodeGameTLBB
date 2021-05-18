#include "stdafx.h"


#include "CGStallShopName.h"

#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

uint CGStallShopNameHandler::Execute( CGStallShopName* pPacket, Player* pPlayer )
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

		if(pHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_OPEN)
		{
			GCStallError	Msg;
			Msg.SetID(STALL_MSG::ERR_ILLEGAL);
			pGamePlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallShopNameHandler::ObjID=%d, ERR_ILLEGAL: != ServerStallBox::STALL_OPEN"
				,pHuman->GetID()) ;
			return PACKET_EXE_CONTINUE;
		}
		//修改服务器数据
		pHuman->m_StallBox.SetStallName(pPacket->GetStallName(),pPacket->GetStallNameSize());
		
		g_pLog->FastSaveLog( LOG_FILE_1, "CGStallShopNameHandler::ObjID=%d"
			,pHuman->GetID()) ;

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}
