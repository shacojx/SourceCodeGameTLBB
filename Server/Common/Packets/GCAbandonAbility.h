// GCAbandonAbility.h

#ifndef _GC_ABANDON_ABILITY_H_
#define _GC_ABANDON_ABILITY_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCAbandonAbility:public Packet
	{

	public:
		GCAbandonAbility(){};
		virtual				~GCAbandonAbility(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_ABANDON_ABILITY; }
		virtual UINT			GetPacketSize() const { return sizeof(AbilityID_t); }

	public:
		VOID			setAbilityId(AbilityID_t id) { m_AbilityID = id; }
		AbilityID_t		getAbilityId() { return m_AbilityID; }

	private:
		AbilityID_t		m_AbilityID;

	};

	class GCAbandonAbilityFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCAbandonAbility(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_ABANDON_ABILITY; }
		UINT		GetPacketMaxSize() const {return sizeof(AbilityID_t); }
	};

	class GCAbandonAbilityHandler
	{
	public:
		static UINT Execute( GCAbandonAbility* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif// _GC_ABANDON_ABILITY_H_
