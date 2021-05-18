

#ifndef __CGCHANNELDISMISS_H__
#define __CGCHANNELDISMISS_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGChannelDismiss : public Packet 
	{
	public:
		CGChannelDismiss( ){} ;
		virtual ~CGChannelDismiss( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_CHANNELDISMISS ; }
			virtual UINT			GetPacketSize()const { return sizeof(GUID_t) ; }

	public :
		VOID				SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t				GetGUID( ){ return m_GUID ; }

	private :
		GUID_t				m_GUID ;	//解散自建聊天室的人

	};


	class CGChannelDismissFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGChannelDismiss() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_CHANNELDISMISS ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t); }			
	};


	class CGChannelDismissHandler 
	{
	public:
		static UINT Execute( CGChannelDismiss* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
