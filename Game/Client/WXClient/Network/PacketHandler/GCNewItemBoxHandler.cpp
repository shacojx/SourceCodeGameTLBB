#include "StdAfx.h"
#include "GCNewItemBox.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\TripperObj\GMTripperObj_ItemBox.h"
#include "..\..\Object\TripperObj\GMTripperObj_Resource.h"
#include "..\..\World\WorldManager.h"
#include "..\..\Object\Character\Obj_Character.h"
#include "TDTimeSystem.h"
#include "..\..\Global.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "TDException.h"

using namespace Packets;
uint GCNewItemBoxHandler::Execute(GCNewItemBox* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNewItemBox::Execute");
	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		
		//检查位置是否合法
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			TDThrow("ERROR POSITION @ GCNewItemBoxHandler");
		}

		if(ITYPE_DROPBOX == pPacket->getObjType())
		{
			BOOL bMustCreater = TRUE;
			CObject* pObj = (CObject*)(pObjectManager->FindServerObject( pPacket->getMonsterID() ));
			if ( pObj != NULL && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)) )
			{
				CObject_Character *pCharacter		= (CObject_Character*)pObj;
				if(!pCharacter->IsDie())
				{
					ObjID_t		idItemBox		= pPacket->getObjID();
					GUID_t		idOwner			= pPacket->getOwner();
					WORLD_POS	posCreate		= pPacket->getWorldPos();
					pCharacter->AddDropBoxEvent(idItemBox, idOwner, &posCreate);
					bMustCreater = FALSE;
				}
			}

			if(bMustCreater)
			{
				//创建ItemBox
				CTripperObject_ItemBox* pBox = (CTripperObject_ItemBox*)CObjectManager::GetMe()->NewObject( "CTripperObject_ItemBox", pPacket->getObjID());
				pBox->Initial(NULL);	
				//设置位置
				pBox->SetMapPosition( fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ));
				//设置掉落箱的归属
				pBox->SetOwnerGUID(pPacket->getOwner());
			}
		}
		else
		{
			//创建矿物生长点
			CTripperObject_Resource* pResource = (CTripperObject_Resource*)CObjectManager::GetMe()->NewObject( "CTripperObject_Resource", pPacket->getObjID());
			if(!(pResource->SetResourceID(pPacket->getObjType())))
			{
				//非法的资源ID
				CObjectManager::GetMe()->DestroyObject(pResource);
				return PACKET_EXE_CONTINUE;
			}
			pResource->Initial(NULL);	
			//设置位置
			pResource->SetMapPosition( fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ));
		}
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}