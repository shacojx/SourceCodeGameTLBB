
#include "stdafx.h"
#include "GCPlayerShopSaleOut.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"

UINT GCPlayerShopSaleOutHandler::Execute( GCPlayerShopSaleOut* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BOOL bSaleOut = pPacket->GetSaleOut();
			
			CDataPool::GetMe()->PlayerShop_SetIsSaleOut( TRUE, bSaleOut );
			CDataPool::GetMe()->PlayerShop_SetShopSerial( TRUE, pPacket->GetSerial() );

			CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_MY_SHOP);
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
