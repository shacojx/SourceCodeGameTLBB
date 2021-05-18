#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerOther.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\NetWork\NetManager.h"
#include "CGCharAskBaseAttrib.h"
#include "GCNewPlayer_Move.h"
#include "..\..\World\WorldManager.h"
#include "TDException.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Object\GMObjectLoadQueue.h"
#include "..\WxRender\RenderSystem.h"

uint GCNewPlayer_MoveHandler :: Execute( GCNewPlayer_Move* pPacket, Player* pPlayer ) 
{
//__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNewPlayerHandler");
//	return PACKET_EXE_CONTINUE ;

	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		//检查位置是否合法
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			TDThrow("ERROR POSITION @ GCNewPlayer_MoveHandler");
		}

		//创建玩家
		CObject_PlayerOther* pOther = (CObject_PlayerOther*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		fVector3 fvGame;
		CGameProcedure::s_pGfxSystem->Axis_Trans(
			CRenderSystem::AX_PLAN, fVector3(pPacket->getWorldPos().m_fX, 0.0f, pPacket->getWorldPos().m_fZ),
			CRenderSystem::AX_GAME, fvGame);

		if ( pOther == NULL )
		{
			pOther = pObjectManager->NewPlayerOther( (INT)pPacket->getObjID() );

			SObjectInit initPlayerOther;
			initPlayerOther.m_fvPos	= fvGame;

			pOther->Initial( &initPlayerOther );
		}
		else
		{
			fVector2 mapPos(fvGame.x, fvGame.z);
			pOther->SetMapPosition(mapPos);
			pOther->Enable( OSF_VISIABLE );
			pOther->Disalbe( OSF_OUT_VISUAL_FIELD );
		}

		////激活鼠标查询
		//if(pOther->GetRenderInterface())
		//{
		//	pOther->Enable(OSF_RAY_QUERY);
		//}

		//更新装备信息
		pOther->GetCharacterData()->Set_EquipVer(pPacket->getEquipVer());
		//跟新移动速度
		pOther->GetCharacterData()->Set_MoveSpeed(pPacket->getMoveSpeed());

		// move to command
		{
			WORLD_POS posTarget;
			posTarget.m_fX	= pPacket->getTargetPos().m_fX;
			posTarget.m_fZ	= pPacket->getTargetPos().m_fZ;

			SCommand_Object cmdTemp;
			cmdTemp.m_wID			= OC_MOVE;
			cmdTemp.m_auParam[0]	= 0;
			cmdTemp.m_anParam[1]	= pPacket->getHandleID();
			cmdTemp.m_anParam[2]	= 1;
			cmdTemp.m_apParam[3]	= &posTarget;
			pOther->PushCommand(&cmdTemp );
		}

		//放入Ask队列
		CObjectManager::GetMe()->GetLoadQueue()->TryAddLoadTask(pOther->GetID(), CObject_Character::CT_PLAYEROTHER);

		// tempcode{
		// 此版不做服务器繁忙客户端延后发消息的处理
//		CGCharAskBaseAttrib msgAskBaseAttrib;
//		msgAskBaseAttrib.setTargetID( (INT)pPacket->getObjID() );
//		CNetManager::GetMe()->SendPacket( &msgAskBaseAttrib );

		//AxTrace(0, 0, "[%d]Ask %d", m_hmObject.size(), pPacket->getObjID());
		// }

		pOther->PushDebugString("GCNewPlayer");
		pOther->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;

//__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
//__ENTER_FUNCTION
//	AxTrace(0, 2, "GCNewPlayerHandler");
//
//	//当前流程是主流程
//	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
//	{
//		CObjectManager* pObjectManager = CObjectManager::GetMe();
//
//		//创建玩家
//		CObject_PlayerOther* pOther = pObjectManager->CreatePlayerOther(
//											(INT)pPacket->getObjID(),
//											(INT)pPacket->getRace(),
//											pPacket->getEquipVer(),
//											fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ),
//											true);
//
//		if(!pOther) return PACKET_EXE_CONTINUE;
//
//		//设置新的位置
//		pOther->SetMapPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ));
//		pOther->RegisterToZone();
//		//IDLE装态
//		pOther->SetMotionStatus(CMS_STATUS_IDLE);
//		pOther->PushDebugString("GCNewPlayer");
//		pOther->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
//	}
//
//	return PACKET_EXE_CONTINUE ;
//
//__LEAVE_FUNCTION
//
//	return PACKET_EXE_ERROR ;
}

