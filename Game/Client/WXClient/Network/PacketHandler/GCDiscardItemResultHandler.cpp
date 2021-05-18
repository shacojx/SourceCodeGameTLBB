#include "StdAfx.h"
#include "GCDiscardItemResult.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\WxRender\RenderSystem.h"

using namespace Packets;


uint GCDiscardItemResultHandler::Execute(GCDiscardItemResult* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	//AxTrace(0, 2, "GCDiscardItemResultHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		DISCARDITEM_RESULT ret = (DISCARDITEM_RESULT)pPacket->getResult();
		switch(ret)
		{
		case DISCARDITEM_SUCCESS:
			{
				if(pPacket->getOpt() == GCDiscardItemResult::FromBag)
				{
					CDataPool::GetMe()->UserBag_SetItemInfo(pPacket->getBagIndex(), TRUE, NULL);
				}
				else if(pPacket->getOpt() == GCDiscardItemResult::FromBank)
				{
					CDataPool::GetMe()->UserBank_SetItem(pPacket->getBagIndex(),  NULL, TRUE);
					//Ë¢ÐÂActionSystem
					//...
					CActionSystem::GetMe()->UserBank_Update();
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
				}
			}
			break;

		case DISCARDITEM_FAIL:
		default:
			{
				CGameProcedure::s_pGfxSystem->PushDebugString("Discard item failed!");
			}
			break;
		}

	}
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}