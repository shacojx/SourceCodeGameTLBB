// GCRemoveCanPickMissionItemHandler.cpp
#include "StdAfx.h"
#include "..\..\GameCommand.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"

#include "GCRemoveCanPickMissionItem.h"

using namespace Packets;

uint GCRemoveCanPickMissionItemHandler::Execute( GCRemoveCanPickMissionItem* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//加入UI数据池
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pDataPool);

		SCommand_DPC cmdTemp;
		cmdTemp.m_wID			= DPC_REMOVE_CAN_PICK_MISSION_ITEM;
		cmdTemp.m_adwParam[0]	= pPacket->GetItemDataID();

		pDataPool->OnCommand_( &cmdTemp );
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
