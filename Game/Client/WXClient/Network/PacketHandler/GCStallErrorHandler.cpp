/*
服务器发送给客户端的错误消息
*/

#include "stdafx.h"
#include "GCStallError.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"

#include "TDException.h"

uint GCStallErrorHandler::Execute( GCStallError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			switch(pPacket->GetID())
			{
				//不用强行取消本次交易的错误
			case STALL_MSG::ERR_OWNER_INVALID:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"目标人物不存在");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NEED_NEW_COPY:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"摊位信息已改变,请重新打开");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NOT_ENOUGH_ROOM:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"空间不足");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NOT_ENOUGH_MONEY:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不足");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"摊位已满");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_CLOSE:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"摊主已收摊");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_ALREADY_LOCK:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"不能出售此物品");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case STALL_MSG::ERR_PET_LEVEL_TOO_HIGH:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"宠物级别过高,无法购买");
					return PACKET_EXE_CONTINUE ;
					break;
				}

				//必须取消本次交易的错误
			case STALL_MSG::ERR_NOT_ENOUGH_MONEY_TO_OPEN:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不足，无法摆摊");
					break;
				}
			case STALL_MSG::ERR_ILLEGAL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"非法消息");
					return PACKET_EXE_CONTINUE ;
					break;
				}
			default:
				{
				
				}
				break;
			}
			
			//解除货架上物品的锁定
			//需要解除背包对应物品的锁定(注意，克隆的物品GUID相同，所以可以这样操作)
			for(INT i=0; i<STALL_BOX_SIZE; i++)
			{
				if(CDataPool::GetMe()->MyStallBox_GetItem(i))
				{
					__int64 ItemGUID = CDataPool::GetMe()->MyStallBox_GetItem(i)->GetGUID();

					tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->UserBag_GetItemIndexByGUID(ItemGUID));
					if(pBagItem)
					{
						//去除锁定
						pBagItem->SetLock(FALSE);
					}
				}
			}


			//清空目标摊位
			CDataPool::GetMe()->OtStallBox_Clear();
			CDataPool::GetMe()->MyStallBox_Clear();

			//ui刷新
			CActionSystem::GetMe()->UserStallSale_Update();
			CActionSystem::GetMe()->UserStallBuy_Update();
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
