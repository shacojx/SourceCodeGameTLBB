#include "StdAfx.h"
#include "GCNewPortal.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\TripperObj\GMTripperObj_Transport.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "GCCharJump.h"

#include "..\..\World\WorldManager.h"
#include "TDException.h"

uint GCNewPortalHandler::Execute( GCNewPortal* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNewPortalHandler::Execute");
	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		//检查位置是否合法
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			TDThrow("ERROR POSITION @ GCNewPortalHandler");
		}

		//创建ItemBox
		CTripperObject_Transport* pTransport = (CTripperObject_Transport*)CObjectManager::GetMe()->NewObject( "CTripperObject_Transport", pPacket->getObjID());

		pTransport->Initial(NULL);	

		//设置位置
		pTransport->SetMapPosition( fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ));

	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
