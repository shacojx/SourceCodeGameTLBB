// GCStallEstablish.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCSTALLESTABLISH_H__
#define __GCSTALLESTABLISH_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class GCStallEstablish : public Packet
	{
	public:
		GCStallEstablish( ){};
		virtual ~GCStallEstablish( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_STALLESTABLISH; }
		virtual UINT			GetPacketSize()const { return	0;}

	};

	class GCStallEstablishFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCStallEstablish() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_STALLESTABLISH; };
		UINT		GetPacketMaxSize()const { return 0;};
	};

	class GCStallEstablishHandler 
	{
	public:
		static UINT Execute( GCStallEstablish* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
