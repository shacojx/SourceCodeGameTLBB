/*
服务器通知客户端关闭摊位
*/
#include "stdafx.h"
#include "GCStallClose.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "TDException.h"


uint GCStallCloseHandler::Execute( GCStallClose* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			//取消背包中对应物品的锁定状态
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


			//清空摊位
			CDataPool::GetMe()->MyStallBox_Clear();

			//清空消息
			CDataPool::GetMe()->MyStallMsg_ClearUp();

			//更新ui，关闭StallMessage界面
			CEventSystem::GetMe()->PushEvent(GE_CLOSE_STALL_MESSAGE);
			//ui刷新
			CActionSystem::GetMe()->UserStallSale_Update();

			//刷新背包界面
			CActionSystem::GetMe()->UserBag_Update();
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);


		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
