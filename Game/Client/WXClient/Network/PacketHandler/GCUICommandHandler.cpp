#include "StdAfx.h"
#include "GCUICommand.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"

using namespace Packets;

uint GCUICommandHandler::Execute( GCUICommand* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	CDataPool::GetMe()->X_PARAM_Set(pPacket->GetParam(),pPacket->GetUIIndex());

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
