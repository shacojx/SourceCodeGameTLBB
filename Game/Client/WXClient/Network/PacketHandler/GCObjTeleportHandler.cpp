// GCObjTeleportHandler.cpp

#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "TDException.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\World\WorldManager.h"

#include "GCObjTeleport.h"

using namespace Packets;

uint GCObjTeleportHandler::Execute( GCObjTeleport* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	//AxTrace(0, 2, "GCCharIdleHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		//检查位置是否合法
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			TDThrow("ERROR POSITION @ GCCharIdleHandler");
		}

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_TELEPORT;
		cmdTemp.m_afParam[0]	= pPacket->getWorldPos().m_fX;
		cmdTemp.m_afParam[1]	= pPacket->getWorldPos().m_fZ;
		pObj->PushCommand(&cmdTemp );

		CHAR szTemp[MAX_PATH];
		_snprintf(szTemp, MAX_PATH ,"ObjTeleport[%.1f,%.1f]", 
			pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ);

		pObj->PushDebugString(szTemp);
		//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,szTemp);
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
