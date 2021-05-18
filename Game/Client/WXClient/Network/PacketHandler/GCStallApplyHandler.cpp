/*
通知客户端确认，价格，税收。
*/
#include "stdafx.h"
#include "GCStallApply.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "TDException.h"

uint GCStallApplyHandler::Execute( GCStallApply* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{

		BYTE	IsCanStall	=	pPacket->GetIsCanStall();
		UINT	PosTax		=	pPacket->GetPosTax();
		BYTE	TradeTax	=	pPacket->GetTradeTax();

		if(IsCanStall == TRUE)
		{
			//显示用户确定对话框
			CDataPool::GetMe()->MyStallBox_SetPosTax(PosTax);
			CDataPool::GetMe()->MyStallBox_SetTradeTax(TradeTax);

			CEventSystem::GetMe()->PushEvent(GE_OPEN_STALL_RENT_FRAME);
		}
		else
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"不满足摆摊要求，摆摊请求失败");
		}
	}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
