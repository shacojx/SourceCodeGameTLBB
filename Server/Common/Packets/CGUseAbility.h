// CGUseAbility.h

#ifndef _CG_USEABILITY_H_
#define _CG_USEABILITY_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGUseAbility:	public Packet
	{
	public:
		CGUseAbility(){};
		virtual		~CGUseAbility(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_USEABILITY; }
		virtual UINT			GetPacketSize()const { return sizeof(AbilityID_t)+sizeof(PrescriptionID_t)+sizeof(ObjID_t); }

		VOID					SetAbilityID(AbilityID_t aid){m_Ability = aid;}
		AbilityID_t				GetAbilityID(){return m_Ability;}

		VOID					SetPrescriptionID(PrescriptionID_t pid) { m_Prescription = pid; }
		PrescriptionID_t		GetPrescriptionID() { return m_Prescription; }

		VOID					SetPlatformGUID(ObjID_t guid) { m_Platform = guid; }
		ObjID_t					GetPlatformGUID() { return m_Platform; }
	private:
		
		AbilityID_t				m_Ability;
		PrescriptionID_t		m_Prescription;
		ObjID_t					m_Platform;
	};

	class CGUseAbilityFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGUseAbility() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_USEABILITY; }
		UINT		GetPacketMaxSize()const		{ return sizeof(AbilityID_t)
														+sizeof(PrescriptionID_t)
														+sizeof(ObjID_t); }
	};

	class CGUseAbilityHandler
	{
	public:
		static UINT Execute(CGUseAbility* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif // _CG_USEABILITY_H_
