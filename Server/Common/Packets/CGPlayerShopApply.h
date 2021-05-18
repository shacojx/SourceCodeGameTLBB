// CGPlayerShopApply.h
// 
// 向服务器申请创建玩家商店
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPAPPLY_H__
#define __CGPLAYERSHOPAPPLY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GCPlayerShopError.h"
namespace Packets
{
	class CGPlayerShopApply : public Packet
	{
	public:
		CGPlayerShopApply( ){};
		virtual ~CGPlayerShopApply( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPAPPLY; }
		virtual UINT			GetPacketSize()const { return	0;}

	};

	class CGPlayerShopApplyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopApply() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPAPPLY; };
		UINT		GetPacketMaxSize()const { return 0;};
	};

	class CGPlayerShopApplyHandler 
	{
	public:
		static UINT Execute( CGPlayerShopApply* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
