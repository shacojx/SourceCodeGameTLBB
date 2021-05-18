// GCAbilityLevel.h

#ifndef _GC_ABILITY_LEVEL_H_
#define _GC_ABILITY_LEVEL_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCAbilityLevel:public Packet
	{

	public:
		GCAbilityLevel(){};
		virtual				~GCAbilityLevel(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_ABILITYLEVEL; }
		virtual UINT			GetPacketSize() const { return sizeof(AbilityID_t)+sizeof(UINT); }

	public:
		
		VOID			setAbilityId(AbilityID_t id) {m_AbilityID = id;};
		AbilityID_t		getAbilityId() {return m_AbilityID;}
		
		VOID			setLevel(UINT lvl) {m_Level = lvl;}
		UINT			getLevel() {return m_Level;}
		

	private:
		UINT			m_Level;
		AbilityID_t		m_AbilityID;

	};
	
	class GCAbilityLevelFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCAbilityLevel(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_ABILITYLEVEL; }
		UINT		GetPacketMaxSize() const {return sizeof(AbilityID_t)+sizeof(UINT); }
	};

	class GCAbilityLevelHandler
	{
	public:
		static UINT Execute( GCAbilityLevel* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // _GC_ABILITY_LEVEL_H_
