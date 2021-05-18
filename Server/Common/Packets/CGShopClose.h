// CGShopClose.h
// 
// 客户端通知服务器买什么东西
// 
//////////////////////////////////////////////////////

#ifndef __CGSHOPCLOSE_H__
#define __CGSHOPCLOSE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGShopClose : public Packet
	{
	public:
		CGShopClose( ){};
		virtual ~CGShopClose( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_SHOPCLOSE; }
		virtual UINT			GetPacketSize()const { return	0;}
	};

	class CGShopCloseFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGShopClose() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_SHOPCLOSE; };
		UINT		GetPacketMaxSize()const { return 0;};
	};

	class CGShopCloseHandler 
	{
	public:
		static UINT Execute( CGShopClose* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
