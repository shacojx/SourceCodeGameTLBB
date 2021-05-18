
#ifndef __GC_GUILDAPPLY_H__
#define __GC_GUILDAPPLY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCGuildApply : public Packet
	{
	public:
		GCGuildApply(){}
		virtual					~GCGuildApply(){}

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_GUILDAPPLY; }

		virtual UINT			GetPacketSize() const{return sizeof(ObjID_t);}

	public:
		ObjID_t					GetNpcId(){return m_NpcId;}
		VOID					SetNpcId(ObjID_t NpcId){m_NpcId = NpcId;}

	private:
		ObjID_t					m_NpcId;
	};

	class GCGuildApplyFactory : public PacketFactory
	{
	public:
		Packet*					CreatePacket() { return new GCGuildApply(); }
		PacketID_t				GetPacketID() const { return PACKET_GC_GUILDAPPLY; }
		UINT					GetPacketMaxSize() const { return sizeof(ObjID_t); }
	};

	class GCGuildApplyHandler
	{
	public:
		static UINT Execute( GCGuildApply* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // __GC_GUILDAPPLY_H__
