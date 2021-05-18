#include "stdafx.h"
// GCBBSMessages.cpp
// 
/////////////////////////////////////////////////////

#include "GCBBSMessages.h"

BOOL GCBBSMessages::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_objID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_Serial), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_nTitleLength), sizeof(BYTE));
	iStream.Read( (CHAR*)(m_szTitle), m_nTitleLength);
	iStream.Read( (CHAR*)(&m_MessageNum), sizeof(BYTE));
	for(UINT i=0; i<m_MessageNum; i++)
	{
		iStream.Read( (CHAR*)(m_MessageData[i].AuthorName), MAX_BBS_MESSAGE_AUTHORLENGTH);
		iStream.Read( (CHAR*)(&m_MessageData[i].nID), sizeof(UINT));
		iStream.Read( (CHAR*)(&m_MessageData[i].nHour), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_MessageData[i].nMin), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_MessageData[i].bHasReply), sizeof(BYTE));
		if(m_MessageData[i].bHasReply)
		{
			iStream.Read( (CHAR*)(&m_MessageData[i].nReHour), sizeof(BYTE));
			iStream.Read( (CHAR*)(&m_MessageData[i].nReMin), sizeof(BYTE));

			iStream.Read( (CHAR*)(&m_MessageData[i].nMsgLength), sizeof(BYTE));
			iStream.Read( (CHAR*)(m_MessageData[i].szMessage), m_MessageData[i].nMsgLength);

			iStream.Read( (CHAR*)(&m_MessageData[i].nReplyMsgLength), sizeof(BYTE));
			iStream.Read( (CHAR*)(m_MessageData[i].szReplyMessage), m_MessageData[i].nReplyMsgLength);
		}
		else
		{
			iStream.Read( (CHAR*)(&m_MessageData[i].nMsgLength), sizeof(BYTE));
			iStream.Read( (CHAR*)(m_MessageData[i].szMessage), m_MessageData[i].nMsgLength);
		}
	}

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCBBSMessages::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_objID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_Serial), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_nTitleLength), sizeof(BYTE));
	oStream.Write( (CHAR*)(m_szTitle), m_nTitleLength);
	oStream.Write( (CHAR*)(&m_MessageNum), sizeof(BYTE));
	for(UINT i=0; i<m_MessageNum; i++)
	{
		oStream.Write( (CHAR*)(m_MessageData[i].AuthorName), MAX_BBS_MESSAGE_AUTHORLENGTH);
		oStream.Write( (CHAR*)(&m_MessageData[i].nID), sizeof(UINT));
		oStream.Write( (CHAR*)(&m_MessageData[i].nHour), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_MessageData[i].nMin), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_MessageData[i].bHasReply), sizeof(BYTE));
		if(m_MessageData[i].bHasReply)
		{
			oStream.Write( (CHAR*)(&m_MessageData[i].nReHour), sizeof(BYTE));
			oStream.Write( (CHAR*)(&m_MessageData[i].nReMin), sizeof(BYTE));

			oStream.Write( (CHAR*)(&m_MessageData[i].nMsgLength), sizeof(BYTE));
			oStream.Write( (CHAR*)(m_MessageData[i].szMessage), m_MessageData[i].nMsgLength);

			oStream.Write( (CHAR*)(&m_MessageData[i].nReplyMsgLength), sizeof(BYTE));
			oStream.Write( (CHAR*)(m_MessageData[i].szReplyMessage), m_MessageData[i].nReplyMsgLength);
		}
		else
		{
			oStream.Write( (CHAR*)(&m_MessageData[i].nMsgLength), sizeof(BYTE));
			oStream.Write( (CHAR*)(m_MessageData[i].szMessage), m_MessageData[i].nMsgLength);
		}
	}
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCBBSMessages::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCBBSMessagesHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

