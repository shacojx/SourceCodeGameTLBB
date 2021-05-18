#ifndef __GCBBSMESSAGES_H__
#define __GCBBSMESSAGES_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class	GCBBSMessages : public Packet 
	{
	public:
		struct _MESSAGE_T
		{
			CHAR	AuthorName[MAX_BBS_MESSAGE_AUTHORLENGTH]; 
			UINT	nID;
			BYTE	nHour;
			BYTE	nMin;
			BYTE	bHasReply;
			BYTE	nMsgLength;
			CHAR	szMessage[MAX_BBS_MESSAGE_LENGTH];
			BYTE	nReHour;										
			BYTE	nReMin;											
			BYTE	nReplyMsgLength;
			CHAR	szReplyMessage[MAX_BBS_MESSAGE_LENGTH];

			_MESSAGE_T()
			{
				memset(AuthorName, 0, MAX_BBS_MESSAGE_AUTHORLENGTH);
				nID				=	0;
				nHour			=	0;
				nMin			=	0;
				bHasReply		=	0;
				nMsgLength		=	0;
				memset(szMessage, 0, MAX_BBS_MESSAGE_LENGTH);
				nReHour			=	0;										
				nReMin			=	0;
				nReplyMsgLength	=	0;
				memset(szReplyMessage, 0, MAX_BBS_MESSAGE_LENGTH);
			}
		};
	public:
		GCBBSMessages( )
		{
			m_objID			=	INVALID_ID;
			m_Serial		=	0;
			m_MessageNum	=	0;
			m_nTitleLength	=	0;
			memset(m_szTitle, 0, MAX_BBS_MESSAGE_LENGTH);
		}
		virtual ~GCBBSMessages( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_BBSMESSAGES ; }
		virtual UINT			GetPacketSize()const 
		{
			UINT nLength = sizeof(BYTE);//m_MessageNum
			nLength	+=	sizeof(ObjID_t);//m_objID
			nLength	+=	sizeof(UINT);//m_Serial
			nLength	+=	sizeof(BYTE);//m_nTitleLength
			nLength	+=	m_nTitleLength;//m_szTitle
			for(UINT i = 0; i<m_MessageNum; i++)
			{
				nLength += MAX_BBS_MESSAGE_AUTHORLENGTH;//AuthorName
				nLength += sizeof(UINT);//nID
				nLength += sizeof(BYTE);//nHour
				nLength += sizeof(BYTE);//nMin
				nLength += sizeof(BYTE);//bHasReply
				
				if(m_MessageData[i].bHasReply)
				{
					nLength += sizeof(BYTE);//nReHour
					nLength += sizeof(BYTE);//nReMin

					nLength += sizeof(BYTE);//nMsgLength
					nLength += m_MessageData[i].nMsgLength;//szMessage

					nLength += sizeof(BYTE);//nReplyMsgLength
					nLength += m_MessageData[i].nReplyMsgLength;//szReplyMessage
				}
				else
				{
					nLength += sizeof(BYTE);//nMsgLength
					nLength += m_MessageData[i].nMsgLength;//szMessage
				}
			}
			return nLength;
		}

	public:
		ObjID_t					GetObjID(VOID) const {return m_objID;};
		VOID					SetObjID(ObjID_t ObjID) {m_objID = ObjID;};

		UINT					GetSerial(VOID) const {return m_Serial;};
		VOID					SetSerial(UINT Serial) {m_Serial = Serial;};

		BYTE					GetTitleLength(VOID) const {return m_nTitleLength;};
		VOID					SetTitleLength(BYTE nTitleLength) {m_nTitleLength = nTitleLength;};

		CHAR*					GetTitle(VOID) {return m_szTitle;}
		VOID					SetTitle(CHAR* pszTitle)
		{
			memcpy(m_szTitle, pszTitle, m_nTitleLength);
		}

		BYTE					GetMessageNum(VOID) const {return m_MessageNum;};
		VOID					SetMessageNum(BYTE nNumber) {m_MessageNum = nNumber;};

		_MESSAGE_T*				GetMessageData(VOID) { return m_MessageData; }
		VOID					SetMessageData(_MESSAGE_T* pSour)
		{
			memcpy(m_MessageData, pSour, m_MessageNum*sizeof(_MESSAGE_T));
		}

	private:
		ObjID_t					m_objID;		//摊主id
		UINT					m_Serial;		//序列号
		BYTE					m_nTitleLength;
		CHAR					m_szTitle[MAX_BBS_MESSAGE_LENGTH];
		BYTE					m_MessageNum;
		_MESSAGE_T				m_MessageData[MAX_BBS_MESSAGE_NUM];
	};

	class GCBBSMessagesFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCBBSMessages() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_BBSMESSAGES ; }
		UINT		GetPacketMaxSize()const { return sizeof(GCBBSMessages::_MESSAGE_T)*MAX_BBS_MESSAGE_NUM + 
												sizeof(BYTE) + 
												sizeof(ObjID_t) + 
												sizeof(UINT) + 
												sizeof(BYTE) +
												MAX_BBS_MESSAGE_LENGTH;  }
	};

	class GCBBSMessagesHandler 
	{
	public:
		static UINT Execute( GCBBSMessages* pPacket, Player* pPlayer ) ;
	};
};
using namespace Packets ;

#endif
