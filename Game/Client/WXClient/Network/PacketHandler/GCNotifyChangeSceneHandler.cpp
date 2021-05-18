#include "StdAfx.h"
#include "GCNotifyChangeScene.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\World\WorldManager.h"
#include "TDDBC_Struct.h"

uint GCNotifyChangeSceneHandler::Execute( GCNotifyChangeScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNotifyChangeSceneHandle[%.1f,%.1f]r::Execute", 
	//	pPacket->getTargetPos()->m_fX, pPacket->getTargetPos()->m_fZ);
	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		INT nSourceID = CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine()->nServerID;
		if(nSourceID != pPacket->getCurrentSceneID()) return PACKET_EXE_CONTINUE;

		CWorldManager::GetMe()->ChangeScene(
			pPacket->getTargetSceneID(),
			fVector2(pPacket->getTargetPos()->m_fX, pPacket->getTargetPos()->m_fZ),
			pPacket->getTargetDir(),
			pPacket->getFlag());
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
