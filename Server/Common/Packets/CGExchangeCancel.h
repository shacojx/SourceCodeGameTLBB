// CGExchangeCancel.h
// 
// 
// 
//////////////////////////////////////////////////////

#ifndef __CGEXCHANGECANCEL_H__
#define __CGEXCHANGECANCEL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGExchangeCancel : public Packet
	{
	public:
		CGExchangeCancel( ){};
		virtual ~CGExchangeCancel( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_EXCHANGECANCEL; }
		virtual UINT			GetPacketSize()const { return	0;}
	};

	class CGExchangeCancelFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGExchangeCancel() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_EXCHANGECANCEL; };
		UINT		GetPacketMaxSize()const { return 0;};
	};

	class CGExchangeCancelHandler 
	{
	public:
		static UINT Execute( CGExchangeCancel* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
