#include "StdAfx.h"
#include "GCShopMerchandiseList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerOther.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"

uint GCShopMerchandiseListHandler::Execute( GCShopMerchandiseList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//清空原有商品列表
		CDataPool::GetMe()->Booth_Clear();

		//清空原有商品列表
		CDataPool::GetMe()->Booth_Sold_Clear();

		//商品个数
		INT nNum = pPacket->GetMerchadiseNum();

		//添加到数据池中
		for(INT i=0; i<nNum; i++)
		{
			//创建商品实例
			tObject_Item* pItem = CObject_Item::NewItem( pPacket->GetMerchadiseList()[i].idTable);
			if(!pItem) continue;

			//设置该物品在货架上的位置
			pItem->SetPosIndex(i);
			//设置该物品的数量（每一组的数量）
			pItem->SetNumber(pPacket->GetMerchadiseList()[i].byNumber);
			//设置有限商品的最大数量（显示用）
			pItem->SetMax(pPacket->GetMerchadiseList()[i].MaxNumber);
			//价格放入数据池
			CDataPool::GetMe()->Booth_SetItemPrice(i, pPacket->GetMerchadiseList()[i].nPrice);
			//加入数据池
			CDataPool::GetMe()->Booth_SetItem(i, pItem);
		}
		//设置数量
		CDataPool::GetMe()->Booth_SetNumber(nNum);
		CDataPool::GetMe()->Booth_SetBuyType(pPacket->GetBuyType());
		CDataPool::GetMe()->Booth_SetRepairType(pPacket->GetRepairType());
		CDataPool::GetMe()->Booth_SetRepairLevel(pPacket->GetRepairLevel());
		CDataPool::GetMe()->Booth_SetRepairSpend(pPacket->GetRepairSpend());
		CDataPool::GetMe()->Booth_SetRepairOkProb(pPacket->GetRepairOkProb());
		CDataPool::GetMe()->Booth_SetBuyLevel(pPacket->GetBuyLevel());
		CDataPool::GetMe()->Booth_SetScale(pPacket->GetScale());
		CDataPool::GetMe()->Booth_SetCallBack(pPacket->GetCanBuyBack());
		CDataPool::GetMe()->Booth_SetCurrencyUnit(pPacket->GetCurrencyUnit());
		CDataPool::GetMe()->Booth_SetSerialNum(pPacket->GetSerialNum());
		CDataPool::GetMe()->Booth_SetbCanBuyMult(pPacket->GetBuyMulti());
		CDataPool::GetMe()->Booth_SetShopType(pPacket->GetShopType());

		//更新到ActionSystem
		if(nNum > 0) CActionSystem::GetMe()->Booth_Update();

		//通知UI
		CDataPool::GetMe()->Booth_Open();

		//得到商人Obj
		INT nID = -1;

		tObject* pObj = CObjectManager::GetMe()->FindServerObject(pPacket->GetObjID());
		if(pObj) 
		{
			nID = pObj->GetID();
			CUIDataPool::GetMe()->SetCurShopNpcId( pObj->GetServerID() );
		}
		else
		{
			nID = -1;
		}

		CDataPool::GetMe()->Booth_SetShopNpcId(nID);
		CDataPool::GetMe()->Booth_SetShopUniqueId(pPacket->GetUniqueID());

		CEventSystem::GetMe()->PushEvent(GE_OPEN_BOOTH);//, nID);
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
