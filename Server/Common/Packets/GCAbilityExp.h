// GCAbilityExp.h

#ifndef _GC_ABILITY_EXP_H_
#define _GC_ABILITY_EXP_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCAbilityExp:public Packet
	{

	public:
		GCAbilityExp(){};
		virtual				~GCAbilityExp(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_ABILITYEXP; }
		virtual UINT			GetPacketSize() const { return sizeof(AbilityID_t)+sizeof(UINT); }

	public:
		
		VOID			setAbilityId(AbilityID_t id) {m_AbilityID = id;};
		AbilityID_t		getAbilityId() {return m_AbilityID;}
		
		VOID			setExp(UINT exp) {m_Exp = exp;}
		UINT			getExp() {return m_Exp;}

	private:
		UINT			m_Exp;
		AbilityID_t		m_AbilityID;

	};

	class GCAbilityExpFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCAbilityExp(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_ABILITYEXP; }
		UINT		GetPacketMaxSize() const {return sizeof(AbilityID_t)+sizeof(UINT); }
	};

	class GCAbilityExpHandler
	{
	public:
		static UINT Execute( GCAbilityExp* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // _GC_ABILITY_EXP_H_
