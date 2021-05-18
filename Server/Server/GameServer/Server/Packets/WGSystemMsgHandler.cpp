#include "stdafx.h"
#include "WGSystemMsg.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GCSystemMsg.h"
#include "GWSystemMsg.h"

UINT WGSystemMsgHandler::Execute( WGSystemMsg* pPacket,Player* pPlayer) 
{
__ENTER_FUNCTION

	Assert( MyGetCurrentThreadID()==g_pServerManager->m_ThreadID );

	GCSystemMsg *pMsg=(GCSystemMsg*)g_pPacketFactoryManager->CreatePacket(PACKET_GC_SYSTEMMSG);
	BYTE nContexSize = pPacket->GetContexSize();

	switch( pPacket->GetMessageType() )
	{
	case GWSystemMsg::SYSTEMMSG_TYPE_PLAYER:
	case GWSystemMsg::SYSTEMMSG_TYPE_GM:
	case GWSystemMsg::SYSTEMMSG_TYPE_SCRIPT:
	case GWSystemMsg::SYSTEMMSG_TYPE_AUTO:
	case GWSystemMsg::SYSTEMMSG_TYPE_TEST:
		{
			if( nContexSize >= MAX_SYSTEM_MSG_SIZE ) nContexSize = MAX_SYSTEM_MSG_SIZE-1;
			pMsg->SetGUID(pPacket->GetGUID());
			pMsg->SetMessageType(pPacket->GetMessageType());
			pMsg->SetContexSize(nContexSize);
			pMsg->SetContex(pPacket->GetContex());

			g_pSceneManager->BroadCast_Scene(pMsg);
		}
		break;
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "WGSystemMsgHandler...!  GUID=%X, MsgType=%d, ContexSize=%d, Contex=%s",
		pPacket->GetGUID(), pPacket->GetMessageType(), nContexSize, pPacket->GetContex() );

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;

}
