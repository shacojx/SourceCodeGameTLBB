#include "StdAfx.h"
#include "GCPrescription.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\DataPool\GMDP_CharacterData.h"


uint GCPrescriptionHandler::Execute( GCPrescription* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//加入UI数据池
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pDataPool);

		SCommand_DPC cmdTemp;

		//--------------------------------------------------------------
		//生活技能配方表刷新
/*
		cmdTemp.m_wID			= DPC_UPDATE_LIFE_PRESCR;
		cmdTemp.m_adwParam[0]	= 2; // update a specified recipe
		cmdTemp.m_adwParam[1]	= pPacket->getPrescription();
		cmdTemp.m_abParam[2]	= pPacket->getLearnOrAbandon();
		pDataPool->OnCommand_( &cmdTemp );
*/
		CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Set_Prescr(
			pPacket->getPrescription(), pPacket->getLearnOrAbandon());

	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
