#include "stdafx.h"
/*
通知服务器要盘入商店
*/

#include "CGPlayerShopBuyShop.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCPlayerShopBuyShop.h"

using namespace PLAYER_SHOP;
UINT CGPlayerShopBuyShopHandler::Execute( CGPlayerShopBuyShop* pPacket, Player* pPlayer )
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

	_PLAYERSHOP_GUID		nShopID		= pPacket->GetShopID();			//商店ID
	BYTE					nSerial		= pPacket->GetSerial();	

	PlayerShopManager*	pPlayerShopManager	= pScene->GetPlayerShopManager();
	PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
	Assert(pPlayerShop);

	if(nSerial!= pPlayerShop->GetSerial())
	{//序列号变化
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopBuyShopHandler::Name=%s Serial changed"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;		
	}

	if(pPlayerShop->GetShopStatus() != STATUS_PLAYER_SHOP_ON_SALE)
	{//不是盘出状态
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopBuyShopHandler::Name=%s Does not sale"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	UINT  uShopPrice = pPlayerShop->GetSaleOutPrice();
	UINT  uSelfMoney = pHuman->GetMoney();
	CHAR  szOldOwner[MAX_CHARACTER_NAME] = {0};
	memcpy(szOldOwner, pPlayerShop->GetOwnerName(), MAX_CHARACTER_NAME);

	if(uSelfMoney < uShopPrice)
	{//金钱不足
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopBuyShopHandler::Name=%s Short of money ,shopprice = %d, money = %d"
			,pHuman->GetName(), uShopPrice, uSelfMoney) ;
		return PACKET_EXE_CONTINUE ;
	}

	if(pPlayerShop->GetShopType() == TYPE_PLAYER_SHOP_ITEM)
	{//物品店
		if(pHuman->GetShopGuid(0).isNull() == FALSE)
		{
			PlayerShop* pCurShop = pPlayerShopManager->GetPlayerShopByGUID(pHuman->GetShopGuid(0));
			if(!pCurShop)
			{
				pHuman->GetShopGuid(0).Reset();
			}
			else if(pCurShop->GetOwnerGuid() == pHuman->GetGUID())
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopBuyShopHandler::Name=%s Already has such item shop"
					,pHuman->GetName()) ;
				return PACKET_EXE_CONTINUE ;
			}
		}
	}
	else if(pPlayerShop->GetShopType() == TYPE_PLAYER_SHOP_PET)
	{//宠物店
		if(pHuman->GetShopGuid(1).isNull() == FALSE)
		{
			PlayerShop* pCurShop = pPlayerShopManager->GetPlayerShopByGUID(pHuman->GetShopGuid(1));
			if(!pCurShop)
			{
				pHuman->GetShopGuid(1).Reset();
			}
			else if(pCurShop->GetOwnerGuid() == pHuman->GetGUID())
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopBuyShopHandler::Name=%s Already has such pet shop"
					,pHuman->GetName()) ;
				return PACKET_EXE_CONTINUE ;
			}
		}
	}

	pPlayerShop->SetOwnerGuid(pHuman->GetGUID());
	pPlayerShop->SetOwnerName(pHuman->GetName());
	pPlayerShop->SetSaleOutPrice(0);
	pPlayerShop->SetShopStatus(STATUS_PLAYER_SHOP_OPEN);

	//扣钱
	pHuman->SetMoney(uSelfMoney-uShopPrice);

	//发可执行邮件，给玩家加钱
	pScene->SendScriptMail(szOldOwner, MAIL_UPDATE_ATTR, MAIL_ATTR_MONEY, uShopPrice, pHuman->GetGUID());

	//通知邮件
	CHAR szMsgContent[MAX_MAIL_CONTEX] = {0};
	sprintf(szMsgContent, "%s 盘入了你的店铺", pHuman->GetName());
	pScene->SendNormalMail(pHuman, szOldOwner, szMsgContent);

	MONEY_LOG_PARAM	MoneyLogParam;
	MoneyLogParam.CharGUID	=	pHuman->GetGUID();
	MoneyLogParam.TargetGUID	=	pPlayerShop->GetOwnerGuid();
	MoneyLogParam.OPType	=	MONEY_BUY_PLAYERSHOP_OUTCOME;	
	MoneyLogParam.Count		=	uShopPrice;
	MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
	MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
	MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
	SaveMoneyLog(&MoneyLogParam);

	

	//商店类型
	if(pPlayerShop->GetShopType() == TYPE_PLAYER_SHOP_ITEM)
	{//物品店
		pHuman->SetShopGuid(0,pPlayerShop->GetShopGUID());
	}
	else if(pPlayerShop->GetShopType() == TYPE_PLAYER_SHOP_PET)
	{//宠物店
		pHuman->SetShopGuid(1,pPlayerShop->GetShopGUID());
	}

	//通知客户端购买商店成功
	GCPlayerShopError Msg;
	Msg.SetID(PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_SELL);
	pHuman->GetPlayer()->SendPacket(&Msg);

	//标志此店已经售出
	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopSaleOutHandler::ObjName=%s"
		,pHuman->GetName());

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
