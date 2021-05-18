#ifndef _CG_USE_EQUIP_SKILL_RESULT_H_
#define _CG_USE_EQUIP_SKILL_RESULT_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class GCUseEquipSkillResult:public Packet
	{

	public:
		GCUseEquipSkillResult(){};
		virtual				~GCUseEquipSkillResult(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_USEEQUIPSKILL_RESULT; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(USE_EQUIPSKILL_RESULT);
		}

	public:
		VOID			SetUseEquipSkillResult(USE_EQUIPSKILL_RESULT Result)
		{
			m_Result	=	Result;
		}
		
		USE_EQUIPSKILL_RESULT	GetEquipSkillResult()
		{
			return m_Result;
		}
		
	private:

		USE_EQUIPSKILL_RESULT	m_Result;

	};


	class GCUseEquipSkillResultFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCUseEquipSkillResult(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_USEEQUIPSKILL_RESULT; }
		UINT		GetPacketMaxSize() const { return	sizeof(USE_EQUIPSKILL_RESULT); }
	};


	class GCUseEquipSkillResultHandler
	{
	public:
		static UINT Execute( GCUseEquipSkillResult* pPacket, Player* pPlayer );
	};



}


#endif