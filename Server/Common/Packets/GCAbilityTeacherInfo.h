// GCAbilityTeacherInfo.h
// 
// 发送NPC的门派ID
// 
//////////////////////////////////////////////////////

#ifndef __GCABILITYSTUDYINFO_H__
#define __GCABILITYSTUDYINFO_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCAbilityTeacherInfo : public Packet
	{
	public:
		struct _AbilityTeacherInfo
		{
			AbilityID_t			m_nAbilityID;
			UINT				m_uSpendMoney;
			UINT				m_uSpendExp;
			UINT				m_uSkillExp;
			ObjID_t				m_uTeacherId;
			INT					m_nLevelLimit;
			UINT				m_uScriptId;
		};
		GCAbilityTeacherInfo( ){};
		virtual ~GCAbilityTeacherInfo( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_ABILITY_STUDYINFO ; }
		virtual UINT			GetPacketSize()const { return	sizeof(_AbilityTeacherInfo);}

	public:
		_AbilityTeacherInfo		GetAbilityTeacherInfo(VOID) const {return m_TeacherInfo;};
		VOID					SetAbilityTeacherInfo(_AbilityTeacherInfo TeacherInfo) 
		{
			m_TeacherInfo.m_nAbilityID	= TeacherInfo.m_nAbilityID ;
			m_TeacherInfo.m_uSkillExp	= TeacherInfo.m_uSkillExp  ;
			m_TeacherInfo.m_uSpendExp	= TeacherInfo.m_uSpendExp  ;
			m_TeacherInfo.m_uSpendMoney	= TeacherInfo.m_uSpendMoney;
			m_TeacherInfo.m_uTeacherId	= TeacherInfo.m_uTeacherId ;
			m_TeacherInfo.m_nLevelLimit = TeacherInfo.m_nLevelLimit;
			m_TeacherInfo.m_uScriptId	= TeacherInfo.m_uScriptId  ;
		};

	private:
		_AbilityTeacherInfo		m_TeacherInfo;

	};


	class GCAbilityTeacherInfoFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCAbilityTeacherInfo() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_ABILITY_STUDYINFO; };
		UINT		GetPacketMaxSize()const { return sizeof(GCAbilityTeacherInfo::_AbilityTeacherInfo);};
	};

	class GCAbilityTeacherInfoHandler 
	{
	public:
		static UINT Execute( GCAbilityTeacherInfo* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif


