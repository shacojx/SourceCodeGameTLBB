/*
接到服务器通知,显示确定按钮
*/
#include "stdafx.h"
#include "GCExchangeSynchConfirmII.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "TDException.h"

uint GCExchangeSynchConfirmIIHandler::Execute( GCExchangeSynchConfirmII* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{//初始化双方交易盒，打开界面
			BYTE IsEnable = pPacket->GetIsEnable();

			if(IsEnable)
			{
				CDataPool::GetMe()->MyExBox_SetConfirm(TRUE);
			}
			else
			{
				CDataPool::GetMe()->MyExBox_SetConfirm(FALSE);
			}
			//刷新界面
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
