

#ifndef __GWCHANNELKICK_H__
#define __GWCHANNELKICK_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWChannelKick : public Packet 
	{
	public:
		GWChannelKick( ){} ;
		virtual ~GWChannelKick( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_CHANNELKICK ; }
			virtual UINT			GetPacketSize()const { return sizeof(GUID_t)*2 ; }


	public :
		//数据接口
		VOID			SetSourGUID( GUID_t guid ){	m_SourGUID = guid ; }
		GUID_t			GetSourGUID( ){ return m_SourGUID ; }

		VOID			SetDestGUID( GUID_t guid ){	m_DestGUID = guid ; }
		GUID_t			GetDestGUID( ){ return m_DestGUID ; }


	private :
		//数据
		GUID_t			m_SourGUID ;		//踢人者
		GUID_t			m_DestGUID ;		//被踢者

	};


	class GWChannelKickFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWChannelKick() ; }
		PacketID_t	GetPacketID()const { return PACKET_GW_CHANNELKICK ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t)*2; }			
	};


	class GWChannelKickHandler 
	{
	public:
		static UINT Execute( GWChannelKick* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
