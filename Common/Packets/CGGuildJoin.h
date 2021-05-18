
#ifndef __CG_GUILDJOIN_H__
#define __CG_GUILDJOIN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGGuildJoin : public Packet
	{
	public:
		CGGuildJoin()
		{
			m_GuildID = INVALID_ID;
		}
		virtual					~CGGuildJoin(){};

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_GUILDJOIN; }

		virtual UINT			GetPacketSize() const{return sizeof(GuildID_t); }

	public:
		GuildID_t				GetGuildID(){ return m_GuildID;}
		VOID					SetGuildID(GuildID_t GuildID){m_GuildID = GuildID;}
	private:
		GuildID_t				m_GuildID;


	};

	class CGGuildJoinFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new CGGuildJoin(); }
		PacketID_t				GetPacketID() const { return PACKET_CG_GUILDJOIN; }
		UINT					GetPacketMaxSize() const { return sizeof(GuildID_t); }
	};

	class CGGuildJoinHandler
	{
	public:
		static UINT Execute( CGGuildJoin* pPacket, Player* pPlayer );
	};
}

using namespace Packets;

#endif // __CG_GUILDJOIN_H__
