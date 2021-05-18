#include"stdafx.h"

#include "GCBankMoney.h"
#include "CGBankMoney.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "TDException.h"
uint GCBankMoneyHandler::Execute( GCBankMoney* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BYTE IsSave = pPacket->GetSaveType();
			INT	 Amount = pPacket->GetAmount();
			INT  AmountRMB = pPacket->GetAmountRMB();
			INT CurMoney = CDataPool::GetMe()->UserBank_GetBankMoney();
			INT CurRMB = CDataPool::GetMe()->UserBank_GetBankRMB();

			CHAR szMsg[256];
			if(IsSave & CGBankMoney::SAVE_MONEY)
			{
				CurMoney += Amount;
				//通知玩家交易成功

				_snprintf(szMsg, 256, "存入金钱成功");
				CDataPool::GetMe()->UserBank_SetBankMoney(CurMoney);
				//update 界面
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK );
				//提示信息
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE,szMsg	);

			}
			else if(IsSave & CGBankMoney::PUTOUT_MONEY)
			{
				CurMoney -= Amount;
				//通知玩家交易成功		

				_snprintf(szMsg, 256, "取出金钱成功");

				CDataPool::GetMe()->UserBank_SetBankMoney(CurMoney);
				//update 界面
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK );
				//提示信息
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE,szMsg	);

			}
			else if(IsSave & CGBankMoney::UPDATE_MONEY)
			{//进入场景时由服务器刷过来，银行中的金钱数
				CurMoney = Amount;

				CDataPool::GetMe()->UserBank_SetBankMoney(CurMoney);
				//update 界面
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK );
			}

			if( IsSave & CGBankMoney::SAVE_RMB )
			{
				CurRMB += AmountRMB;
				//通知玩家交易成功

				_snprintf(szMsg, 256, "存入元宝成功");
				CDataPool::GetMe()->UserBank_SetBankRMB(CurRMB);
				//update 界面
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK );
				//提示信息
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE,szMsg	);
			}
			else if( IsSave & CGBankMoney::PUTOUT_RMB )
			{
				CurRMB -= AmountRMB;
				//通知玩家交易成功		

				_snprintf(szMsg, 256, "取出元宝成功");

				CDataPool::GetMe()->UserBank_SetBankRMB(CurRMB);
				//update 界面
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK );
				//提示信息
				CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE,szMsg	);
			}
			else if( IsSave & CGBankMoney::UPDATA_RMB )
			{
				CurRMB = AmountRMB;

				CDataPool::GetMe()->UserBank_SetBankRMB(CurRMB);
				//update 界面
				CEventSystem::GetMe()->PushEvent( GE_UPDATE_BANK );
			}

		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
