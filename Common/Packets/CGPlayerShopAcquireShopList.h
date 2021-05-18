// CGPlayerShopAcquireShopList.h
// 
// 向服务器申请商店列表
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPACQUIRESHOPLIST_H__
#define __CGPLAYERSHOPACQUIRESHOPLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GCPlayerShopError.h"
namespace Packets
{
	class CGPlayerShopAcquireShopList : public Packet
	{
	public:
		CGPlayerShopAcquireShopList( ){};
		virtual ~CGPlayerShopAcquireShopList( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPACQUIRESHOPLIST; }
		virtual UINT			GetPacketSize()const { return	0;}

	};

	class CGPlayerShopAcquireShopListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopAcquireShopList() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPACQUIRESHOPLIST; };
		UINT		GetPacketMaxSize()const { return 0;};
	};

	class CGPlayerShopAcquireShopListHandler 
	{
	public:
		static UINT Execute( CGPlayerShopAcquireShopList* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
