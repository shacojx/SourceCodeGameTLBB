#include "StdAfx.h"
#include "GCPlayerShopStallStatus.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"

UINT GCPlayerShopStallStatusHandler::Execute( GCPlayerShopStallStatus* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			_PLAYERSHOP_GUID	nShopID		=	pPacket->GetShopID();			//商店ID
			BYTE				nStallIndex	=	pPacket->GetStallIndex();		//柜台索引
			BYTE				bStatus		=	pPacket->GetStatus();			//柜台状态
			BYTE				FinalStallNum=	pPacket->GetFinalStallNum();	//柜台总数

			CDataPool::GetMe()->PlayerShop_SetStallEnableByIndex( TRUE, nStallIndex, bStatus);
			CDataPool::GetMe()->PlayerShop_SetOpenStallNum(TRUE, FinalStallNum);
		}

		//通知商店更新
		CEventSystem::GetMe()->PushEvent(GE_PS_UPDATE_MY_SHOP);

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
