#include "StdAfx.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"


#include "GCMissionModify.h"



using namespace Packets;

uint GCMissionModifyHandler::Execute( GCMissionModify* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pDataPool);


		SCommand_DPC cmdTemp;
		cmdTemp.m_wID			= DPC_UPDATE_MISSION_MODIFY;

		cmdTemp.m_anParam[1]	= (INT)pPacket->GetFlag();

		if(pPacket->GetFlag() == GCMissionModify::MISSIONMODIFY_MISSION)
			cmdTemp.m_apParam[0]	= (VOID*)(pPacket->GetMission());
		else if(pPacket->GetFlag() == GCMissionModify::MISSIONMODIFY_MISSIONDATA)
			cmdTemp.m_apParam[0]	= (VOID*)(pPacket->GetMissionData());

		pDataPool->OnCommand_( &cmdTemp );
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
