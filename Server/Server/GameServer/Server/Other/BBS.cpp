#include "stdafx.h"
#include "BBS.h"
#include "TimeManager.h"//g_pTimeManager

ServerBBS::ServerBBS()
{
__ENTER_FUNCTION
	memset(m_szTempBuffer, 0, MAX_BBS_MESSAGE_LENGTH);
__LEAVE_FUNCTION
}
ServerBBS::~ServerBBS()
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

BOOL	ServerBBS::Init()
{
__ENTER_FUNCTION
	CleanUp();
__LEAVE_FUNCTION
	return	FALSE;
}
VOID	ServerBBS::CleanUp()
{
__ENTER_FUNCTION
	m_Serial		=	0;
	m_SerialIncStep	=	1;
	FinalID			=	1;
	m_FinalIndex	=	0;
	m_nTitleLength	=	0;
	memset(m_szTitle, 0, MAX_BBS_MESSAGE_LENGTH);
	memset(m_IsValid, 0, MAX_BBS_MESSAGE_LENGTH);
__LEAVE_FUNCTION

}

BOOL ServerBBS::AddNewMessageByID(UINT nID, CHAR*	szMessage, INT nLength, const CHAR* szAuthorName)
{
	__ENTER_FUNCTION
	INT	nIndex;
	if( nIndex = GetMessageIndexByID(nID) >= 0 )
	{//此ID已经存在
		return	FALSE;
	}

	
	MessageEntry_t	NewEntry;
	memcpy(NewEntry.szAuthorName, szAuthorName, MAX_BBS_MESSAGE_AUTHORLENGTH);
	NewEntry.nID		=	nID;
	g_pTimeManager->SetTime();
	NewEntry.nHour		=	(BYTE)g_pTimeManager->GetHour();
	NewEntry.nMin		=	(BYTE)g_pTimeManager->GetMinute();
	NewEntry.nMsgLength	=	nLength;
	memcpy(NewEntry.szMessage, szMessage, nLength);
	return	AddNewMessage(&NewEntry);
	__LEAVE_FUNCTION
	return	FALSE;
}

BOOL ServerBBS::ReplyMessageByID(UINT nID, CHAR*	szReplyMessage, INT	nReplyLength )
{
__ENTER_FUNCTION
	MessageEntry_t*	pCur = GetMessageByID( nID );
	if(pCur)
	{
		pCur->bHasReply			=	1;
		g_pTimeManager->SetTime();
		pCur->nReHour			=	(BYTE)g_pTimeManager->GetHour();
		pCur->nReMin			=	(BYTE)g_pTimeManager->GetMinute();
		pCur->nReplyMsgLength	=	nReplyLength;
		memcpy(pCur->szReplyMessage, szReplyMessage, nReplyLength);
		IncSerial();//序列号递增
		return	TRUE;
	}
	else
	{
		return	FALSE;
	}
__LEAVE_FUNCTION
	return	FALSE;
}

MessageEntry_t*	ServerBBS::GetMessageByID(UINT nID)
{
__ENTER_FUNCTION
	INT	nIndex;
	if( (nIndex = GetMessageIndexByID(nID)) < 0 )
	{
		return	NULL;
	}
	return &m_MessageData[nIndex];
	
__LEAVE_FUNCTION
	return NULL;
}

MessageEntry_t*	ServerBBS::GetMessageByIndex(UINT nIndex)
{
__ENTER_FUNCTION

	if( nIndex <0 || nIndex >=MAX_BBS_MESSAGE_NUM || m_IsValid[nIndex] == 0 )
	{
		return NULL;
	}
	else
	{
		return &m_MessageData[nIndex];
	}
__LEAVE_FUNCTION
 return NULL;
}

BOOL ServerBBS::DeleteMessageByID(UINT nID)
{
__ENTER_FUNCTION
	INT	nIndex;
	if( nIndex = GetMessageIndexByID(nID) < 0 )
	{
		return	FALSE;
	}
	m_IsValid[nIndex] = 0;
	IncSerial();//序列号递增
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

INT	ServerBBS::GetMessageIndexByID(UINT nID)
{
__ENTER_FUNCTION
	for(INT i = 0; i< MAX_BBS_MESSAGE_NUM; i++)
	{
		if(m_MessageData[i].nID == nID && m_IsValid[i] == 1)
		{
			return	i;
		}
	}
	return -1;
__LEAVE_FUNCTION
	return -1;
}
BOOL ServerBBS::AddNewMessage(MessageEntry_t* pMessage)
{
	__ENTER_FUNCTION
		
		m_MessageData[m_FinalIndex].nID				=	pMessage->nID;
		m_MessageData[m_FinalIndex].nHour			=	pMessage->nHour;
		m_MessageData[m_FinalIndex].nMin			=	pMessage->nMin;
		m_MessageData[m_FinalIndex].bHasReply		=	pMessage->bHasReply;
		m_MessageData[m_FinalIndex].nReHour			=	pMessage->nReHour;
		m_MessageData[m_FinalIndex].nReMin			=	pMessage->nReMin;
		m_MessageData[m_FinalIndex].nMsgLength		=	pMessage->nMsgLength;
		m_MessageData[m_FinalIndex].nReplyMsgLength	=	pMessage->nReplyMsgLength;
		memcpy(m_MessageData[m_FinalIndex].szAuthorName, pMessage->szAuthorName, MAX_BBS_MESSAGE_AUTHORLENGTH);
		memcpy(m_MessageData[m_FinalIndex].szMessage, pMessage->szMessage, pMessage->nMsgLength);
		memcpy(m_MessageData[m_FinalIndex].szReplyMessage, pMessage->szReplyMessage, pMessage->nReplyMsgLength);
		
		m_IsValid[m_FinalIndex] = 1;
		IncSerial();//序列号递增

		// 覆盖掉最老的
		if(m_FinalIndex==MAX_BBS_MESSAGE_NUM - 1)
		{
			m_FinalIndex =0;
		}
		else
		{
			m_FinalIndex++;
		}
		return	TRUE;
		__LEAVE_FUNCTION
			return	FALSE;
}
