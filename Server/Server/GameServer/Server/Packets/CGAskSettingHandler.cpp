#include "stdafx.h"


#include "CGAskSetting.h"
#include	"Log.h"
#include	"GamePlayer.h"
#include	"Scene.h"
#include	"GCRetSetting.h"

using namespace Packets;


uint CGAskSettingHandler::Execute( CGAskSetting* pPacket, Player* pPlayer )
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

	GCRetSetting Msg ;
	_OWN_SETTING* pSetting = (_OWN_SETTING*)(pHuman->GetSettingList()->m_aSetting) ;
	Msg.SetSetting( pSetting ) ;
	pGamePlayer->SendPacket(&Msg) ;


	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskSettingHandler::SOCKET=%d OK!",
		pPlayer->GetSocket()->getSOCKET() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
