#include "stdafx.h"
//#include "GWSystemMsg.h"
//#include "SceneManager.h"
//#include "Scene.h"
//#include "CGSystemMsg.h"
//
//UINT CGSystemMsgHandler::Execute( CGSystemMsg* pPacket,Player* pPlayer) 
//{
//	__ENTER_FUNCTION
//
//	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
//	Assert( pGamePlayer ) ;
//
//	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
//	Assert( pHuman ) ;
//
//	Scene* pScene = pHuman->getScene() ;
//	if( pScene==NULL )
//	{
//		Assert(FALSE) ;
//		return PACKET_EXE_ERROR ;
//	}
//
//	//检查线程执行资源是否正确
//	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
//
//	if( pGamePlayer->GetPlayerStatus() == PS_SERVER_NORMAL )
//	{
//		BYTE nContexSize = pPacket->GetContexSize();
//		if( nContexSize >= MAX_SYSTEM_MSG_SIZE ) nContexSize = MAX_SYSTEM_MSG_SIZE-1;
//
//		GWSystemMsg *pMsg = (GWSystemMsg*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_SYSTEMMSG);
//
//		pMsg->SetMessageType( GWSystemMsg::SYSTEMMSG_TYPE_PLAYER );
//		pMsg->SetContexSize(nContexSize);
//		pMsg->SetContex(pPacket->GetContex());
//
//		g_pServerManager->SendPacket( pMsg, INVALID_ID );
//
//		g_pLog->FastSaveLog( LOG_FILE_1, "CGSystemMsgHandler: GUID=%X, Type=%d, Size=%d, Contex=%s", 
//			pHuman->GetGUID(), GWSystemMsg::SYSTEMMSG_TYPE_PLAYER, pPacket->GetContexSize(), pPacket->GetContex() ) ;
//	}
//
//	return PACKET_EXE_CONTINUE ;
//
//	__LEAVE_FUNCTION
//
//	return PACKET_EXE_ERROR ;
//
//}
