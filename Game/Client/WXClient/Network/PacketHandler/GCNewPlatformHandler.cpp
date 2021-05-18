// GCNewPlatformHandler.cpp
#include "StdAfx.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\TripperObj\GMTripperObj_ItemBox.h"
#include "..\..\Object\TripperObj\GMTripperObj_Resource.h"
#include "..\..\Object\TripperObj\GMTripperObj_Platform.h"
#include "..\..\World\WorldManager.h"
#include "TDException.h"
#include "GCNewPlatform.h"


uint GCNewPlatformHandler::Execute( GCNewPlatform* pPacket, Player* pPlayer )
{
//__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNewItemBox::Execute");
	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		
		//检查位置是否合法
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			TDThrow("ERROR POSITION @ GCNewPlatformHandler");
		}

		//创建平台生长点
		CTripperObject_Platform* pPlatform = (CTripperObject_Platform*)CObjectManager::GetMe()->NewObject( "CTripperObject_Platform", pPacket->getObjID());
		if(!(pPlatform->SetPlatformID(pPacket->getType())))
		{
			//非法的平台ID
			CObjectManager::GetMe()->DestroyObject(pPlatform);
		}
		pPlatform->Initial(NULL);	
		//设置位置
		pPlatform->SetMapPosition( fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ));
		pPlatform->SetFaceDir( pPacket->getDir() );
	}

	return PACKET_EXE_CONTINUE ;

//__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
