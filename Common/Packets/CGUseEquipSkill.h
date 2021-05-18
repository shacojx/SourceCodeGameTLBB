#ifndef _CG_USE_EQUIP_SKILL_H_
#define _CG_USE_EQUIP_SKILL_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class CGUseEquipSkill:public Packet
	{

	public:
		CGUseEquipSkill(){};
		virtual				~CGUseEquipSkill(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_USEEQUIPSKILL; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)+sizeof(ObjID_t);
		}

	public:
		VOID			setSkillOffSet(BYTE offset){m_SkillOffSet = offset;}
		BYTE			getSkillOffSet(){return m_SkillOffSet;}

		VOID			setTarget(ObjID_t	Target){m_Target = Target;}
		ObjID_t			getTarget(){return m_Target;}
	private:
		
		BYTE			m_SkillOffSet;
		ObjID_t			m_Target;

	};


	class CGUseEquipSkillFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGUseEquipSkill(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_USEEQUIPSKILL; }
		UINT		GetPacketMaxSize() const { return	sizeof(BYTE)+sizeof(ObjID_t); }
	};


	class CGUseEquipSkillHandler
	{
	public:
		static UINT	Execute( CGUseEquipSkill* pPacket, Player* pPlayer );
	};



}


#endif