

#ifndef __CGCHANNELINVITE_H__
#define __CGCHANNELINVITE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGChannelInvite : public Packet 
	{
	public:
		CGChannelInvite( ){} ;
		virtual ~CGChannelInvite( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_CHANNELINVITE ; }
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


	class CGChannelInviteFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGChannelInvite() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_CHANNELINVITE ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(GUID_t)*2; }			
	};


	class CGChannelInviteHandler 
	{
	public:
		static UINT Execute( CGChannelInvite* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
