// CGAskStudyXinfa.h
// 
// 发送NPC的门派ID
// 
//////////////////////////////////////////////////////

#ifndef __CGASKSTUDYXINFA_H__
#define __CGASKSTUDYXINFA_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGAskStudyXinfa : public Packet
	{
	public:
		struct _STUDYXINFA_INFO		
		{
			short			m_idMenpai;
			short			m_idXinfa;
			short			m_NowLevel;
		};

		CGAskStudyXinfa( ){};
		virtual ~CGAskStudyXinfa( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKSTUDYXINFA ; }
		virtual UINT			GetPacketSize()const { return	sizeof(_STUDYXINFA_INFO);}

	public:
		
		_STUDYXINFA_INFO	GetUplevelInfoInfo(VOID) const {return m_UplevelInfo;}
		VOID				SetUplevelInfoInfo(_STUDYXINFA_INFO *UplevelInfo) 
		{
			m_UplevelInfo.m_idMenpai = UplevelInfo->m_idMenpai;
			m_UplevelInfo.m_idXinfa  = UplevelInfo->m_idXinfa ;
			m_UplevelInfo.m_NowLevel = UplevelInfo->m_NowLevel;
		}
	private:
		_STUDYXINFA_INFO		m_UplevelInfo;
	};


	class CGAskStudyXinfaFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGAskStudyXinfa() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_ASKSTUDYXINFA; };
		UINT		GetPacketMaxSize()const { return sizeof(CGAskStudyXinfa::_STUDYXINFA_INFO);};
	};

	class CGAskStudyXinfaHandler 
	{
	public:
		static UINT Execute( CGAskStudyXinfa* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif


