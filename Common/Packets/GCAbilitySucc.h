// GCAbilitySucc.h

#ifndef __GC_ABILITY_SUCC_H__
#define __GC_ABILITY_SUCC_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCAbilitySucc:	public Packet
	{
	public:
		GCAbilitySucc(){};
		virtual		~GCAbilitySucc(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_ABILITY_SUCC; }
		virtual UINT			GetPacketSize()const { return sizeof(AbilityID_t)+sizeof(UINT)+sizeof(PrescriptionID_t); }

		VOID					SetAbilityID(AbilityID_t aid){m_Ability = aid;}
		AbilityID_t				GetAbilityID(){return m_Ability;}

		VOID					SetPrescriptionID(PrescriptionID_t pid) { m_Prescription = pid; }
		PrescriptionID_t		GetPrescriptionID() { return m_Prescription; }

		VOID					SetItemSerial(UINT oid) { m_Obj = oid; }
		UINT					GetItemSerial() { return m_Obj; }
	private:
		
		AbilityID_t				m_Ability;
		PrescriptionID_t		m_Prescription;
		UINT					m_Obj;
	};

	class GCAbilitySuccFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCAbilitySucc() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_ABILITY_SUCC; }
		UINT		GetPacketMaxSize()const		{ return sizeof(AbilityID_t)+sizeof(UINT)+sizeof(PrescriptionID_t); }
	};

	class GCAbilitySuccHandler
	{
	public:
		static UINT Execute(GCAbilitySucc* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // __GC_ABILITY_SUCC_H__
