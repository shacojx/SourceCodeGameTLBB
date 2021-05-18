/*
收到此消息服务器端的交易盒数据已经被清空
*/

#include "stdafx.h"
#include "GCExchangeCancel.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\Item\Obj_Item.h"

#include "TDException.h"
uint GCExchangeCancelHandler::Execute( GCExchangeCancel* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
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

			CDataPool::GetMe()->MyExBox_Clear();
			CDataPool::GetMe()->OtExBox_Clear();

			//更新Action
			CActionSystem::GetMe()->UserExchangeOther_Update();
			CActionSystem::GetMe()->UserExchangeSelf_Update();
			CActionSystem::GetMe()->UserBag_Update();

			//更新背包
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
			//关闭界面
			CEventSystem::GetMe()->PushEvent(GE_SUCCEED_EXCHANGE_CLOSE);
			//交易已经被取消

			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"交易已经被取消");
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
