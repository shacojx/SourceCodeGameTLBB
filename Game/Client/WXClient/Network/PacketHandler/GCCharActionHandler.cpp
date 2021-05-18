#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "GCCharAction.h"

uint GCCharActionHandler :: Execute( GCCharAction* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	////AxTrace(0, 2, "GCCharActionHandler::Execute");
	//if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	//{
	//	CObjectManager* pObjectManager = CObjectManager::GetMe();

	//	CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
	//	if ( pObj == NULL )
	//		return PACKET_EXE_CONTINUE;

	//	SCommand_Object cmdTemp;
	//	cmdTemp.m_wID			= OC_MAGIC_CHARGE;
	//	cmdTemp.m_auParam[0]	= 0;
	//	cmdTemp.m_anParam[1]	= pPacket->getLogicCount();
	//	cmdTemp.m_anParam[2]	= pPacket->getSkillDataID();
	//	cmdTemp.m_anParam[3]	= pPacket->getTargetID();
	//	cmdTemp.m_afParam[4]	= pPacket->getTargetPos().m_fX;
	//	cmdTemp.m_afParam[5]	= pPacket->getTargetPos().m_fZ;
	//	cmdTemp.m_afParam[6]	= pPacket->getDir();
	//	cmdTemp.m_auParam[7]	= pPacket->getTotalTime();
	//	pObj->PushCommand(&cmdTemp );

	//	pObj->PushDebugString("GCCharAction");
	//	pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	//}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

