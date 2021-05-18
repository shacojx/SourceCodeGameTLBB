/*
根据服务器指令同步自己的锁变化
*/
#include "stdafx.h"
#include "GCExchangeSynchLock.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"

uint GCExchangeSynchLockHandler::Execute( GCExchangeSynchLock* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{//
			BYTE IsBoth		= pPacket->GetIsBoth();
			BYTE IsMyself	= pPacket->GetIsMyself();
			BYTE IsLock		= pPacket->GetIsLocked();

			if(IsBoth)
			{
				if(IsLock)
				{
					CDataPool::GetMe()->MyExBox_SetLock(TRUE);
					CDataPool::GetMe()->OtExBox_SetLock(TRUE);
				}
				else
				{
					CDataPool::GetMe()->MyExBox_SetLock(FALSE);
					CDataPool::GetMe()->OtExBox_SetLock(FALSE);
				}
			}
			else if(IsMyself)
			{
				if(IsLock)
				{
					CDataPool::GetMe()->MyExBox_SetLock(TRUE);
				}
				else
				{
					CDataPool::GetMe()->MyExBox_SetLock(FALSE);
				}
			}
			else
			{
				if(IsLock)
				{
					CDataPool::GetMe()->OtExBox_SetLock(TRUE);
				}
				else
				{
					CDataPool::GetMe()->OtExBox_SetLock(FALSE);
				}
			}
			//刷新界面
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_EXCHANGE);
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
