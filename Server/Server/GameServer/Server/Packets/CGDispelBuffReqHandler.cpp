#include "stdafx.h"

#include "Log.h"
#include "GamePlayer.h"
#include "CGDispelBuffReq.h"
#include "Scene.h"

namespace Packets
{
	uint CGDispelBuffReqHandler::Execute( CGDispelBuffReq* pPacket, Player* pPlayer )
	{
		__ENTER_FUNCTION

		GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
		Assert( pGamePlayer ) ;

		Obj_Human* pHuman = pGamePlayer->GetHuman() ;

		if(NULL==pHuman)
		{
			AssertEx(FALSE,"[CGDispelBuffReqHandler::Execute]: NULL Obj_Human pointer found!!");
			return PACKET_EXE_ERROR ;
		}
		Scene* pScene = pHuman->getScene() ;
		if( pScene==NULL )
		{
			AssertEx(FALSE,"[CGDispelBuffReqHandler::Execute]: NULL scene pointer found!!") ;
			return PACKET_EXE_ERROR ;
		}

		//检查线程执行资源是否正确
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

		ObjID_t nMeID = pHuman->GetID();
		if( nMeID == INVALID_ID )
		{
			Assert(FALSE) ;
			return PACKET_EXE_ERROR ;
		}
		//update target
		if(NULL==pPacket)
		{
			AssertEx(FALSE,"[CGDispelBuffReqHandler::Execute]: NULL packet pointer found!!");
			return PACKET_EXE_ERROR ;
		}
		Obj_Character& rMe = *((Obj_Character*)pHuman);
		rMe.Impact_UnRegisterImpactBySN(pPacket->GetSN());
		g_pLog->FastSaveLog( LOG_FILE_1, "CGDispelBuffReqHandler: ok obj=%d scene=%d", 
			nMeID, pScene->SceneID() ) ;
		return PACKET_EXE_CONTINUE ;

		__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
	}

}
