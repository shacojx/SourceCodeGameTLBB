#ifndef _CG_STOPTEAMFOLLOW_H_
#define _CG_STOPTEAMFOLLOW_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGStopTeamFollow:	public Packet
	{
	public:
		CGStopTeamFollow(){}
		virtual		~CGStopTeamFollow(){}

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_STOP_TEAMFOLLOW; }
		virtual UINT			GetPacketSize()const { return 0; }

	};

	class CGStopTeamFollowFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGStopTeamFollow(); }
		PacketID_t	GetPacketID() const	 { return PACKET_CG_STOP_TEAMFOLLOW; }
		UINT		GetPacketMaxSize() const { return 0; }			
	};

	class CGStopTeamFollowHandler
	{
	public:
		static UINT Execute(CGStopTeamFollow* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // _CG_STOPTEAMFOLLOW_H_
