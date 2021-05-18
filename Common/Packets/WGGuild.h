#ifndef __WG_GUILD_H__
#define __WG_GUILD_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGGuild : public Packet
	{
	public:
		WGGuild(){}
		virtual					~WGGuild(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_GUILD; }

		virtual UINT			GetPacketSize() const
		{
			return sizeof(PlayerID_t) + m_GuildPacket.GetPacketSize() + sizeof(m_GUID);
		}

	public:
		VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t				GetPlayerID( ) { return m_PlayerID; }

		_GUILD_WGC_PACKET*		GetGuildPacket() { return &m_GuildPacket; }

		VOID					SetGUID(GUID_t guid) { m_GUID = guid; }
		GUID_t					GetGUID() { return m_GUID; }

	private:
		PlayerID_t				m_PlayerID;	//¡¨Ω”’ﬂ
		_GUILD_WGC_PACKET		m_GuildPacket;
		GUID_t					m_GUID;

	};

	class WGGuildFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new WGGuild(); }
		PacketID_t				GetPacketID() const { return PACKET_WG_GUILD; }
		UINT					GetPacketMaxSize() const { return _GUILD_WGC_PACKET::GetPacketMaxSize() + sizeof(GUID_t); }
	};

	class WGGuildHandler
	{
	public:
		static UINT Execute( WGGuild* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // __WG_GUILD_H__
