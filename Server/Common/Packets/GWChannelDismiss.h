

#ifndef __GWCHANNELDISMISS_H__
#define __GWCHANNELDISMISS_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWChannelDismiss : public Packet 
	{
	public:
		GWChannelDismiss( ){} ;
		virtual ~GWChannelDismiss( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_CHANNELDISMISS ; }
			virtual UINT			GetPacketSize()const { return sizeof(GUID_t) ; }

	public :
		VOID				SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t				GetGUID( ){ return m_GUID ; }

	private :
		GUID_t				m_GUID ;	//解散队伍的人

	};


	class GWChannelDismissFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWChannelDismiss() ; }
		PacketID_t	GetPacketID()const { return PACKET_GW_CHANNELDISMISS ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t); }			
	};


	class GWChannelDismissHandler 
	{
	public:
		static UINT Execute( GWChannelDismiss* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
