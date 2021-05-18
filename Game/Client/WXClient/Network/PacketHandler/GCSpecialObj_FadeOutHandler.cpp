// GCSpecialObj_FadeOutHandler.cpp

#include "stdafx.h"

#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Surface\Obj_Special.h"

#include "GCSpecialObj_FadeOut.h"

using namespace Packets;
uint GCSpecialObj_FadeOutHandler::Execute( GCSpecialObj_FadeOut* pPacket, Player* pPlayer )
{
//__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->GetObjID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_SPECIAL_OBJ_DIE;
		pObj->PushCommand(&cmdTemp );

		pObj->PushDebugString("GCSpecialObj_FadeOutHandler");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;

//__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
