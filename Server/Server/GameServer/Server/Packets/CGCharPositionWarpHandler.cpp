#include "stdafx.h"
// CGCharPositionWarpHandler.cpp

#include "Log.h"
#include "Scene.h"
#include "GamePlayer.h"
#include "Obj_Human.h"

#include "CGCharPositionWarp.h"

uint CGCharPositionWarpHandler::Execute( CGCharPositionWarp* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer != NULL ) ;
	if ( pGamePlayer == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharPositionWarpHandler: Error (pGamePlayer == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman != NULL ) ;
	if ( pHuman == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharPositionWarpHandler: Error (pHuman == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Scene* pScene = pHuman->getScene() ;
	Assert( pScene != NULL ) ;
	if ( pScene == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharPositionWarpHandler: Error Obj = %d (pScene == NULL) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	if ( MyGetCurrentThreadID() != pScene->m_ThreadID )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharPositionWarpHandler: Error Obj = %d (MyGetCurrentThreadID() != pScene->m_ThreadID) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	// 是否是当前服务器的位置
	FLOAT fServerLen = fabsf(pPacket->getServerPos().m_fX - pHuman->getWorldPos()->m_fX) + fabsf(pPacket->getServerPos().m_fZ - pHuman->getWorldPos()->m_fZ);
	if(fServerLen > 0.1f)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharPositionWarpHandler: ErrorObj = %d fServerLen > 0.1f ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE;
	}

	WORLD_POS pos1(pPacket->getServerPos().m_fX, pPacket->getServerPos().m_fZ);
	WORLD_POS pos2(pPacket->getClientPos().m_fX, pPacket->getClientPos().m_fZ);
	FLOAT fDist = sqrtf((pos1.m_fX-pos2.m_fX)*(pos1.m_fX-pos2.m_fX) + (pos1.m_fZ-pos2.m_fZ)*(pos1.m_fZ-pos2.m_fZ));
	if(fDist > DEF_SERVER_ADJUST_POS_WARP_DIST)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharPositionWarpHandler: ErrorObj = %d fDist > DEF_SERVER_ADJUST_POS_WARP_DIST", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE;
	}

	WORLD_POS posTarget = pPacket->getClientPos();
	pHuman->Teleport(&posTarget);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGCharPositionWarpHandler::ObjID=%d, ServerX=%f ServerZ=%f, ClientX=%f ClientZ=%f",
		pHuman->GetID(), pPacket->getServerPos().m_fX, pPacket->getServerPos().m_fZ, pPacket->getClientPos().m_fX, pPacket->getClientPos().m_fZ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
