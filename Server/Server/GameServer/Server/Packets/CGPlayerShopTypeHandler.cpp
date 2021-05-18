#include "stdafx.h"

#include "CGPlayerShopType.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCPlayerShopType.h"
//#include "RecordOpt.h"

UINT CGPlayerShopTypeHandler::Execute( CGPlayerShopType* pPacket, Player* pPlayer )
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
	_PLAYERSHOP_GUID	nShopID	=	pPacket->GetShopID();			//商店ID
	BYTE				nType	=	pPacket->GetType();				//存到哪
	
	PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
	PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
	Assert(pPlayerShop);


	if(pPlayerShop->GetShopStatus() == STATUS_PLAYER_SHOP_ON_SALE)
	{//商店未开张
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopMoneyHandler::Name=%s shop close"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	//是不是自己的店
	BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

	//是不是自己可以管理的店
	BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

	if(bIsMine == FALSE )
	{
		//log...
		return PACKET_EXE_CONTINUE;
	}


	GCPlayerShopType	MsgToClient;
	
	TYPE_PLAYER_SHOP CurrentType = pPlayerShop->GetShopType();
	if((BYTE)CurrentType == nType)
	{
		//log...
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopTypeHandler::Name=%s, needn't change"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	pPlayerShop->SetShopType((TYPE_PLAYER_SHOP)nType);

	MsgToClient.SetShopID(nShopID);
	MsgToClient.SetType(nType);

	pGamePlayer->SendPacket(&MsgToClient);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopTypeHandler::Name=%s new type = %d"
		,pHuman->GetName(), nType) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
