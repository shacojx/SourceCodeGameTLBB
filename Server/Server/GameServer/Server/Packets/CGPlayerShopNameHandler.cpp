#include "stdafx.h"

#include "CGPlayerShopName.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCPlayerShopError.h"

UINT CGPlayerShopNameHandler::Execute( CGPlayerShopName* pPacket, Player* pPlayer )
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
	_PLAYERSHOP_GUID	nShopID				=	pPacket->GetShopID();				//商店ID
	BYTE				PlayerShopNameSize	=	pPacket->GetPlayerShopNameSize();	//商店描述
	CHAR*				PlayerShopName		=	pPacket->GetPlayerShopName();		//获得商店名

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
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopNameHandler::Name=%s shop close"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	//如果玩家身上的金钱≥10金 *商业指数，修改成功。
	pPlayerShopManager->ClampComFactor();
	FLOAT fNeedMoney = (FLOAT)100000.0*pPlayerShopManager->GetComFactor();
	if(pHuman->GetMoney()<(UINT)fNeedMoney)
	{
		//通知客户端购买商店成功
		GCPlayerShopError Msg;
		Msg.SetID(PLAYERSHOP_MSG::ERR_SHOP_NOT_ENOUTH_MONEY_TO_CHANGE_NAME);
		pHuman->GetPlayer()->SendPacket(&Msg);
	}

	//扣钱
	pHuman->SetMoney(pHuman->GetMoney() - (UINT)fNeedMoney);
	
	//更改店名
	pPlayerShop->SetShopName(PlayerShopName, PlayerShopNameSize);

	//更改称号
	CHAR	szMsgTitle[256] = {0};
	sprintf(szMsgTitle, "%s大掌柜", PlayerShopName);
	pHuman->SetShangDianName(szMsgTitle,(BYTE)strlen(szMsgTitle));
	pHuman->UpdateTitlesToClient();
	//通知客户端商店名修改成功
	GCPlayerShopError Msg;
	Msg.SetID(PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_CHANGE_NAME);
	pHuman->GetPlayer()->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopNameHandler::Name=%s"
		,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
