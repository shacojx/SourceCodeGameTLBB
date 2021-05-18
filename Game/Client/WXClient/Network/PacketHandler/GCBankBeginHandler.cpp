#include"stdafx.h"
#include "..\..\Network\NetManager.h"
#include "GCBankBegin.h"
#include "CGBankAcquireList.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerOther.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\DataPool\GMDataPool.h"
#include "TDException.h"

uint GCBankBeginHandler::Execute( GCBankBegin* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			//将银行NPC的ID保存到数据池
			ObjID_t ObjServerId = pPacket->GetNPCObjID();
			INT ObjectID = (CObjectManager::GetMe()->FindServerObject(ObjServerId))->GetID();
			CDataPool::GetMe()->UserBank_SetNpcId(ObjectID);

			CGBankAcquireList Msg;
            CNetManager::GetMe()->SendPacket(&Msg);
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
