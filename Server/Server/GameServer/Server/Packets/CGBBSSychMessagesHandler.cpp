#include "stdafx.h"
/*
客户端通知服务器留言板被修改
*/

#include "CGBBSSychMessages.h"
#include "CGBBSApply.h"
#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "BBS.h"
#include "GCStallError.h"
#include "GCBBSMessages.h"

uint CGBBSSychMessagesHandler::Execute( CGBBSSychMessages* pPacket, Player* pPlayer )
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

	ObjID_t	ObjID			=	pPacket->GetObjID();
	BYTE	Opt				=	pPacket->GetOpt();
	UINT	nID				=	pPacket->GetID();
	BYTE	nLength			=	pPacket->GetLength();
	CHAR*	m_MessageData	=	pPacket->GetMessage();

	if( nLength >= MAX_BBS_MESSAGE_LENGTH ) nLength = MAX_BBS_MESSAGE_LENGTH-1;
	m_MessageData[nLength] = 0;

	ReplaceIllegalString( m_MessageData, nLength );

	Obj_Human* pTargetHuman = pScene->GetHumanManager()->GetHuman( ObjID );
	if( pTargetHuman == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGBBSApplyHandler::ObjID=%d, ObjId = %d"
			,pHuman->GetID(), ObjID) ;
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

	ServerBBS*	pBBS	=	pTargetHuman->m_StallBox.GetBBS();
	switch(Opt)
	{
		case CGBBSSychMessages::OPT_NEW_MESSAGE:
			{
				CHAR szDisplayName[MAX_BBS_MESSAGE_AUTHORLENGTH];
				sprintf(szDisplayName, "%s(%X)", pHuman->GetName(), (UINT)(pHuman->GetGUID()));
				UINT	NewID = pBBS->NewMessageID();
				if(pBBS->AddNewMessageByID( NewID, m_MessageData, nLength, szDisplayName ) == FALSE)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGBBSSychMessagesHandler::ObjID=%d, NewID = %d"
					,pHuman->GetID(), NewID) ;
					return PACKET_EXE_CONTINUE;
				}
			}
			break;
		case CGBBSSychMessages::OPT_REPLY_MESSAGE:
			{
				if(pBBS->ReplyMessageByID(nID, m_MessageData, nLength) == FALSE)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGBBSSychMessagesHandler::ObjID=%d, nID = %d"
						,pHuman->GetID(), nID) ;
					return PACKET_EXE_CONTINUE;
				}
			}
			break;
		case CGBBSSychMessages::OPT_DEL_MESSAGE:
			{
				//...
			}
			break;
		case CGBBSSychMessages::OPT_SET_TITLE:
			{
				if(pBBS->SetBBSTitle( m_MessageData, nLength) == FALSE)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGBBSSychMessagesHandler::ObjID=%d"
						,pHuman->GetID()) ;
					return PACKET_EXE_CONTINUE;
				}
			}
			break;
		default:
			break;
	}

	//发送新的留言板给客户端
	MessageEntry_t* pEntry		= NULL;
	GCBBSMessages::_MESSAGE_T	MessageList[MAX_BBS_MESSAGE_NUM];
	UINT k						= 0;
	GCBBSMessages	MsgBBS;
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

	pGamePlayer->SendPacket(&MsgBBS);
	g_pLog->FastSaveLog( LOG_FILE_1, "CGBBSSychMessagesHandler::ObjID=%d"
		,pHuman->GetID()) ;
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
