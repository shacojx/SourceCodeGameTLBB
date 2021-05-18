#include "StdAfx.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"


#include "GCRetMissionDesc.h"



using namespace Packets;

uint GCRetMissionDescHandler::Execute( GCRetMissionDesc* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pDataPool);


		SCommand_DPC cmdTemp;
		cmdTemp.m_wID			= DPC_UPDATE_MISSION_DESC;

		cmdTemp.m_apParam[0]	= (VOID*)(pPacket);
		pDataPool->OnCommand_( &cmdTemp );
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
