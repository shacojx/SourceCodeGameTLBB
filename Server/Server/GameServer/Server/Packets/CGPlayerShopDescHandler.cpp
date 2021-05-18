#include "stdafx.h"

#include "CGPlayerShopDesc.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCPlayerShopError.h"

UINT CGPlayerShopDescHandler::Execute( CGPlayerShopDesc* pPacket, Player* pPlayer )
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
	_PLAYERSHOP_GUID	nShopID		=	pPacket->GetShopID();				//商店ID
	BYTE	PlayerShopDescSize		=	pPacket->GetPlayerShopDescSize();	//商店描述的长度
	CHAR*	PlayerShopDesc			=	pPacket->GetPlayerShopDesc();		//商店描述的内容

	PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
	PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
	Assert(pPlayerShop);

	//是不是自己的店.只有店主有资格更改此属性
	BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;
	if(bIsMine == FALSE )
	{
		//log...
		return PACKET_EXE_CONTINUE;
	}

	if(pPlayerShop->GetShopStatus() == STATUS_PLAYER_SHOP_ON_SALE)
	{//商店未开张
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopDescHandler::Name=%s shop close"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	pPlayerShop->SetShopDesc(PlayerShopDesc, PlayerShopDescSize);

	//通知客户端商店描述成功
	GCPlayerShopError Msg;
	Msg.SetID(PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_CHANGE_DESC);
	pHuman->GetPlayer()->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopDescHandler::Name=%s"
		,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
