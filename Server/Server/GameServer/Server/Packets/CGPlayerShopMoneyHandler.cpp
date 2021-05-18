#include "stdafx.h"

#include "CGPlayerShopMoney.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCPlayerShopMoney.h"
#include "GCPlayerShopError.h"
#include "RecordOpt.h"

UINT CGPlayerShopMoneyHandler::Execute( CGPlayerShopMoney* pPacket, Player* pPlayer )
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
	BYTE				nOpt	=	pPacket->GetOpt();				//操作
	BYTE				nType	=	pPacket->GetType();				//存到哪
	UINT				uAmount	=	pPacket->GetAmount();			//数量
	BYTE			nShopSerial =	pPacket->GetShopSerial();		//商店序列号

	GCPlayerShopError MsgError;

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

	if(bIsMine == FALSE && bCanManager == FALSE)
	{
		//log...
		return PACKET_EXE_CONTINUE;
	}

	if(nShopSerial != pPlayerShop->GetSerial())
	{
		MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
		pGamePlayer->SendPacket(&MsgError);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	GCPlayerShopMoney	MsgToClient;
	MsgToClient.SetShopID(nShopID);
	switch(nOpt)
	{
		case CGPlayerShopMoney::OPT_SAVE_MONEY:
			{//存钱

				if(pHuman->GetMoney() < uAmount)
				{
					Assert(0);
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGPlayerShopMoneyHandler::Name=%s, Out Of Money In Packet"
						,pHuman->GetName()) ;

					return PACKET_EXE_CONTINUE ;
				}

				if(nType == CGPlayerShopMoney::TYPE_BASE_MONEY)
				{
					UINT uBaseMoney = pPlayerShop->GetBaseMoney();
					if(uBaseMoney+uAmount > 10000000)
					{//超过1000金
						Assert(0);
						g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGPlayerShopMoneyHandler::Name=%s, uBaseMoney+uAmount > 10000000"
							,pHuman->GetName()) ;
						return PACKET_EXE_CONTINUE ;
					}
					UINT	uRealBaseMoney =  (UINT)(uBaseMoney+(FLOAT)uAmount*(1-0.03));
					pPlayerShop->SetBaseMoney(uRealBaseMoney);

					//充入本金：AAA往店铺本金中充入（换行）？金？银？铜（AAA为操作者名）
					RecordOpt::Excute(REC_INPUT_BASE, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)(uRealBaseMoney - uBaseMoney));
					
					MsgToClient.SetType(GCPlayerShopMoney::TYPE_BASE_MONEY);
					MsgToClient.SetAmount(uRealBaseMoney);
					MsgToClient.SetShopSerial(pPlayerShop->IncSerial());
					pGamePlayer->SendPacket(&MsgToClient);
				}
				else if(nType == CGPlayerShopMoney::TYPE_PROFIT_MONEY)
				{
					UINT uProfitMoney = pPlayerShop->GetProfitMoney();
					UINT uRealProfitMoney =  (UINT)(uProfitMoney+(FLOAT)uAmount*(1-0.03));
					pPlayerShop->SetProfitMoney(uRealProfitMoney);
					MsgToClient.SetType(GCPlayerShopMoney::TYPE_PROFIT_MONEY);
					MsgToClient.SetAmount(uRealProfitMoney);
					MsgToClient.SetShopSerial(pPlayerShop->IncSerial());

					//充入赢利资金：AAA往店铺赢利资金中充入（换行）？金？银？铜（AAA为操作者名）
					RecordOpt::Excute(REC_INPUT_PROFIT, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)(uRealProfitMoney - uProfitMoney));
					
					pGamePlayer->SendPacket(&MsgToClient);
				}

				pHuman->SetMoney(pHuman->GetMoney() - uAmount);

				MONEY_LOG_PARAM	MoneyLogParam;
				MoneyLogParam.CharGUID	=	pHuman->GetGUID();
				MoneyLogParam.TargetGUID	=	pPlayerShop->GetOwnerGuid();
				MoneyLogParam.OPType	=	MONEY_PLAYERSHOP_SAVE;	
				MoneyLogParam.Count		=	uAmount;
				MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);
			}
			break;
		case CGPlayerShopMoney::OPT_GET_MONEY:
			{//取钱
				if(nType == CGPlayerShopMoney::TYPE_BASE_MONEY)
				{
					UINT uBaseMoney = pPlayerShop->GetBaseMoney();
					if(uBaseMoney < uAmount)
					{
						Assert(0);
						g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGPlayerShopMoneyHandler::Name=%s, Out Of Money In ShopBase"
							,pHuman->GetName()) ;

						return PACKET_EXE_CONTINUE ;
					}
					pPlayerShop->SetBaseMoney(uBaseMoney-uAmount);
					MsgToClient.SetType(GCPlayerShopMoney::TYPE_BASE_MONEY);
					MsgToClient.SetAmount(uBaseMoney-uAmount);
					MsgToClient.SetShopSerial(pPlayerShop->IncSerial());
					pGamePlayer->SendPacket(&MsgToClient);
				}
				else if(nType == CGPlayerShopMoney::TYPE_PROFIT_MONEY)
				{
					UINT uProfitMoney = pPlayerShop->GetProfitMoney();
					if(uProfitMoney < uAmount)
					{
						Assert(0);
						g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGPlayerShopMoneyHandler::Name=%s, Out Of Money In ShopProfit"
							,pHuman->GetName()) ;
						return PACKET_EXE_CONTINUE ;
					}
					pPlayerShop->SetProfitMoney(uProfitMoney-uAmount);
					MsgToClient.SetType(GCPlayerShopMoney::TYPE_PROFIT_MONEY);
					MsgToClient.SetAmount(uProfitMoney-uAmount);
					MsgToClient.SetShopSerial(pPlayerShop->IncSerial());
					pGamePlayer->SendPacket(&MsgToClient);
				}
				pHuman->SetMoney(pHuman->GetMoney() + uAmount);

				MONEY_LOG_PARAM	MoneyLogParam;
				MoneyLogParam.CharGUID	=	pHuman->GetGUID();
				MoneyLogParam.TargetGUID	=	pPlayerShop->GetOwnerGuid();
				MoneyLogParam.OPType	=	MONEY_PLAYERSHOP_WITHDRAW;	
				MoneyLogParam.Count		=	uAmount;
				MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);
			}
			break;
		default:
			break;
	}
	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopMoneyHandler::Name=%s"
		,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
