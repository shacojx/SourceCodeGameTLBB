// CGStallEstablish.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __CGSTALLESTABLISH_H__
#define __CGSTALLESTABLISH_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class CGStallEstablish : public Packet
	{
	public:
		CGStallEstablish( ){};
		virtual ~CGStallEstablish( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_STALLESTABLISH; }
		virtual UINT			GetPacketSize()const { return	0;}

	};

	class CGStallEstablishFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGStallEstablish() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_STALLESTABLISH; };
		UINT		GetPacketMaxSize()const { return 0;};
	};

	class CGStallEstablishHandler 
	{
	public:
		static UINT Execute( CGStallEstablish* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
