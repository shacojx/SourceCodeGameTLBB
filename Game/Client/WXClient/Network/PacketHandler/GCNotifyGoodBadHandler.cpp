
#include "StdAfx.h"

#include "..\..\Procedure\GameProcedure.h"

#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"

#include "GCNotifyGoodBad.h"

uint GCNotifyRMBMoneyHandler::Execute( GCNotifyRMBMoney* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CHAR szText[256];
		_snprintf(szText, 256, "#Y你获得了%d个元宝", pPacket->GetValue());
		ADDTALKMSG(szText);

		CCharacterData* pCharacterData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
		pCharacterData->Set_RMB(pCharacterData->Get_RMB()+pPacket->GetValue());
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
