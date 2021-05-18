// GCPlayerRelive.h
// 
// 主角复活
//
//////////////////////////////////////////////////////

#ifndef __GCPLAYERRELIVE_H__
#define __GCPLAYERRELIVE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCPlayerRelive : public Packet
	{
	public:
		GCPlayerRelive( )
		{
		}
		virtual ~GCPlayerRelive( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYER_RELIVE ; }
		virtual UINT			GetPacketSize()const { return	0; }

	};


	class GCPlayerReliveFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerRelive() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYER_RELIVE; }
		UINT		GetPacketMaxSize()const { return	0; }
	};

	class GCPlayerReliveHandler 
	{
	public:
		static UINT Execute( GCPlayerRelive* pPacket, Player* pPlayer ) ;
	};
}

#endif // __GCPLAYERRELIVE_H__
