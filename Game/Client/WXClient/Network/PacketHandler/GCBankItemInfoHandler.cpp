
#include "StdAfx.h"
#include "GCBankItemInfo.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Event\GMEventSystem.h"

uint GCBankItemInfoHandler::Execute( GCBankItemInfo* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CDataPool::GetMe()->UserBank_SetItemExtraInfo(pPacket->getBankIndex(),pPacket->getIsNull(),pPacket->getItem());

		CActionSystem::GetMe()->UserBank_Update();
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}