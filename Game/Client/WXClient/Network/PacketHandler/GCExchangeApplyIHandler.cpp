/*
被申请的客户端收到交易申请
*/
#include "stdafx.h"
#include "GCExchangeApplyI.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Character\Obj_Character.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Interface\GMGameInterface.h"
#include "TDException.h"

uint GCExchangeApplyIHandler::Execute( GCExchangeApplyI* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{//初始化双方交易盒，打开界面
			//加入等待对列
			ObjID_t Appid = pPacket->GetObjID();
			CObject_Character* pCharObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject( Appid ));
			ENUM_RELATION sCamp = CGameInterface::GetMe()->GetCampType( pCharObj, (CObject*)CObjectManager::GetMe()->GetMySelf() );
			if( sCamp != RELATION_FRIEND )
			{//是敌人
				return PACKET_EXE_CONTINUE;
			}

			CDataPool::GetMe()->AddToAppList(Appid);

			//通知界面提示用户有人申请与你交易
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"收到一个新的交易请求！");
			CEventSystem::GetMe()->PushEvent(GE_RECEIVE_EXCHANGE_APPLY);
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
