#include "StdAfx.h"
#include "..\..\Network\NetManager.h"
#include "GCPlayerShopApply.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "TDException.h"
#include "CGPlayerShopEstablish.h"
#include "..\..\Object\ObjectManager.h"

UINT GCPlayerShopApplyHandler::Execute( GCPlayerShopApply* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{

			FLOAT ComFactor	=	pPacket->GetComFactor();	//当前商业指数
			UINT  Cost		=	pPacket->GetCost();			//开店金额
			BYTE  nType		=	pPacket->GetType();			//可开的类型
			ObjID_t NpcId	=	pPacket->GetNpcId();		//Npc Id

			INT ObjectID = (CObjectManager::GetMe()->FindServerObject(NpcId))->GetID();

			//收到这个消息，一定级别测试通过
			CDataPool::GetMe()->PlayerShop_SetCommercialFactor(ComFactor);
			CDataPool::GetMe()->PlayerShop_SetCost(Cost);
			CDataPool::GetMe()->PlayerShop_SetType(nType);
			CDataPool::GetMe()->PlayerShop_SetNpcID(ObjectID);

			//打开摊位确定界面
			CEventSystem::GetMe()->PushEvent(GE_PS_OPEN_CREATESHOP, ObjectID);
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
