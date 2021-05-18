#ifndef __CGBBSSYCHMESSAGES_H__
#define __CGBBSSYCHMESSAGES_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class	CGBBSSychMessages : public Packet 
	{
	public:
		enum
		{
			OPT_NONE = 0,
			OPT_NEW_MESSAGE,
			OPT_REPLY_MESSAGE,
			OPT_DEL_MESSAGE,
			OPT_SET_TITLE,
		};
	public:
		CGBBSSychMessages( )
		{
			m_Opt	=	OPT_NONE;
			m_nID	=	0;
			m_nLength	=	0;
			memset(m_MessageData, 0, MAX_BBS_MESSAGE_LENGTH);
		}
		virtual ~CGBBSSychMessages( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_BBSSYCHMESSAGES ; }
		virtual UINT			GetPacketSize()const 
		{
			UINT nlength = sizeof(BYTE);//m_Opt
			nlength += sizeof(ObjID_t);
			switch(m_Opt)
			{
			case OPT_NEW_MESSAGE:
				{
					nlength += sizeof(BYTE);//m_nLength
					nlength += m_nLength;//m_MessageData
				}
				break;
			case OPT_REPLY_MESSAGE:
				{
					nlength += sizeof(UINT);//m_nID
					nlength += sizeof(BYTE);//m_nLength
					nlength += m_nLength;//m_MessageData
				}
				break;
			case OPT_DEL_MESSAGE:
				{
					nlength += sizeof(UINT);//m_nID
				}
				break;
			case OPT_SET_TITLE:
				{
					nlength += sizeof(BYTE);//m_nLength
					nlength += m_nLength;//m_MessageData
				}
				break;
			default:
				break;
			}
			return nlength;
		};
	public:

		ObjID_t					GetObjID(VOID) const {return m_objID;};
		VOID					SetObjID(ObjID_t ObjID) {m_objID = ObjID;};

		BYTE					GetOpt(){return m_Opt;}
		VOID					SetOpt(BYTE	Opt){m_Opt = Opt;}

		BYTE					GetLength(){return m_nLength;}
		VOID					SetLength(BYTE	nLength){m_nLength = nLength;}

		UINT					GetID(){return m_nID;}
		VOID					SetID(UINT	nID){m_nID = nID;}

		CHAR*					GetMessage(){return m_MessageData;}
		VOID					SetMessage(CHAR* pSour)
		{
			strncpy( m_MessageData, pSour, MAX_BBS_MESSAGE_LENGTH-1 ) ;
			m_MessageData[MAX_BBS_MESSAGE_LENGTH-1] = 0 ;
			m_nLength = (BYTE)strlen(m_MessageData); //矫正长度
		}


	private:
		ObjID_t					m_objID;		//申请者id
		BYTE					m_Opt;
		UINT					m_nID;
		BYTE					m_nLength;
		CHAR					m_MessageData[MAX_BBS_MESSAGE_LENGTH];
	};

	class CGBBSSychMessagesFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGBBSSychMessages() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_BBSSYCHMESSAGES ; }
		UINT		GetPacketMaxSize()const { return sizeof(ObjID_t)+sizeof(BYTE)+sizeof(UINT)+sizeof(BYTE)+MAX_BBS_MESSAGE_LENGTH;  }
	};

	class CGBBSSychMessagesHandler 
	{
	public:
		static UINT Execute( CGBBSSychMessages* pPacket, Player* pPlayer ) ;
	};
};
using namespace Packets ;

#endif
