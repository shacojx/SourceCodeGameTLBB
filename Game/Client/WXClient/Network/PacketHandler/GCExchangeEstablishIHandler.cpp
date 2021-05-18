/*
收到此消息,交易连接已经建立,双方打开交易盒界面
*/
#include "stdafx.h"
#include "GCExchangeEstablishI.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"


uint GCExchangeEstablishIHandler::Execute( GCExchangeEstablishI* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{//初始化双方交易盒，打开界面
			CDataPool::GetMe()->MyExBox_Clear();
			CDataPool::GetMe()->OtExBox_Clear(FALSE);

			if(CObjectManager::GetMe()->FindServerObject(pPacket->GetObjID()) == NULL)
			{
				return PACKET_EXE_CONTINUE ;				
			}
			INT nObjId = CObjectManager::GetMe()->FindServerObject(pPacket->GetObjID())->GetID();

			//设置当前与本人交易的玩家ID
			CDataPool::GetMe()->SetExchangObjID(pPacket->GetObjID());

			//如果这个时候制造物品的界面开着，那么需要强行关闭，不能同时进行这两个操作
			if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Synthesize"))
			{
				CEventSystem::GetMe()->PushEvent(GE_CLOSE_SKILL_BOOK);
			}

			//打开界面
			CEventSystem::GetMe()->PushEvent(GE_OPEN_EXCHANGE_FRAME,nObjId);

			//更新
			CActionSystem::GetMe()->UserExchangeSelf_Update();
			CActionSystem::GetMe()->UserExchangeOther_Update();

		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
