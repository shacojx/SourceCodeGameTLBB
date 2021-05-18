// GCSpecialObj_ActNowHandler.cpp

#include "stdafx.h"

#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Surface\Obj_Special.h"

#include "GCSpecialObj_ActNow.h"

using namespace Packets;
uint GCSpecialObj_ActNowHandler::Execute( GCSpecialObj_ActNow* pPacket, Player* pPlayer )
{
//__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->GetObjID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		const ObjID_List &listObjID	= pPacket->GetTargetList();

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_SPECIAL_OBJ_TRIGGER;
		cmdTemp.m_anParam[0]	= pPacket->GetLogicCount();
		cmdTemp.m_anParam[1]	= listObjID.m_nCount;
		cmdTemp.m_apParam[2]	= (VOID*)(listObjID.m_aIDs);
		pObj->PushCommand(&cmdTemp );

		pObj->PushDebugString("GCSpecialObj_ActNowHandler");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;

//__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
