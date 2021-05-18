// CGStallApply.h
// 
// 客户端向服务器申请摆摊
// 
//////////////////////////////////////////////////////

#ifndef __CGSTALLAPPLY_H__
#define __CGSTALLAPPLY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class CGStallApply : public Packet
	{
	public:
		CGStallApply( ){};
		virtual ~CGStallApply( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_STALLAPPLY; }
		virtual UINT			GetPacketSize()const { return	0;}

	};

	class CGStallApplyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGStallApply() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_STALLAPPLY; };
		UINT		GetPacketMaxSize()const { return 0;};
	};

	class CGStallApplyHandler 
	{
	public:
		static UINT Execute( CGStallApply* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
