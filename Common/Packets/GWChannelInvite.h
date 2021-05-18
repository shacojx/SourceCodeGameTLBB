

#ifndef __GWCHANNELINVITE_H__
#define __GWCHANNELINVITE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWChannelInvite : public Packet 
	{
	public:
		GWChannelInvite( ){} ;
		virtual ~GWChannelInvite( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_CHANNELINVITE ; }
			virtual UINT			GetPacketSize()const { return sizeof(GUID_t)*2 ; }

	public :
		//数据接口
		VOID			SetSourGUID( GUID_t guid ){	m_SourGUID = guid ; }
		GUID_t			GetSourGUID( ){ return m_SourGUID ; }

		VOID			SetDestGUID( GUID_t guid ){	m_DestGUID = guid ; }
		GUID_t			GetDestGUID( ){ return m_DestGUID ; }


	private :
		//数据
		GUID_t			m_SourGUID ;		//邀请人
		GUID_t			m_DestGUID ;		//被邀请人

	};


	class GWChannelInviteFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWChannelInvite() ; }
		PacketID_t	GetPacketID()const { return PACKET_GW_CHANNELINVITE ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t)*2; }			
	};


	class GWChannelInviteHandler 
	{
	public:
		static UINT Execute( GWChannelInvite* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
