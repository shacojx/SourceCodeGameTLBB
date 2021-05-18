// CGExchangeOkIII.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __CGEXCHANGEOKIII_H__
#define __CGEXCHANGEOKIII_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGExchangeOkIII : public Packet
	{
	public:
		CGExchangeOkIII( ){};
		virtual ~CGExchangeOkIII( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_EXCHANGEOKIII; }
		virtual UINT			GetPacketSize()const { return	0;}
	};

	class CGExchangeOkIIIFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGExchangeOkIII() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_EXCHANGEOKIII; };
		UINT		GetPacketMaxSize()const { return 0;}
	};

	class CGExchangeOkIIIHandler 
	{
	public:
		static UINT Execute( CGExchangeOkIII* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
