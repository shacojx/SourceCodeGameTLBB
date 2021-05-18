// GCExchangeCancel.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCEXCHANGECANCEL_H__
#define __GCEXCHANGECANCEL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCExchangeCancel : public Packet
	{
	public:
		GCExchangeCancel( ){};
		virtual ~GCExchangeCancel( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EXCHANGECANCEL; }
		virtual UINT			GetPacketSize()const { return	0;}

	};

	class GCExchangeCancelFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCExchangeCancel() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_EXCHANGECANCEL; };
		UINT		GetPacketMaxSize()const { return 0;};
	};

	class GCExchangeCancelHandler 
	{
	public:
		static UINT Execute( GCExchangeCancel* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
