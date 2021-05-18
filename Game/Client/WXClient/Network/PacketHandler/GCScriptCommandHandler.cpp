
#include "StdAfx.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\GameCommand.h"
#include "..\..\DataPool\DataPoolCommandDef.h"
#include "..\..\DataPool\GMUIDataPool.h"

#include "GCScriptCommand.h"

using namespace Packets;

uint GCScriptCommandHandler::Execute( GCScriptCommand* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pDataPool);


	SCommand_DPC cmdTemp;
	cmdTemp.m_wID			= DPC_SCRIPT_COMMAND;

	cmdTemp.m_anParam[0]	= pPacket->getCmdID();
	cmdTemp.m_apParam[1]	= pPacket->getBuf();
	pDataPool->OnCommand_( &cmdTemp );

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
