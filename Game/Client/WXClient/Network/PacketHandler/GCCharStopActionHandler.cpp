#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "GCCharStopAction.h"

using namespace Packets;

uint GCCharStopActionHandler :: Execute( GCCharStopAction* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_STOP_ACTION;
		cmdTemp.m_anParam[0]	= pPacket->getLogicCount();
		cmdTemp.m_auParam[1]	= pPacket->getStopTime();
		pObj->PushCommand(&cmdTemp );

		pObj->PushDebugString("GCCharStopAction");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

