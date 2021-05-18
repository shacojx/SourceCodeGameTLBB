#include "StdAfx.h"
#include "GCDiscardEquipResult.h"
#include "GCDiscardItemResult.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\WxRender\RenderSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"

using namespace Packets;

uint GCDiscardEquipResultHandler::Execute(GCDiscardEquipResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	//AxTrace(0, 2, "GCDiscardEquipResultHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		DISCARDEQUIP_RESULT ret = (DISCARDEQUIP_RESULT)pPacket->GetResult();
		switch(ret)
		{
		case DISCARDEQUIP_SUCCESS:
			{
				//---------------------------------------------------
				//Êý¾Ý³Ø
				CDataPool::GetMe()->UserEquip_SetItem((HUMAN_EQUIP)pPacket->GetEquipPoint(), NULL, TRUE);

				//---------------------------------------------------
				//Âß¼­²ã
				CObject_PlayerMySelf * pMySelf = CObjectManager::GetMe()->GetMySelf();
				pMySelf->UnEquipItem((HUMAN_EQUIP)pPacket->GetEquipPoint());
			}
			break;

		case DISCARDEQUIP_CANNT_DISCARD:
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("Equip can't destory!");
			}
			break;
		case DISCARDEQUIP_NO_SUCH_EQUIP:
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("No such equip!");
			}
			break;
		case DISCARDEQUIP_FAIL:
		default:
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("Destroy equip failed!");
			}
			break;
		}

	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}