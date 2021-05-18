
#include "stdafx.h"
#include "GCPlayerShopUpdateFavorite.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"

UINT GCPlayerShopUpdateFavoriteHandler::Execute( GCPlayerShopUpdateFavorite* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			for(INT i =0; i<CDataPool::GetMe()->PlayerShop_GetShopNum();i++)
			{
				if(CDataPool::GetMe()->PlayerShop_GetIDByIndex(i) == pPacket->GetShopID())
				{
					CDataPool::GetMe()->PlayerShop_SetIsFavorByIndex(i, pPacket->GetIsInFavorite());
					break;
				}
			}
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
