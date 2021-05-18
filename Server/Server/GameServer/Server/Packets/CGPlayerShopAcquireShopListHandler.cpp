#include "stdafx.h"
/*
客户端申请商店列表
*/

#include "CGPlayerShopAcquireShopList.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCPlayerShopAcquireShopList.h"

UINT CGPlayerShopAcquireShopListHandler::Execute( CGPlayerShopAcquireShopList* pPacket, Player* pPlayer )
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
	SHOP_INFO_t aShopList[MAX_SHOP_NUM_PER_SCENE];
	UINT ListIndex = 0;
	PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
	UINT uShopNum = pPlayerShopManager->GetPlayerShopNum();

	for(UINT i = 0; i<uShopNum; i++)
	{
		PlayerShop*	pPlayerShop	= pPlayerShopManager->GetPlayerShopByIndex(i);
		Assert(pPlayerShop);
		aShopList[ListIndex].m_ID				= pPlayerShop->GetShopGUID();
		memcpy(aShopList[ListIndex].m_ShopName, pPlayerShop->GetShopName(), MAX_SHOP_NAME_SIZE);
		aShopList[ListIndex].m_StallNumOnSale	= pPlayerShop->GetNumStallOnSale();
		aShopList[ListIndex].m_StallNumOpened	= pPlayerShop->GetNumStallOpened();
		aShopList[ListIndex++].m_Type			= (BYTE)pPlayerShop->GetShopType();
	}

	GCPlayerShopAcquireShopList Msg;
	Msg.SetShopNum(ListIndex);
    Msg.SetShopList(aShopList);

	pGamePlayer->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopAcquireShopList::Name=%s"
		,pHuman->GetName()) ;


	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
