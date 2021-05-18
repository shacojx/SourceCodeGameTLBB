// GCShopUpdateMerchandiseListHandler.cpp
#include "stdafx.h"
#include "GCShopUpdateMerchandiseList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerOther.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"

uint GCShopUpdateMerchandiseListHandler::Execute( GCShopUpdateMerchandiseList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			//已经关上了就算了
			if(CDataPool::GetMe()->Booth_IsClose())
				return PACKET_EXE_CONTINUE ;

			//更新的商品的个数
			INT nNum = pPacket->GetMerchadiseNum();
			//添加到数据池中
			for(INT i=0; i<nNum; i++)
			{
				//查找商品实例
				tObject_Item* pItem = CDataPool::GetMe()->Booth_GetItemByID(pPacket->GetMerchadiseList()[i].idTable);
				TDAssert(pItem);
	
				//设置新的数量
				INT	num = (pPacket->GetMerchadiseList()[i].byNumber<0)? 0:pPacket->GetMerchadiseList()[i].byNumber;
				pItem->SetMax(num);
			}
			//更新到ActionSystem
			if(nNum > 0) CActionSystem::GetMe()->Booth_Update();
			//更新到UI
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_BOOTH);
		}

		return PACKET_EXE_CONTINUE ;

		__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
