// GCMissionResultHandler.cpp
#include "stdafx.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\event\GMEventSystem.h"
#include "GCMissionResult.h"

using namespace Packets;
uint GCMissionResultHandler::Execute( GCMissionResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pDataPool);

		if(pPacket->getIsFinished())		//任务完成
		{
			// 关闭提交任务物品的界面
			CEventSystem::GetMe()->PushEvent(GE_QUEST_AFTER_CONTINUE);
	
			// 打开接收奖励的界面
			
		}
		else								//没有完成任务
		{
			// 答应一个提示信息，不关闭提交任务物品的界面，这样可以让玩家检查是不是自己放错了物品
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你给我的物品不是我需要的!");
		}
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
