#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "GCCharSkill_CreateBullet.h"

uint GCCharSkill_CreateBulletHandler :: Execute( GCCharSkill_CreateBullet* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	//AxTrace(0, 2, "GCCharMoveHandler::Execute");
	//if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	//{
	//	CObjectManager* pObjectManager = CObjectManager::GetMe();

	//	CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
	//	if ( pObj == NULL )
	//		return PACKET_EXE_CONTINUE;

	//	SCommand_Object cmdTemp;
	//	cmdTemp.m_wID			= OC_MOVE;
	//	pObj->PushCommand( FALSE, &cmdTemp );

	//	pObj->PushDebugString("GCCharMove");
	//	pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	//}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

