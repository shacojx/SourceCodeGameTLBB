
#include "stdafx.h"
#include "GCStallItemPrice.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"

uint GCStallItemPriceHandler::Execute( GCStallItemPrice* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			_ITEM_GUID		ItemGuid	=	pPacket->GetObjGUID();
			PET_GUID_t		PetGuid		=	pPacket->GetPetGUID();
			UINT			nPrice		=	pPacket->GetPrice();
			UINT			nSerial		=	pPacket->GetSerial();

			if(PetGuid.IsNull())
			{//物品价格的设定
				//转换GUID
				tObject_Item::ITEM_GUID tempitemGuid;
				tempitemGuid.m_idOrg.m_idWorld	=	ItemGuid.m_World;
				tempitemGuid.m_idOrg.m_idServer =	ItemGuid.m_Server;
				tempitemGuid.m_idOrg.m_uSerial	=	ItemGuid.m_Serial;

				//背包中的位置
				INT	unIndexInStall = CDataPool::GetMe()->MyStallBox_GetItemIndexByGUID(tempitemGuid.m_idUnion);
				if(unIndexInStall<0)
					return PACKET_EXE_CONTINUE;

				CDataPool::GetMe()->MyStallBox_SetItemPrice(unIndexInStall,nPrice);
				CDataPool::GetMe()->MyStallBox_SetItemSerial(unIndexInStall,nSerial);

				//更改物品价格
				CActionSystem::GetMe()->UserStallSale_Update();
				//通知界面
				CEventSystem::GetMe()->PushEvent(GE_STALL_SALE_SELECT,(INT)unIndexInStall);
			}
			else
			{//宠物价格的设定
				INT	IndexInStall = CDataPool::GetMe()->MyStallBox_GetPetByGuid(PetGuid);
				if(IndexInStall<0)
					return PACKET_EXE_CONTINUE;

				CDataPool::GetMe()->MyStallBox_SetPetPrice(IndexInStall, nPrice);
				CDataPool::GetMe()->MyStallBox_SetPetSerial(IndexInStall, nSerial);
				//刷新界面

				CEventSystem::GetMe()->PushEvent(GE_STALL_SALE_SELECT,(INT)IndexInStall);
			}
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
