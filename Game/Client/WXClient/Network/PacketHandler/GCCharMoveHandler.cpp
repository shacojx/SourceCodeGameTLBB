#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Network\NetManager.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "GCCharMove.h"

#include "..\..\World\WorldManager.h"
#include "TDException.h"

#include "CGEnvRequest.h"
#include "..\WxRender\RenderSystem.h"


uint GCCharMoveHandler::Execute( GCCharMove* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		//检查位置是否合法
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getTargetPos().m_fX, pPacket->getTargetPos().m_fZ)))
		{
			TDThrow("ERROR POSITION @ GCCharMoveHandler target");
		}

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		if ( pObj == NULL )
		{
			//--- FOR DEBUG
			// DANGER!!!!!!!!

			//发送环境请求
			CGEnvRequest msgEnvRequest;
			CNetManager::GetMe()->SendPacket(&msgEnvRequest) ;

			CGameProcedure::s_pGfxSystem->PushDebugString("AskEvnRequest!!!!!");

			//--- FOR DEBUG
			return PACKET_EXE_CONTINUE;
		}

		if(pPacket->IsHaveStopInfo())
		{
			SCommand_Object cmdTemp;
			cmdTemp.m_wID			= OC_STOP_MOVE;
			cmdTemp.m_anParam[0]	= pPacket->getStopLogicCount();
			cmdTemp.m_anParam[1]	= 0;
			cmdTemp.m_afParam[2]	= pObj->GetPosition().x;// pPacket->getStopPos()->m_fX;
			cmdTemp.m_afParam[3]	= pObj->GetPosition().z;// pPacket->getStopPos()->m_fZ;
			pObj->PushCommand(&cmdTemp );
		}

		WORLD_POS posTarget;
		posTarget.m_fX	= pPacket->getTargetPos().m_fX;
		posTarget.m_fZ	= pPacket->getTargetPos().m_fZ;

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_MOVE;
		cmdTemp.m_auParam[0]	= pPacket->getStartTime();
		cmdTemp.m_anParam[1]	= pPacket->getHandleID();
		cmdTemp.m_anParam[2]	= 1;
		cmdTemp.m_apParam[3]	= &posTarget;
		pObj->PushCommand(&cmdTemp );

		//--- for debug
		pObj->Enable(OSF_VISIABLE);
		//--- for debug

		CHAR szTemp[MAX_PATH];
		_snprintf(szTemp, MAX_PATH ,"CharMove[%.1f,%.1f]->[%.1f,%.1f]", 
			pObj->GetPosition().x, pObj->GetPosition().z,
			pPacket->getTargetPos().m_fX, pPacket->getTargetPos().m_fZ);

		pObj->PushDebugString(szTemp);
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

