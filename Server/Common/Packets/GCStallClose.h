// GCStallClose.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCSTALLCLOSE_H__
#define __GCSTALLCLOSE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class GCStallClose : public Packet
	{
	public:
		GCStallClose( ){};
		virtual ~GCStallClose( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_STALLCLOSE; }
		virtual UINT			GetPacketSize()const { return	0;}

	};

	class GCStallCloseFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCStallClose() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_STALLCLOSE; };
		UINT		GetPacketMaxSize()const { return 0;};
	};

	class GCStallCloseHandler 
	{
	public:
		static UINT Execute( GCStallClose* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
