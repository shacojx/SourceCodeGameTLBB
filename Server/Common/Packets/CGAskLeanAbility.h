// CGAskLeanAbility.h
// 
// 发送NPC的门派ID
// 
//////////////////////////////////////////////////////

#ifndef __CGASKSTUDYABILITY_H__
#define __CGASKSTUDYABILITY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGAskLeanAbility : public Packet
	{
	public:
		CGAskLeanAbility(){};
		virtual ~CGAskLeanAbility(){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKSTUDYABILITY; }
		virtual UINT			GetPacketSize()const { return	sizeof(AbilityID_t) + sizeof(INT) + sizeof(INT) + sizeof(INT);}

	public:
		AbilityID_t				GetAbilityTeacherInfo(VOID) const {return m_nAbilityID;};
		VOID					SetAbilityTeacherInfo(AbilityID_t nAbilityID) {m_nAbilityID = nAbilityID;}
		
		INT						GetNeedMoney(VOID)const {return m_nNeedMoney;}
		VOID					SetNeedMoney(INT nNeedMoney) {m_nNeedMoney = nNeedMoney;}

		INT						GetNeedExp(VOID)const {return m_nNeedExp;}
		VOID					SetNeedExpy(INT nNeedExp) {m_nNeedExp = nNeedExp;}

		INT						GetNeedSkillExp(VOID)const {return m_nNeedSkillExp;}
		VOID					SetNeedSkillExpy(INT nNeedSkillExp) {m_nNeedSkillExp = nNeedSkillExp;}
	private:
		AbilityID_t				m_nAbilityID;

		INT						m_nNeedMoney;
		INT						m_nNeedExp;
		INT						m_nNeedSkillExp;

	};


	class CGAskLeanAbilityFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGAskLeanAbility() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_ASKSTUDYABILITY; };
		UINT		GetPacketMaxSize()const { return sizeof(AbilityID_t) + sizeof(INT) + sizeof(INT) + sizeof(INT);};
	};

	class CGAskLeanAbilityHandler 
	{
	public:
		static UINT Execute( CGAskLeanAbility* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif


