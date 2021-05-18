
#include "stdafx.h"
#include "GCExchangeError.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "TDException.h"


uint GCExchangeErrorHandler::Execute( GCExchangeError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			switch(pPacket->GetID())
			{
			//不用强行取消本次交易的错误
			case EXCHANGE_MSG::ERR_ALREADY_LOCKED:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你已上锁，请重新操作");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_EXROOM:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"超过可交易物品的数量上限，请重新操作");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			//必须取消本次交易的错误
			case EXCHANGE_MSG::ERR_SELF_IN_EXCHANGE:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"自己正在交易中");
					break;
				}
			case EXCHANGE_MSG::ERR_TARGET_IN_EXCHANGE:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"对方正在交易中");
					break;
				}
			case EXCHANGE_MSG::ERR_DROP:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"对方取消交易");
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_OTHER:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"背包空间不够");
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_SELF:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"背包空间不够");
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_OTHER:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不足，请重新操作");
					break;
				}
			case EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_SELF:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不足，请重新操作");
					break;
				}
			case EXCHANGE_MSG::ERR_TOO_FAR:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"超过能够交易的范围");
					break;
				}

			case EXCHANGE_MSG::ERR_REFUSE_TRADE:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"对方拒绝了你的交易请求");
					break;
				}
			case EXCHANGE_MSG::ERR_PET_LEVEL_TOO_HIGH:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"宠物级别过高,无法交易");
					break;
				}
			case EXCHANGE_MSG::ERR_ILLEGAL:
			default:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"非法状态，交易取消");
					break;
				}
			}

			//需要解除背包对应物品的锁定(注意，克隆的物品GUID相同，所以可以这样操作)
			for(INT i=0; i<EXCHANGE_BOX_SIZE; i++)
			{
				if(CDataPool::GetMe()->MyExBox_GetItem(i))
				{
					__int64 ItemGUID = CDataPool::GetMe()->MyExBox_GetItem(i)->GetGUID();

					tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->UserBag_GetItemIndexByGUID(ItemGUID));
					if(pBagItem)
					{
						//去除锁定
						pBagItem->SetLock(FALSE);
					}
				}
			}

			//一切清空
			CDataPool::GetMe()->MyExBox_Clear();
			CDataPool::GetMe()->OtExBox_Clear();

			//更新Action
			CActionSystem::GetMe()->UserExchangeOther_Update();
			CActionSystem::GetMe()->UserExchangeSelf_Update();


			//更新背包物品（如果可能还需要更新装备栏物品，这个要看装备栏上的东西是不是可以用来交易）
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

			//关闭交易界面
			CEventSystem::GetMe()->PushEvent(GE_SUCCEED_EXCHANGE_CLOSE);
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
