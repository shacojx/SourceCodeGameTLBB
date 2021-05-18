// CGStallClose.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __CGSTALLCLOSE_H__
#define __CGSTALLCLOSE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGStallClose : public Packet
	{
	public:
		CGStallClose( ){};
		virtual ~CGStallClose( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_STALLCLOSE; }
		virtual UINT			GetPacketSize()const { return	0;}

	};

	class CGStallCloseFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGStallClose() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_STALLCLOSE; };
		UINT		GetPacketMaxSize()const { return 0;};
	};

	class CGStallCloseHandler 
	{
	public:
		static UINT Execute( CGStallClose* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
