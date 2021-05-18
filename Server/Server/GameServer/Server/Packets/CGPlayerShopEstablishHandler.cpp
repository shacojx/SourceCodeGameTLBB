#include "stdafx.h"

#include "CGPlayerShopEstablish.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "PacketFactoryManager.h"
#include "GCPlayerShopEstablish.h"
#include "TimeManager.h"//g_pTimeManager
#include "GWChat.h"
#include "ServerManager.h"

UINT CGPlayerShopEstablishHandler::Execute( CGPlayerShopEstablish* pPacket, Player* pPlayer )
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

	CHAR* pszShopName		=	pPacket->GetShopName();
	BYTE  nShopNameSize		=	pPacket->GetShopNameSize();
	BYTE  nShopType			=	pPacket->GetType();
	SceneID_t sceneid		=	pScene->SceneID();
	GCPlayerShopError	MsgError;

	PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
	pPlayerShopManager->ClampComFactor();
	FLOAT CommercialFactor = pPlayerShopManager->GetComFactor();
	UINT uCost = (UINT)(300000*CommercialFactor*2*1.03);

	//金钱验证
	if(uCost>pHuman->GetMoney())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopApplyHandler::Name=%s Cost = %d Money = %d ", pHuman->GetName(), uCost, pHuman->GetMoney() ) ;
		MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_MONEY_TO_NEW);
		pGamePlayer->SendPacket(&MsgError);
		return PACKET_EXE_CONTINUE ;
	}

	//需要再验证是不是已经建立满了商店了
	if(nShopType == PLAYERSHOP_MSG::TYPE_ITEM)
	{
		if( !pHuman->GetShopGuid(0).isNull() )
		{
			PlayerShop* pCurShop = pPlayerShopManager->GetPlayerShopByGUID(pHuman->GetShopGuid(0));
			if(!pCurShop)
			{
				pHuman->GetShopGuid(0).Reset();
			}
			else if(pCurShop->GetOwnerGuid() == pHuman->GetGUID())
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopApplyHandler::Name=%s Can not create shop", pHuman->GetName()) ;
				MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_ALREADY_EXIST);
				pGamePlayer->SendPacket(&MsgError);
				return PACKET_EXE_CONTINUE ;
			}
			else
			{
				pHuman->GetShopGuid(0).Reset();
			}
		}
	}
	else if(nShopType == PLAYERSHOP_MSG::TYPE_PET)
	{
		if( !pHuman->GetShopGuid(1).isNull() )
		{
			PlayerShop* pCurShop = pPlayerShopManager->GetPlayerShopByGUID(pHuman->GetShopGuid(1));
			if(!pCurShop)
			{
				pHuman->GetShopGuid(1).Reset();
			}
            else if(pCurShop->GetOwnerGuid() == pHuman->GetGUID())
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopApplyHandler::Name=%s Can not create shop", pHuman->GetName()) ;
				MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_ALREADY_EXIST);
				pGamePlayer->SendPacket(&MsgError);
				return PACKET_EXE_CONTINUE ;
			}
			else
			{
				pHuman->GetShopGuid(1).Reset();
			}
		}
	}
	else
	{
		Assert(0);
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopApplyHandler::Name=%s Can not create shop", pHuman->GetName(), uCost, pHuman->GetMoney() ) ;
		MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_ALREADY_EXIST);
		pGamePlayer->SendPacket(&MsgError);
		return PACKET_EXE_CONTINUE ;
	}
	//...

	//重名验证
	for(UINT i =0; i<MAX_SHOP_NUM_PER_SCENE; i++)
	{
		PlayerShop* pTempShop = pPlayerShopManager->GetPlayerShopByIndex(i);
		if(!pTempShop)
		{
			continue;
		}
		else
		{
			if(!strcmp(pszShopName, pTempShop->GetShopName()))
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopApplyHandler::Name = %s This name has already existed", pHuman->GetName()) ;
				MsgError.SetID(PLAYERSHOP_MSG::ERR_NAME_ALREADY_EXISTED);
				pGamePlayer->SendPacket(&MsgError);
				return PACKET_EXE_CONTINUE ;
			}
		}
	}

	//New一个商店
	INT nIndex	= pPlayerShopManager->NewPlayerShop();

	if(nIndex < 0)
	{
		MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_SHOP_IN_POOL);
		pGamePlayer->SendPacket(&MsgError);
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGPlayerShopEstablishHandler::Name=%s OUT OF SHOP POOL", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	PlayerShop*			pPlayerShop	= pPlayerShopManager->GetPlayerShopByIndex(nIndex);
	
	//时间采样
	UINT  nYear		=	(UINT)g_pTimeManager->GetYear();
	BYTE  nMonth	=	(BYTE)g_pTimeManager->GetMonth();
	BYTE  nDay		=	(BYTE)g_pTimeManager->GetDay();
	BYTE  nHour		=	(BYTE)g_pTimeManager->GetHour();
	BYTE  nMin		=	(BYTE)g_pTimeManager->GetMinute();

	//开张了
	pPlayerShop->SetShopStatus(STATUS_PLAYER_SHOP_OPEN);
	//商店类型
	if(nShopType == 1)
	{//物品点
		pPlayerShop->SetShopType(TYPE_PLAYER_SHOP_ITEM);
		pHuman->SetShopGuid(0,pPlayerShop->GetShopGUID());
	}
	else
	{//宠物店
		pPlayerShop->SetShopType(TYPE_PLAYER_SHOP_PET);
		pHuman->SetShopGuid(1,pPlayerShop->GetShopGUID());
	}
	//冲入本金
	pPlayerShop->SetBaseMoney(uCost/2);
	//本金上限
	pPlayerShop->SetMaxBaseMoney(uCost/2);
	//冲入投资金
	pPlayerShop->SetProfitMoney(uCost/2);
	//商店名
	pPlayerShop->SetShopName(pszShopName, nShopNameSize);
	//商店描述
	pPlayerShop->SetShopDesc("",0);
	//店主名
	pPlayerShop->SetOwnerName(pHuman->GetName());
	//店主GUID
	pPlayerShop->SetOwnerGuid(pHuman->GetGUID());
	//商店的建立时间
	pPlayerShop->SetFoundedYear(nYear);
	pPlayerShop->SetFoundedMonth(nMonth);
	pPlayerShop->SetFoundedDay(nDay);
	pPlayerShop->SetFoundedHour(nHour);
	pPlayerShop->SetFoundedMin(nMin);
	//设置开放的柜台数
	pPlayerShop->SetNumStallOpened(1);
	//设置开张的柜台数
	pPlayerShop->SetNumStallOnSale(0);

	//柜台开张
	PlayerStallBox*		pNewPlayerStallBox	= pPlayerShop->GetPlayerStallBoxByIndex(0);
	pNewPlayerStallBox->SetStallStatus(PLAYER_SHOP::STALL_CLOSE);

	//扣钱
	pHuman->SetMoney(pHuman->GetMoney()-uCost);

	MONEY_LOG_PARAM	MoneyLogParam;
	MoneyLogParam.CharGUID	=	pHuman->GetGUID();
	MoneyLogParam.TargetGUID	=	pPlayerShop->GetOwnerGuid();
	MoneyLogParam.OPType	=	MONEY_CREATE_PLAYERSHOP_OUTCOME;	
	MoneyLogParam.Count		=	uCost;
	MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
	MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
	MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
	SaveMoneyLog(&MoneyLogParam);

	//"AAA（店铺唯一ID）开张大吉，大掌柜BBB(玩家自身名字)恭迎惠顾。" 
	CHAR	szMsgChat[256] = {0};
	sprintf(szMsgChat, "@*;SrvMsg;CHAT_PS_OPEN;%s;%s", pszShopName, pHuman->GetName());
	GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;
	pChatPacket->SetSourGUID(pHuman->GetGUID());
	pChatPacket->SetChatType( CHAT_TYPE_SYSTEM ) ;
	pChatPacket->SetContexSize( (BYTE)strlen(szMsgChat)) ;
	pChatPacket->SetContex( szMsgChat ) ;

	g_pServerManager->SendPacket( pChatPacket, INVALID_ID );

	CHAR	szMsgTitle[256] = {0};
	sprintf(szMsgTitle, "%s大掌柜", pszShopName);
	pHuman->SetShangDianName(szMsgTitle,(BYTE)strlen(szMsgTitle));
	pHuman->UpdateTitlesToClient();

	GCPlayerShopEstablish Msg;
	Msg.SetShopName(pPlayerShop->GetShopName());
	pGamePlayer->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopEstablishHandler::Name=%s"
		,pHuman->GetName()) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
