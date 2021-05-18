/*
留言板服务器端结构
*/

#ifndef _BBS_H_
#define _BBS_H_

#include "Type.h"
class Obj_Human;

/*
单条留言结构
*/
struct MessageEntry_t
{
	CHAR	szAuthorName[MAX_BBS_MESSAGE_AUTHORLENGTH];				//发帖人名字
	UINT	nID;											//帖子ID
	BYTE	nHour;											//发表时间（小时）
	BYTE	nMin;											//发表时间（分钟）
	BYTE	bHasReply;										//是否有回复
	UINT	nMsgLength;										//帖子长度
	CHAR	szMessage[MAX_BBS_MESSAGE_LENGTH];				//帖子内容
	BYTE	nReHour;										//回复时间（小时）
	BYTE	nReMin;											//回复时间（分钟）
	UINT	nReplyMsgLength;								//回复长度
	CHAR	szReplyMessage[MAX_BBS_MESSAGE_LENGTH];			//回复内容


	MessageEntry_t()
	{
		memset(szAuthorName, 0, MAX_BBS_MESSAGE_AUTHORLENGTH);
		nID			=	0;
		nHour		=	0;
		nMin		=	0;
		bHasReply	=	0;
		nMsgLength	=	0;
		memset(szMessage, 0, MAX_BBS_MESSAGE_LENGTH);
		nReHour		=	0;
		nReMin		=	0;
		nReplyMsgLength	=	0;
		memset(szReplyMessage, 0, MAX_BBS_MESSAGE_LENGTH);
	}
};

/*
留言板结构
*/
class ServerBBS
{
public:
	ServerBBS();
	~ServerBBS();

	BOOL			Init( ) ;
	VOID			CleanUp( ) ;

	/*
	Message
	*/
	BOOL			AddNewMessageByID(UINT nID, CHAR*	szMessage, INT nLength, const CHAR* szAuthorName);
	BOOL			ReplyMessageByID(UINT nID, CHAR*	szReplyMessage, INT	nReplyLength);
	BOOL			DeleteMessageByID(UINT nID);
	MessageEntry_t*	GetMessageByIndex(UINT nIndex);

	/*
	ID
	*/
	UINT			NewMessageID()	
	{
		FinalID++;
		if(FinalID == 0)
			return ++FinalID;
		else
			return FinalID;
	}

	/*
	title
	*/
	BOOL			SetBBSTitle(CHAR*	szMessage, INT nLength)
	{
		memset(m_szTitle, 0, MAX_BBS_MESSAGE_LENGTH);
		m_nTitleLength	=	nLength;
		memcpy(m_szTitle, szMessage, nLength);
		IncSerial();
		return TRUE;
	}
	CHAR*			GetBBSTitle(INT& nLength)
	{
		nLength	=	m_nTitleLength;
		return	m_szTitle;
	}

	/*
	TempBuff
	*/
	VOID			SaveTitle()
	{
		SetTempBuff(m_szTitle, m_nTitleLength);
	}

	VOID			LoadTitle()
	{
		INT titleLength = 0;
		CHAR*	pszTitle = GetTempBuff(titleLength);
		if(titleLength!=0)
			SetBBSTitle(pszTitle, titleLength);
		/*else
			memset(m_szTitle, 0, MAX_BBS_MESSAGE_LENGTH);*/
	}

	/*
	Serial change
	*/
	UINT			GetSerial() {return m_Serial;}
	VOID			IncSerial()
	{
		m_Serial += m_SerialIncStep;
	}
	VOID			SetSerialIncStep(UINT SerialIncStep) {m_SerialIncStep = SerialIncStep;}
	UINT			GetSerialIncStep() {return m_SerialIncStep;}

	UINT			GetFinalIndex()const{return m_FinalIndex;}

private:
	BOOL			AddNewMessage(MessageEntry_t* pMessage);
	INT				GetMessageIndexByID(UINT nID);
	MessageEntry_t*	GetMessageByID(UINT nID);

	BOOL			SetTempBuff(CHAR*	szMessage, INT nLength)
	{
		if(nLength<MAX_BBS_MESSAGE_LENGTH)
		{
			memset(m_szTempBuffer, 0, MAX_BBS_MESSAGE_LENGTH);
			memcpy(m_szTempBuffer, szMessage, nLength);
		}
		return TRUE;
	}
	CHAR*			GetTempBuff(INT& nLength)
	{
		nLength	=	(INT)strlen(m_szTempBuffer);
		return	m_szTempBuffer;
	}


private:
	UINT			m_SerialIncStep;
	UINT			FinalID;
    UINT			m_Serial;
	UINT			m_nTitleLength;
	BYTE			m_FinalIndex;
	CHAR			m_szTitle[MAX_BBS_MESSAGE_LENGTH];
	BYTE			m_IsValid[MAX_BBS_MESSAGE_NUM];
	MessageEntry_t	m_MessageData[MAX_BBS_MESSAGE_NUM];			
	CHAR			m_szTempBuffer[MAX_BBS_MESSAGE_LENGTH];//存储临时缓存的摊位广告语
};

#endif