/*
通知客户端删掉此摊位中的某个物品
*/

#include "stdafx.h"
#include "GCStallRemoveItem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"

#include "TDException.h"

uint GCStallRemoveItemHandler::Execute( GCStallRemoveItem* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			_ITEM_GUID	ItemGuid	=	pPacket->GetObjGUID();
			PET_GUID_t	PetGuid		=	pPacket->GetPetGUID();
			UINT		Serial		=	pPacket->GetSerial();
			BYTE		ToType		=	pPacket->GetToType();

			switch(ToType)
			{
			case STALL_MSG::POS_BAG:
				{
					//转换GUID
					tObject_Item::ITEM_GUID tempitemGuid;
					tempitemGuid.m_idOrg.m_idWorld	=	ItemGuid.m_World;
					tempitemGuid.m_idOrg.m_idServer =	ItemGuid.m_Server;
					tempitemGuid.m_idOrg.m_uSerial	=	ItemGuid.m_Serial;

					//摊位中的位置
					UINT	IndexInStall = CDataPool::GetMe()->MyStallBox_GetItemIndexByGUID(tempitemGuid.m_idUnion);
					if(IndexInStall<0)
						return PACKET_EXE_CONTINUE;
					
					//背包中的位置
					UINT	IndexInBag = CDataPool::GetMe()->UserBag_GetItemIndexByGUID(tempitemGuid.m_idUnion);
					if(IndexInBag<0)
						return PACKET_EXE_CONTINUE;

					//解除背包中的物品锁定
					if(CDataPool::GetMe()->UserBag_GetItem(IndexInBag))
					{
						CDataPool::GetMe()->UserBag_GetItem(IndexInBag)->SetLock(FALSE);
					}

					//删掉摊位中此位置上的东西
					CDataPool::GetMe()->MyStallBox_SetItem(IndexInStall, NULL);

					//此位置序列号改变
					CDataPool::GetMe()->MyStallBox_SetItemSerial(IndexInStall, Serial);

					//更新界面
					CActionSystem::GetMe()->UserStallSale_Update();
					//更新ActionSystem
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_STALL_SALE);
					//更新背包
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				}
				break;
			case STALL_MSG::POS_EQUIP:
				{
				}
				break;
			case STALL_MSG::POS_PET:
				{
					INT IndexInStallBox = CDataPool::GetMe()->MyStallBox_GetPetByGuid(PetGuid);
					CDataPool::GetMe()->MyStallBox_SetPet(IndexInStallBox, NULL, FALSE);
					CDataPool::GetMe()->MyStallBox_SetPetSerial(IndexInStallBox, Serial);
					INT IndexInBag = CDataPool::GetMe()->Pet_GetPetByGuid(PetGuid);
					//解锁这个位置的宠物
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_STALL_SALE);
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_PET_LIST);
				}
				break;
			default:
				break;
			}

		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
