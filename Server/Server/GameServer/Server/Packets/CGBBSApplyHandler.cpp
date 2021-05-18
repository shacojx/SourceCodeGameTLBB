#include "stdafx.h"
/*
客户端申请留言板信息
*/

#include "CGBBSApply.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCStallError.h"
#include "GCBBSMessages.h"
#include "BBS.h"
uint CGBBSApplyHandler::Execute( CGBBSApply* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}
	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	ObjID_t ObjId	= pPacket->GetObjID();
	UINT Serial		= pPacket->GetSerial();

	Obj_Human* pTargetHuman = pScene->GetHumanManager()->GetHuman( ObjId );
	if( pTargetHuman == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGBBSApplyHandler::ObjID=%d, ObjId = %d"
			,pHuman->GetID(), ObjId) ;
		return PACKET_EXE_CONTINUE ;
	}
	
	if(pTargetHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_OPEN)
	{
		GCStallError	Msg;
		Msg.SetID(STALL_MSG::ERR_CLOSE);
		pGamePlayer->SendPacket(&Msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallOpenHandler::ObjID=%d, ERR_CLOSE"
			,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE;
	}

	//从留言板中取出留言，并更新到客户端
	//摊主BBS
	ServerBBS*	pBBS			= pTargetHuman->m_StallBox.GetBBS();
	MessageEntry_t* pEntry		= NULL;
	GCBBSMessages::_MESSAGE_T	MessageList[MAX_BBS_MESSAGE_NUM];
	UINT k						= 0;
	GCBBSMessages	MsgBBS;

	
	if(Serial == pBBS->GetSerial())
	{//版本号相同，不用发新的了
		MsgBBS.SetObjID(pTargetHuman->GetID());
		MsgBBS.SetSerial(Serial);
	}
	else
	{//版本号不同发新的
		UINT	CurIndex = pBBS->GetFinalIndex();
		for(UINT i = 0; i<MAX_BBS_MESSAGE_NUM; i++)
		{
			pEntry = pBBS->GetMessageByIndex(CurIndex);
			if(pEntry)
			{
				MessageList[k].bHasReply		=	pEntry->bHasReply;
				MessageList[k].nID				=	pEntry->nID;
				MessageList[k].nHour			=	pEntry->nHour;
				MessageList[k].nMin				=	pEntry->nMin;
				MessageList[k].nMsgLength		=	pEntry->nMsgLength;
				MessageList[k].nReplyMsgLength	=	pEntry->nReplyMsgLength;
				MessageList[k].nReHour			=	pEntry->nReHour;
				MessageList[k].nReMin			=	pEntry->nReMin;

				memcpy(MessageList[k].AuthorName, pEntry->szAuthorName, MAX_BBS_MESSAGE_AUTHORLENGTH);
				memcpy(MessageList[k].szMessage, pEntry->szMessage, pEntry->nMsgLength);
				memcpy(MessageList[k].szReplyMessage, pEntry->szReplyMessage, pEntry->nReplyMsgLength);
				k++;
			}
			CurIndex++;
			if(CurIndex == MAX_BBS_MESSAGE_NUM)
				CurIndex = 0;
		}

		INT titleLength = 0;
		CHAR*	pszTitle = pBBS->GetBBSTitle(titleLength);

		MsgBBS.SetObjID(pTargetHuman->GetID());
		MsgBBS.SetSerial(pBBS->GetSerial());
		MsgBBS.SetTitleLength(titleLength);
		MsgBBS.SetTitle(pszTitle);
		MsgBBS.SetMessageNum(k);
		MsgBBS.SetMessageData(MessageList);
	}

	pGamePlayer->SendPacket(&MsgBBS);
	g_pLog->FastSaveLog( LOG_FILE_1, "CGStallOpenHandler::ObjID=%d"
		,pHuman->GetID()) ;
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
