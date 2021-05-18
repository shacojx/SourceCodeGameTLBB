
#ifndef __CG_GUILDAPPLY_H__
#define __CG_GUILDAPPLY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGGuildApply : public Packet
	{
	public:
		CGGuildApply()
		{
			m_GuildNameSize = 0;
			memset(m_GuildName, 0, MAX_GUILD_NAME_SIZE);

			m_GuildDescSize = 0;
			memset(m_GuildDesc, 0, MAX_GUILD_DESC_SIZE);
		}
		virtual					~CGGuildApply(){};

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_GUILDAPPLY; }

		virtual UINT			GetPacketSize() const{return	sizeof(BYTE)+
																m_GuildNameSize+
																sizeof(BYTE)+
																m_GuildDescSize;
													 }

	public:
		VOID					SetGuildNameSize( BYTE bSize ){ m_GuildNameSize = bSize; }
		BYTE					GetGuildNameSize( ){ return m_GuildNameSize; }

		VOID					SetGuildName( CHAR* pName )
		{
			strncpy( m_GuildName, pName, MAX_GUILD_NAME_SIZE-1 );
			m_GuildName[MAX_GUILD_NAME_SIZE-1] = 0 ;
			m_GuildNameSize = (BYTE)strlen(m_GuildName); //矫正长度
		}
		CHAR*					GetGuildName(){ return (CHAR*)m_GuildName ; }

		VOID					SetGuildDescSize( BYTE bSize ){ m_GuildDescSize = bSize; }
		BYTE					GetGuildDescSize( ){ return m_GuildDescSize; }

		VOID					SetGuildDesc( CHAR* pName )
		{
			strncpy( m_GuildDesc, pName, MAX_GUILD_DESC_SIZE-1 ) ;
			m_GuildDesc[MAX_GUILD_DESC_SIZE-1] = 0 ;
			m_GuildDescSize = (BYTE)strlen(m_GuildDesc); //矫正长度
		}
		CHAR*					GetGuildDesc(){ return (CHAR*)m_GuildDesc; }

	private:
		BYTE					m_GuildNameSize;
		CHAR					m_GuildName[MAX_GUILD_NAME_SIZE];

		BYTE					m_GuildDescSize;
		CHAR					m_GuildDesc[MAX_GUILD_DESC_SIZE];

	};

	class CGGuildApplyFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new CGGuildApply(); }
		PacketID_t				GetPacketID() const { return PACKET_CG_GUILDAPPLY; }
		UINT					GetPacketMaxSize() const { return	sizeof(BYTE)+
																	MAX_GUILD_NAME_SIZE+
																	sizeof(BYTE)+
																	MAX_GUILD_DESC_SIZE;
														 }
	};

	class CGGuildApplyHandler
	{
	public:
		static UINT Execute( CGGuildApply* pPacket, Player* pPlayer );
	};
}

using namespace Packets;

#endif // __GC_GUILDAPPLY_H__
