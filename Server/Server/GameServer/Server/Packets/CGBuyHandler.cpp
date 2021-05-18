#include "stdafx.h"

// CGBuyHandler.cpp

#include "CGBuy.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Human.h"
#include "GCNotifyEquip.h"
#include "Log.h"
#include "GCChat.h"
#include "GCBuy.h"

uint CGBuyHandler::Execute( CGBuy* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}
	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	DWORD	m_idTable	=	pPacket->GetBuyTableID();		//资源id
	BYTE	m_byNumber	=	pPacket->GetBuyNum();		//数量


	if(pHuman->GetCurrentShopId( ) == -1)
	{//没有打开交易窗口就买
		Assert(0);
		return PACKET_EXE_CONTINUE ;
	}

	_SHOP* pShop = pScene->GetShopManager()->GetShopByID(pHuman->GetCurrentShopId());
	int i;
	for(i = 0;i<pShop->m_ItemList->m_ListCount;i++)
	{
		if(pShop->m_ItemList->m_ListTypeIndex[i] == m_idTable)
			break;
	}

	if(i == pShop->m_ItemList->m_ListCount)
	{
		Assert(0);
		return PACKET_EXE_CONTINUE ;
	}

	//计算价格
    uint BaseMoney = ShopManager::ConvertItemType2Money(pShop->m_ItemList->m_ListType[i]);
	BaseMoney *= static_cast<uint>(pShop->m_scale);
	//
	BaseMoney = 0;
	if( pHuman->GetMoney() < BaseMoney )
	{//金钱不够
		GCBuy Msg ;
		Msg.SetBuyOk((BYTE)GCBuy::BUY_FAIL);
		pHuman->GetPlayer()->SendPacket( &Msg ) ;
		return PACKET_EXE_CONTINUE ;
	}

	_ITEM item = pScene->GetItemBoxManager()->CreateItem(pShop->m_ItemList->m_ListType[i], 1);
	uint BagIndex;
	if(pHuman->ReceiveItem(&item,BagIndex) == TRUE)
	{
		pHuman->SetMoney( pHuman->GetMoney() - BaseMoney);
		GCBuy Msg ;
		Msg.SetBuyOk((BYTE)GCBuy::BUY_OK);
		pHuman->GetPlayer()->SendPacket( &Msg ) ;
	}

	//通知
	GCNotifyEquip Msg ;
	Msg.SetBagIndex( BagIndex ) ;
	Msg.SetItem( &item ) ;

	pGamePlayer->SendPacket( &Msg ) ;

	g_pLog->FastSaveLog( LOG_FILE_1, "CGBuyHandler m_idTable=%d m_byNumber=%d",
		m_idTable, m_byNumber  ) ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
