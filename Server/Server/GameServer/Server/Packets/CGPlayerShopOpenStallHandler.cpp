#include "stdafx.h"

#include "CGPlayerShopOpenStall.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCPlayerShopOpenStall.h"
#include "GCPlayerShopError.h"
#include "RecordOpt.h"

using namespace PLAYER_SHOP;
UINT CGPlayerShopOpenStallHandler::Execute( CGPlayerShopOpenStall* pPacket, Player* pPlayer )
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
	_PLAYERSHOP_GUID	nShopID			=	pPacket->GetShopID();			//商店ID
	BYTE				nStallIndex		=	pPacket->GetStallIndex();		//柜台索引
	BYTE				bOpen			=	pPacket->GetOpen();				//是否开张
	BYTE				nShopSerial		=	pPacket->GetShopSerial();
	GCPlayerShopError MsgError;
	PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
	PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
	Assert(pPlayerShop);
	if(pPlayerShop->GetShopStatus() == STATUS_PLAYER_SHOP_ON_SALE)
	{//商店未开张
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopOpenStallHandler::Name=%s shop close"
			,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;
	}

	//是不是自己的店
	BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

	//是不是自己可以管理的店
	BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

	if(bIsMine == FALSE)
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

	PlayerStallBox* pPlayerStallBox = pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
	BOOL	bRefresh = FALSE;
	if(pPlayerStallBox->GetStallStatus() == STALL_CLOSE && bOpen)
	{
		pPlayerStallBox->SetStallStatus(STALL_OPEN);
		BYTE nStallOnSale = pPlayerShop->GetNumStallOnSale();
		pPlayerShop->SetNumStallOnSale(nStallOnSale+1);
		bRefresh = TRUE;
		//开张：AAA开张了第N间柜台（AAA为操作者名，N为柜台所属间数）
		RecordOpt::Excute(REC_OPEN, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)nStallIndex);
		
	}
	else if(pPlayerStallBox->GetStallStatus() == STALL_OPEN && !bOpen)
	{
		pPlayerStallBox->SetStallStatus(STALL_CLOSE);
		BYTE nStallOnSale = pPlayerShop->GetNumStallOnSale();
		pPlayerShop->SetNumStallOnSale(nStallOnSale-1);
		bRefresh = TRUE;
		//打烊：AAA打烊了第N间柜台（AAA为操作者名，N为柜台所属间数）
		RecordOpt::Excute(REC_CLOSE, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)nStallIndex);
	}

	if(bRefresh)
	{
		GCPlayerShopOpenStall MsgToClient;
		MsgToClient.SetShopID(nShopID);
		MsgToClient.SetStallIndex(nStallIndex);
		MsgToClient.SetOpen(bOpen);
		MsgToClient.SetShopSerial(pPlayerShop->IncSerial());
		pGamePlayer->SendPacket(&MsgToClient);
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopOpenStallHandler::Name=%s"
		,pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
