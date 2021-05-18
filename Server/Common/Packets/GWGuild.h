#ifndef __GW_GUILD_H__
#define __GW_GUILD_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWGuild : public Packet
	{
	public:
		GWGuild(){}
		virtual					~GWGuild(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_GUILD; }

		virtual UINT			GetPacketSize() const
		{
			return sizeof(m_GUID) + m_GuildPacket.GetPacketSize();
		}

	public:
		VOID					SetGUID(GUID_t guid) { m_GUID = guid; }
		GUID_t					GetGUID() { return m_GUID; }

		VOID					SetGuildPacket(const _GUILD_CGW_PACKET* pGuildPacket)
		{
			memcpy((void*)&m_GuildPacket, pGuildPacket, sizeof(_GUILD_CGW_PACKET));
		}
		_GUILD_CGW_PACKET*		GetGuildPacket() { return &m_GuildPacket; }

	private:
		GUID_t					m_GUID;
		_GUILD_CGW_PACKET		m_GuildPacket;

	};

	class GWGuildFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new GWGuild(); }
		PacketID_t				GetPacketID() const { return PACKET_GW_GUILD; }
		UINT					GetPacketMaxSize() const { return sizeof(GUID_t) + _GUILD_CGW_PACKET::GetPacketMaxSize(); }
	};

	class GWGuildHandler
	{
	public:
		static UINT Execute( GWGuild* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // __GW_GUILD_H__
